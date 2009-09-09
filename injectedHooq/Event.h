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
			virtual Type type() const = 0;
	};

	/// Abstract base class for events that reference an object.
	class PathEvent: public Event
	{
		public:
			QString objectPath() const;
			virtual ~PathEvent();
			// NOT IMPLEMENTED: virtual Type type();
		protected:
			PathEvent(const QString& objectPath);
		private:
			QString m_objectPath;
	};

	class DumpEvent : public PathEvent
	{
		public:
			DumpEvent(const QString& objectPath);
			Type type() const;
	};

	class FocusEvent: public PathEvent
	{
		public:
			FocusEvent(const QString& objectPath, Qt::FocusReason reason);
			virtual ~FocusEvent();
			Type type() const;
			Qt::FocusReason reason() const;
		private:
			Qt::FocusReason m_reason;
	};

	class ObjectEvent : public PathEvent
	{
		public:
			ObjectEvent(const QString& objectPath, QEvent* qtEvent);
			virtual ~ObjectEvent();
			Type type() const;

			QEvent* qtEvent() const;
		private:
			QEvent* m_qtEvent;
	};

	class PickEvent : public Event
	{
		public:
			PickEvent();
			Type type() const;
	};

	class SleepEvent : public Event
	{
		public:
			SleepEvent(int msec);
			virtual ~SleepEvent();
			Type type() const;

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

		// Segfaults if ::type() depends on class data. "Shouldn't" happen
		const bool sameType = static_cast<T>(0)->type() == e->type();
		Q_ASSERT(sameType);

		if(sameType)
		{
			return static_cast<T>(e);
		}
		return 0;
	}
};
