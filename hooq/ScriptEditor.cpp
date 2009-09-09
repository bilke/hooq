/*
	Hooq: Qt4 UI recording, playback, and testing toolkit.
	Copyright (C) 2009  Mendeley Limited <copyright@mendeley.com>
	Copyright (C) 2009  Frederick Emmott <mail@fredemmott.co.uk>

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#include "ScriptEditor.h"

#include "BacktraceWidget.h"
#include "CodeEditor.h"
#include "ObjectInformation.h"
#include "PropertyDialog.h"

#include "qscripthighlighter.h"

#include <QAction>
#include <QColor>
#include <QCoreApplication>
#include <QDebug>
#include <QDockWidget>
#include <QFile>
#include <QKeySequence>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QScriptContext>
#include <QScriptContextInfo>
#include <QScriptEngine>
#include <QStyle>
#include <QTextBlock>
#include <QTextCursor>
#include <QTextDocument>
#include <QToolBar>

ScriptEditor::ScriptEditor(QScriptEngine* engine)
: QMainWindow()
, QScriptEngineAgent(engine)
, m_backtraceDockWidget(new QDockWidget(tr("Backtrace"), this))
, m_backtraceWidget(new BacktraceWidget(this))
, m_errorWidget(new QDockWidget(tr("Error"), this))
, m_errorLabel(new QLabel(this))
, m_currentLine(-1)
, m_dirty(false)
, m_editor(new CodeEditor(this))
, m_mode(Interactive)
, m_paused(false)
{
	new SharedTools::QScriptHighlighter(m_editor->document());
	m_backtraceDockWidget->setWidget(m_backtraceWidget);
	m_errorWidget->setWidget(m_errorLabel);
	m_errorLabel->setFrameShape(QFrame::StyledPanel);
	m_errorLabel->setFrameShadow(QFrame::Raised);
	m_errorLabel->setStyleSheet("background-color: red");
	addDockWidget(Qt::TopDockWidgetArea, m_errorWidget);
	m_errorWidget->hide();
	engine->setAgent(this);
	setCentralWidget(m_editor);

	QToolBar* toolBar = addToolBar(tr("Toolbar"));
	toolBar->setMovable(false);

	m_saveAction = toolBar->addAction(style()->standardIcon(QStyle::SP_DialogSaveButton), tr("Save"), this, SLOT(save()));
	m_discardAction = toolBar->addAction(style()->standardIcon(QStyle::SP_DialogDiscardButton), tr("Discard Changes"), this, SLOT(revert()));

	toolBar->addSeparator();

	m_runAction = toolBar->addAction(style()->standardIcon(QStyle::SP_MediaPlay), tr("Run"), this, SLOT(run()));
	m_stopAction = toolBar->addAction(style()->standardIcon(QStyle::SP_MediaStop), tr("Stop"), this, SLOT(stop()));

	toolBar->addSeparator();

	m_pickAction = toolBar->addAction(style()->standardIcon(QStyle::SP_FileLinkIcon), tr("Pick Property"), this, SIGNAL(pickRequested()));
	setupMenuBar();
	setupActionShortcuts();
	updateActionStates();

	connect(
		m_editor,
		SIGNAL(textChanged()),
		SLOT(markAsDirty())
	);
}

void ScriptEditor::markAsDirty()
{
	m_dirty = true;
	updateActionStates();
}

bool ScriptEditor::isDirty() const
{
	return m_dirty;
}

ScriptEditor::Mode ScriptEditor::mode() const
{
	return m_mode;
}

void ScriptEditor::setMode(Mode mode)
{
	m_mode = mode;
}

void ScriptEditor::setupActionShortcuts()
{
	m_saveAction->setShortcut(QKeySequence::Save);
	m_closeAction->setShortcut(QKeySequence::Close);
}

void ScriptEditor::setupMenuBar()
{
	QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(m_saveAction);
	fileMenu->addAction(m_discardAction);
	fileMenu->addSeparator();
	m_closeAction = fileMenu->addAction(tr("Close"), this, SLOT(hide()));

	QMenu* toolsMenu = menuBar()->addMenu(tr("&Tools"));
	toolsMenu->addAction(m_runAction);
	toolsMenu->addAction(m_stopAction);
	toolsMenu->addSeparator();
	toolsMenu->addAction(m_pickAction);
}

void ScriptEditor::setPaused(bool paused)
{
	m_paused = paused;
}

bool ScriptEditor::isPaused() const
{
	return m_paused;
}

void ScriptEditor::insertPropertyFetch(const QString& objectPath, const QString& property)
{
	insertLine(
		QString(
			"objectFromPath(\"%1\").property(\"%2\")\n"
		).arg(
			objectPath
		).arg(
			property
		)
	);
}

void ScriptEditor::scriptUnload(qint64 scriptId)
{
	Q_UNUSED(scriptId);
	updateActionStates();
}

void ScriptEditor::insertPropertyAssert(const QString& objectPath, const QString& property, const QVariant& value)
{
	insertLine(
		QString(
			"compare(objectFromPath(\"%1\").property(\"%2\"), %3);\n"
		).arg(
			objectPath
		).arg(
			property
		).arg(
			escapeValue(value)
		)
	);
}

void ScriptEditor::insertLine(const QString& text)
{
	QTextCursor cursor = m_editor->textCursor();
	const QTextDocument* const document = m_editor->document();
	const QTextBlock block = document->findBlockByNumber(m_currentLine - 1);
	cursor.setPosition(block.position());
	cursor.insertText(text);
	m_editor->setTextCursor(cursor);
	m_editor->ensureCursorVisible();
}

QString ScriptEditor::escapeValue(const QVariant& value)
{
	switch(value.type())
	{
		case QVariant::Bool:
			return value.toBool() ? "true" : "false";
		case QVariant::Int:
			return QString::number(value.toInt());
		case QVariant::String:
			return QString("\"%1\"").arg(value.toString().replace("\\", "\\\\").replace("\"", "\\\""));
		default:
			return QString();
	}
}

void ScriptEditor::objectPicked(const ObjectInformation& object)
{
	PropertyDialog* dialog = new PropertyDialog(object, this);
	connect(
		dialog,
		SIGNAL(fetchRequested(QString, QString)),
		SLOT(insertPropertyFetch(QString, QString))
	);
	connect(
		dialog,
		SIGNAL(compareRequested(QString, QString, QVariant)),
		SLOT(insertPropertyAssert(QString, QString, QVariant))
	);
	dialog->show();
}

void ScriptEditor::exceptionThrow(qint64 scriptId, const QScriptValue& exception, bool hasHandler)
{
	Q_UNUSED(scriptId);
	if(!hasHandler)
	{
		QScriptContext* context = engine()->currentContext();
		while(context && QScriptContextInfo(context).lineNumber() == -1)
		{
			context = context->parentContext();
		}
		Q_ASSERT(context);

		if(mode() == Interactive)
		{
			m_errorLabel->setText(
				tr("Uncaught exception: %1").arg(exception.toString())
			);

			m_errorWidget->show();
			m_backtraceWidget->setContext(context);
			addDockWidget(Qt::BottomDockWidgetArea, m_backtraceDockWidget);
			m_editor->highlightLine(QScriptContextInfo(context).lineNumber());
			show();
			raise();
		}
		updateActionStates();
		emit exceptionThrown(engine()->uncaughtException().toString(), engine()->uncaughtExceptionBacktrace());
	}
}

void ScriptEditor::pauseOnLine(int lineNumber)
{
	updateActionStates();
	m_editor->highlightLine(lineNumber);
	pause();
	m_editor->clearLineHighlight();
}

void ScriptEditor::positionChange(qint64 scriptId, int lineNumber, int columnNumber)
{
	Q_UNUSED(columnNumber);
	Q_UNUSED(scriptId);

	if(m_currentLine != lineNumber && m_editor->breakpoints().contains(lineNumber) && mode() == Interactive)
	{
		pauseOnLine(lineNumber);
	}
	m_currentLine = lineNumber;
}

void ScriptEditor::pause()
{
	setPaused();
	while(isPaused())
	{
		QCoreApplication::processEvents();
	}
}

void ScriptEditor::run()
{
	reset(DebuggingFeatures);

	if(isPaused())
	{
		setPaused(false);
		return;
	}
	// TODO
}

void ScriptEditor::stop()
{
	engine()->abortEvaluation();
	setPaused(false);
	reset(DebuggingFeatures);
}

void ScriptEditor::updateActionStates()
{
	m_saveAction->setEnabled(isDirty());
	m_discardAction->setEnabled(isDirty());
	m_stopAction->setEnabled(engine()->isEvaluating());
	m_pickAction->setEnabled(engine()->isEvaluating());
}

void ScriptEditor::save()
{
	QFile file(m_filePath);
	file.open(QIODevice::WriteOnly | QFile::Truncate);
	file.write(m_editor->toPlainText().toUtf8());
	file.close();

	reset(DirtyState);
}

void ScriptEditor::revert()
{
	open(m_filePath);
}

void ScriptEditor::reset(int features)
{
	if(!features)
	{
		return;
	}

	if(features & BacktraceUi)
	{
		m_backtraceDockWidget->hide();
	}
	if(features & Breakpoints)
	{
		m_editor->clearBreakpoints();
	}
	if(features & CurrentLineUi)
	{
		m_editor->clearLineHighlight();
	}
	if(features & ErrorUi)
	{
		m_errorWidget->hide();
	}
	if(features & DirtyState)
	{
		m_dirty = false;
	}
	updateActionStates();
}

void ScriptEditor::open(const QString& filePath)
{
	m_filePath = filePath;
	QFile file(filePath);
	file.open(QIODevice::ReadOnly);
	m_editor->setPlainText(QString::fromUtf8(file.readAll()));
	reset();
}
