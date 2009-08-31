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
#include <QXmlStreamReader>

class QWidget;

namespace Hooq
{
class Event;

class Player: public QObject, private QXmlStreamReader
{
	Q_OBJECT;
	public:
		static Player* instance(QIODevice*);
	signals:
		void finished();
	private slots:
		void waitFinished();
		void readNext();
		void processEvents();
	private:
		static Player* instance();
		Player(QIODevice* device);
		static QPointer<Player> m_instance;

		void startPick();
		void endPick();

		/// The main hook.
		static bool hook(void** data);
		bool hook(QObject* receiver, QEvent* event);

		void handleElement();
		QObject* findObject(const QString& path);

		void postDumpEvent();
		void postPickEvent();
		void postSleepEvent();

		void postKeyEvent(int type);
		void postMouseEvent(int type);
		void postShortcutEvent();
		void postWheelEvent();
		void sleep(int msec);

		void ack();

		bool m_processingEvents;

		QQueue<Event*> m_eventQueue;

		QWidget* m_pickWidget;
};

} // namespace
