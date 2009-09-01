#pragma once

#include <QMainWindow>
#include <QScriptEngineAgent>
#include <QSet>
#include <QWidget>

class ObjectInformation;

class QsciScintilla;

class QDockWidget;
class QLabel;

class ScriptEditor : public QMainWindow, public QScriptEngineAgent
{
	Q_OBJECT;
	public:
		enum Features
		{
			BacktraceUi,
			Breakpoints,
			CurrentLineUi,
			ErrorUi,
			AllFeatures = ~0
		};
		ScriptEditor(QScriptEngine* engine);

		bool isPaused() const;

		// QScriptEngineAgent
		virtual void exceptionThrow(qint64 scriptId, const QScriptValue& exception, bool hasHandler);
		virtual void positionChange(qint64 scriptId, int lineNumber, int columnNumber);
		virtual void scriptUnload(qint64 scriptId);
	public slots:
		void open(const QString& filePath);
		void objectPicked(const ObjectInformation&);
		void reset(int features = AllFeatures);
	signals:
		void pickRequested();
		void exceptionThrown(const QString& exception, const QStringList& backtrace);
	private slots:
		void save();
		void revert();
		void stop();
		void handleMarginAction(int margin, int line, Qt::KeyboardModifiers state);
		void toggleBreakPoint(int line);
		void run();

		void insertPropertyFetch(const QString& objectPath, const QString& property);
		void insertPropertyAssert(const QString& objectPath, const QString& property, const QVariant& value);
	private:
		void setupActionShortcuts();
		void setupMenuBar();
		void updateActionStates();
		void markLine(int lineNumber);
		void clearMark();
		void pauseOnLine(int lineNumber);
		void insertLine(const QString& text);
		QString escapeValue(const QVariant& value);
		void pause();
		void setPaused(bool paused = true);

		QDockWidget* m_backtraceWidget;
		QDockWidget* m_errorWidget;
		QLabel* m_errorLabel;

		int m_breakPointMarker;
		QSet<int> m_breakPoints;

		int m_currentLineMarker;
		int m_currentLine;
		QsciScintilla* m_editor;
		QString m_filePath;

		QAction* m_closeAction;
		
		QAction* m_saveAction;
		QAction* m_discardAction;

		QAction* m_runAction;
		QAction* m_stopAction;
		QAction* m_pickAction;

		bool m_paused;
};
