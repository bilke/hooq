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

		bool paused() const;

		// QScriptEngineAgent
		virtual void positionChange(qint64 scriptId, int lineNumber, int columnNumber);
	public slots:
		void open(const QString& filePath);
	signals:
		void pickRequested();
	private slots:
		void save();
		void revert();
		void handleMarginAction(int margin, int line, Qt::KeyboardModifiers state);
		void toggleBreakPoint(int line);
		void run();
	private:
		void pause();

		int m_breakPointMarker;
		QSet<int> m_breakPoints;

		int m_currentLineMarker;
		int m_currentLine;
		QsciScintilla* m_editor;
		QString m_filePath;

		bool m_paused;
};
