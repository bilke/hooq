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

#include <QObject>
#include <QPointer>
#include <QQueue>
#include <QString>
#include <QTextStream>
#include <QXmlStreamReader>

// Compatibility with Qt 4.4 is currently desirable (lenny, hardy)
// We don't actually want a boost dependency on Windows though, where
// whoever's using Hooq is likely to have full control of the Qt version
// used.
#if QT_VERSION >= 0x040500
# include <QSharedPointer>
# define INJECTEDHOOQ_WITH_QSHAREDPOINTER
  typedef QSharedPointer<QTextStream> TextStreamPointer;
#else
# include <boost/shared_ptr.hpp>
# define INJECTEDHOOQ_WITH_BOOST_SHARED_PTR
  typedef boost::shared_ptr<QTextStream> TextStreamPointer;
#endif

class QWidget;

namespace Hooq
{
class Event;

class Player: public QObject, private QXmlStreamReader
{
	Q_OBJECT;
	public:
		static Player* instance(QIODevice*);
		~Player();
		void run();
	signals:
		void finished();
	private slots:
		void waitFinished();
		void readNext();
		void processEvents();
	private:
		void debugPrintObjectTree(int nestingDepth = 0, QWidget* root = 0, TextStreamPointer stream = TextStreamPointer());
		static Player* instance();
		Player(QIODevice* device);
		static QPointer<Player> m_instance;

		void startPick();
		void endPick();

		/// The main hook.
		static bool hook(void** data);
		bool eventFilter(QObject* receiver, QEvent* event);

		void handleElement();
		QObject* findObject(const QString& path);

		void postDumpEvent();
		void postPickEvent();
		void postSleepEvent();

		void postFocusEvent();
		void postKeyEvent(int type);
		void postMouseEvent(int type);
		void postContextMenuEvent();
		void postShortcutEvent();
		void postWheelEvent();
		void sleep(int msec);

		void ack();

		bool m_processingEvents;

		QQueue<Event*> m_eventQueue;

		QWidget* m_pickWidget;
};

} // namespace
