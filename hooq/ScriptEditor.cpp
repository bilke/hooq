#include "ScriptEditor.h"

#include "BacktraceWidget.h"
#include "ObjectInformation.h"
#include "PropertyDialog.h"

#include <Qsci/qscilexerjavascript.h>
#include <Qsci/qsciscintilla.h>

#include <QCoreApplication>
#include <QAction>
#include <QColor>
#include <QDebug>
#include <QDockWidget>
#include <QFile>
#include <QFont>
#include <QKeySequence>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QScriptEngine>
#include <QStyle>
#include <QToolBar>

ScriptEditor::ScriptEditor(QScriptEngine* engine)
: QMainWindow()
, QScriptEngineAgent(engine)
, m_backtraceWidget(0)
, m_currentLine(-1)
, m_editor(new QsciScintilla(this))
, m_paused(false)
{
	engine->setAgent(this);
	setCentralWidget(m_editor);

	QFont codeFont(font());
	codeFont.setFamily("monospace");
	codeFont.setStyleHint(QFont::TypeWriter);

	QsciLexer* lexer = new QsciLexerJavaScript(m_editor);
	lexer->setDefaultFont(codeFont);

	m_editor->setUtf8(true);
	m_editor->setLexer(lexer);
	m_editor->setMarginMarkerMask(0, 1); // bitfield - 1 == first user-assigned marker (breakpoint)
	m_editor->setMarginLineNumbers(0, true);
	m_editor->setMarginWidth(1, 2);
	m_editor->setMarginWidth(0, "00000");
	m_editor->setMarginSensitivity(0, true);

	m_breakPointMarker = m_editor->markerDefine(QsciScintilla::Circle);
	m_editor->setMarkerForegroundColor(Qt::black, m_breakPointMarker);
	m_editor->setMarkerBackgroundColor(Qt::red, m_breakPointMarker);

	m_currentLineMarker = m_editor->markerDefine(QsciScintilla::Background);
	m_editor->setMarkerBackgroundColor(QColor::fromRgb(255,140,0), m_currentLineMarker); // orange

	connect(
		m_editor,
		SIGNAL(marginClicked(int, int, Qt::KeyboardModifiers)),
		SLOT(handleMarginAction(int, int, Qt::KeyboardModifiers))
	);

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
			"assert(objectFromPath(\"%1\").property(\"%2\") == %3);\n"
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
	m_editor->insertAt(text, m_currentLine, 0);
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
		Q_UNUSED(exception); // FIXME
		delete m_backtraceWidget;
		QWidget* widget = new BacktraceWidget(engine()->currentContext(), this);
		m_backtraceWidget = new QDockWidget(tr("Backtrace"), this);
		m_backtraceWidget->setWidget(widget);
		addDockWidget(Qt::BottomDockWidgetArea, m_backtraceWidget);
	}
}

void ScriptEditor::pauseOnLine(int lineNumber)
{
	updateActionStates();
	m_editor->markerAdd(lineNumber - 1, m_currentLineMarker);
	pause();
	m_editor->markerDeleteAll(m_currentLineMarker);
}

void ScriptEditor::positionChange(qint64 scriptId, int lineNumber, int columnNumber)
{
	Q_UNUSED(columnNumber);
	Q_UNUSED(scriptId);

	if(m_currentLine != lineNumber && m_breakPoints.contains(lineNumber))
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
	updateActionStates();
}

void ScriptEditor::updateActionStates()
{
	m_stopAction->setEnabled(engine()->isEvaluating());
	m_pickAction->setEnabled(engine()->isEvaluating());
}

void ScriptEditor::save()
{
	QFile file(m_filePath);
	file.open(QIODevice::WriteOnly | QFile::Truncate);
	m_editor->write(&file);
	file.close();
}

void ScriptEditor::revert()
{
	open(m_filePath);
}

void ScriptEditor::handleMarginAction(int margin, int line, Qt::KeyboardModifiers state)
{
	Q_UNUSED(margin);
	Q_UNUSED(state);
	toggleBreakPoint(line + 1);
}

void ScriptEditor::toggleBreakPoint(int line)
{
	if(m_breakPoints.contains(line))
	{
		m_editor->markerDelete(line - 1, m_breakPointMarker);
		m_breakPoints.remove(line);
	}
	else
	{
		m_editor->markerAdd(line - 1, m_breakPointMarker);
		m_breakPoints.insert(line);
	}
}

void ScriptEditor::open(const QString& filePath)
{
	m_editor->markerDeleteAll();
	m_breakPoints.clear();
	m_filePath = filePath;
	QFile file(filePath);
	file.open(QIODevice::ReadOnly);
	m_editor->read(&file);
}
