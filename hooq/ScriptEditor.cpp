#include "ScriptEditor.h"
#include <Qsci/qscilexerjavascript.h>
#include <Qsci/qsciscintilla.h>

#include <QDebug>
#include <QFile>
#include <QFont>

ScriptEditor::ScriptEditor(QWidget* parent)
: QMainWindow(parent)
, m_editor(new QsciScintilla(this))
{
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
}

void ScriptEditor::handleMarginAction(int margin, int line, Qt::KeyboardModifiers state)
{
	Q_UNUSED(margin);
	Q_UNUSED(state);
	toggleBreakPoint(line);
}

void ScriptEditor::toggleBreakPoint(int line)
{
	qDebug() << "Setting marker on" << line << m_breakPointMarker;
	m_editor->markerAdd(line, m_breakPointMarker);
}

void ScriptEditor::open(const QString& filePath)
{
	m_filePath = filePath;
	QFile file(filePath);
	file.open(QIODevice::ReadOnly);
	m_editor->read(&file);
}
