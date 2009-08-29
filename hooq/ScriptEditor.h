#pragma once

#include <QMainWindow>
#include <QWidget>

class QsciScintilla;

class ScriptEditor : public QMainWindow
{
	Q_OBJECT;
	public:
		ScriptEditor(QWidget* parent);
	public slots:
		void open(const QString& filePath);
	private slots:
		void handleMarginAction(int margin, int line, Qt::KeyboardModifiers state);
		void toggleBreakPoint(int line);
	private:
		int m_breakPointMarker;
		QsciScintilla* m_editor;
		QString m_filePath;
};
