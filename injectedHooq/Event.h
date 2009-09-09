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
			virtual ~Event();
			enum Type
			{
				// PathEvent not listed here, as it's abstract
				Dump,
				Focus,
				Object,
				Pick,
				Sleep
			};
			// static const Event::Type EVENT_TYPE = TypeValueGoesHere; // must be in every instantiable subclass, for event_cast
			Type type() const;
		protected:
			Event(Type type);
		private:
			Type m_type;
	};

	/// Abstract base class for events that reference an object.
	class PathEvent: public Event
	{
		public:
			QString objectPath() const;
			virtual ~PathEvent();
			// NOT IMPLEMENTED: virtual Type type();
		protected:
			PathEvent(const QString& objectPath, Type type);
		private:
			QString m_objectPath;
	};

	class DumpEvent : public PathEvent
	{
		public:
			static const Event::Type EVENT_TYPE = Dump;
			DumpEvent(const QString& objectPath);
	};

	class FocusEvent: public PathEvent
	{
		public:
			static const Event::Type EVENT_TYPE = Focus;
			FocusEvent(const QString& objectPath, Qt::FocusReason reason);
			virtual ~FocusEvent();
			Qt::FocusReason reason() const;
		private:
			Qt::FocusReason m_reason;
	};

	class ObjectEvent : public PathEvent
	{
		public:
			static const Event::Type EVENT_TYPE = Object;
			ObjectEvent(const QString& objectPath, QEvent* qtEvent);
			virtual ~ObjectEvent();

			QEvent* qtEvent() const;
		private:
			QEvent* m_qtEvent;
	};

	class PickEvent : public Event
	{
		public:
			static const Event::Type EVENT_TYPE = Pick;
			PickEvent();
	};

	class SleepEvent : public Event
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

		const Event::Type wantedType = static_cast<T>(0)->EVENT_TYPE;
		const bool sameType = e->type() == wantedType;
		Q_ASSERT(sameType);

		if(sameType)
		{
			return static_cast<T>(e);
		}
		return 0;
	}
};
