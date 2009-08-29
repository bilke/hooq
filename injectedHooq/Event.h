#pragma once

#include <QString>

class QEvent;

namespace Hooq
{
	class Event
	{
		public:
			enum Type
			{
				Sleep,
				Object
			};
			virtual Type type() const = 0;
	};

	class SleepEvent : public Event
	{
		public:
			SleepEvent(int msec);
			Type type() const;

			int msec() const;
		private:
			int m_msec;
	};

	class ObjectEvent : public Event
	{
		public:
			ObjectEvent(const QString& objectPath, QEvent* qtEvent);
			Type type() const;

			QString objectPath() const;
			QEvent* qtEvent() const;
		private:
			QString m_objectPath;
			QEvent* m_qtEvent;
	};
};
