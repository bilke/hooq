#include "ScriptEditor.h"
#include <Qsci/qscilexerjavascript.h>
#include <Qsci/qsciscintilla.h>

#include <QAction>
#include <QDebug>
#include <QFile>
#include <QFont>
#include <QScriptEngine>
#include <QStyle>
#include <QToolBar>

ScriptEditor::ScriptEditor(QScriptEngine* engine)
: QMainWindow()
, QScriptEngineAgent(engine)
, m_editor(new QsciScintilla(this))
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
	m_editor->setMarginLineNumbers(0, true);
	m_editor->setMarginMarkerMask(0, ~0);
	m_editor->setMarginWidth(1, 2);
	m_editor->setMarginWidth(0, "00000");
	m_editor->setMarginSensitivity(0, true);
	m_breakPointMarker = m_editor->markerDefine(QsciScintilla::Circle);

	connect(
		m_editor,
		SIGNAL(marginClicked(int, int, Qt::KeyboardModifiers)),
		SLOT(handleMarginAction(int, int, Qt::KeyboardModifiers))
	);

	QToolBar* toolBar = addToolBar(tr("Toolbar"));
	toolBar->setMovable(false);

	toolBar->addAction(style()->standardIcon(QStyle::SP_DialogSaveButton), tr("Save"), this, SLOT(save()));
	toolBar->addAction(style()->standardIcon(QStyle::SP_DialogDiscardButton), tr("Discard Changes"), this, SLOT(revert()));

	toolBar->addSeparator();

	toolBar->addAction(style()->standardIcon(QStyle::SP_MediaPlay), tr("Run"));
	toolBar->addAction(style()->standardIcon(QStyle::SP_MediaStop), tr("Stop"));

	toolBar->addSeparator();

	toolBar->addAction(style()->standardIcon(QStyle::SP_FileLinkIcon), tr("Pick Property"));
}

void ScriptEditor::positionChange(qint64 scriptId, int lineNumber, int columnNumber)
{
	Q_UNUSED(columnNumber);
	Q_UNUSED(scriptId);
	if(m_breakPoints.contains(lineNumber))
	{
		qDebug() << "HIT BREAKPOINT" << lineNumber << columnNumber;
	}
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
	m_filePath = filePath;
	QFile file(filePath);
	file.open(QIODevice::ReadOnly);
	m_editor->read(&file);
}
