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

#include <QString>

class QEvent;

namespace Hooq
{
	class Event
	{
		public:
			Event();
			virtual ~Event();
			enum Type
			{
				// PathEvent not listed here, as it's abstract
				Ack,    ///< No-operation event, just send the frontend an ack
				Dump,   ///< Request for all the Q_PROPERTYs of a given QObject
				Focus,  ///< A focus request for a given QObject
				Object, ///< A QEvent to be sent to a QObject
				Pick,   ///< Request for the user to click on a QObject, then treat as the target of a Dump event
				Sleep,  ///< Wait a certain amount of time
				Flush   ///< Call qApp->processEvents - be careful with this, may block Hooq
			};
			virtual Type type() const = 0;
			bool ack() const;
			void setAck(bool);

			QString tag() const;

			static Event* withoutAck(Event* event);
			static Event* addTag(const QString& tag, Event* event);
		private:
			bool m_ack;
			QString m_tag;
	};

	template<class Derived, class Base> class TypedEvent : public Base
	{
		public:
			TypedEvent() : Base() {}
			TypedEvent(const QString& p) : Base(p) {}

			static Event::Type staticType() { return Derived::EVENT_TYPE; };
			virtual Event::Type type() const { return staticType(); }
	};


	class AckEvent : public TypedEvent<AckEvent,Event>
	{
		public:
			static const Event::Type EVENT_TYPE = Ack;
	};

	class FlushEvent : public TypedEvent<FlushEvent,Event>
	{
		public:
			static const Event::Type EVENT_TYPE = Flush;
	};

	/// Abstract base class for events that reference an object.
	class PathEvent: public Event
	{
		public:
			QString objectPath() const;
			virtual ~PathEvent();
		protected:
			PathEvent(const QString& objectPath);
		private:
			QString m_objectPath;
	};

	class DumpEvent : public TypedEvent<DumpEvent, PathEvent>
	{
		public:
			static const Event::Type EVENT_TYPE = Dump;
			DumpEvent(const QString& objectPath);
	};

	class FocusEvent: public TypedEvent<FocusEvent, PathEvent>
	{
		public:
			static const Event::Type EVENT_TYPE = Focus;
			FocusEvent(const QString& objectPath, Qt::FocusReason reason);
			virtual ~FocusEvent();
			Qt::FocusReason reason() const;
		private:
			Qt::FocusReason m_reason;
	};

	class ObjectEvent : public TypedEvent<ObjectEvent, PathEvent>
	{
		public:
			static const Event::Type EVENT_TYPE = Object;
			ObjectEvent(const QString& objectPath, QEvent* qtEvent);
			virtual ~ObjectEvent();

			QEvent* qtEvent() const;
		private:
			QEvent* m_qtEvent;
	};

	class PickEvent : public TypedEvent<PickEvent, Event>
	{
		public:
			static const Event::Type EVENT_TYPE = Pick;
	};

	class SleepEvent : public TypedEvent<SleepEvent, Event>
	{
		public:
			static const Event::Type EVENT_TYPE = Sleep;
			SleepEvent(int msec);
			virtual ~SleepEvent();

			int msec() const;
		private:
			int m_msec;
	};

	template<class T> T event_cast(Event* e)
	{
		Q_ASSERT(e);
		if(!e)
		{
			return 0;
		}

		const Event::Type wantedType = static_cast<T>(0)->staticType();
		const bool sameType = e->type() == wantedType;
		Q_ASSERT(sameType);

		if(sameType)
		{
			return static_cast<T>(e);
		}
		return 0;
	}
};
