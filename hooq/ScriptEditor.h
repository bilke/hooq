#pragma once

#include <QMainWindow>
#include <QScriptEngineAgent>
#include <QSet>
#include <QWidget>

class QsciScintilla;

class ScriptEditor : public QMainWindow, public QScriptEngineAgent
{
	Q_OBJECT;
	public:
		ScriptEditor(QScriptEngine* engine);

		// QScriptEngineAgent
		virtual void positionChange(qint64 scriptId, int lineNumber, int columnNumber);
	public slots:
		void open(const QString& filePath);
	private slots:
		void save();
		void revert();
		void handleMarginAction(int margin, int line, Qt::KeyboardModifiers state);
		void toggleBreakPoint(int line);
	private:
		int m_breakPointMarker;
		int m_currentLineMarker;
		QsciScintilla* m_editor;
		QString m_filePath;

		QSet<int> m_breakPoints;
};
