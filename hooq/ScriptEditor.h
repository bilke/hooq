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
		enum Mode
		{
			Headless,
			Interactive
		};
		ScriptEditor(QScriptEngine* engine);

		bool isPaused() const;

		// QScriptEngineAgent
		virtual void exceptionThrow(qint64 scriptId, const QScriptValue& exception, bool hasHandler);
		virtual void positionChange(qint64 scriptId, int lineNumber, int columnNumber);
		virtual void scriptUnload(qint64 scriptId);

		Mode mode() const;
		void setMode(Mode mode);
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
		Mode m_mode;
		bool m_paused;

		QAction* m_closeAction;
		
		QAction* m_saveAction;
		QAction* m_discardAction;

		QAction* m_runAction;
		QAction* m_stopAction;
		QAction* m_pickAction;
};
