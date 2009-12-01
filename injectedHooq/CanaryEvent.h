#pragma once

#include <QEvent>

namespace Hooq
{
	/** QEvent subclass to avoid logging of infered events.
	 *
	 * After an event is logged, this will be appended to the event queue.
	 * Until it is received, no more events will be logged. This should avoid
	 * infered events being logged.
	 */
	class CanaryEvent : public QEvent
	{
		public:
			CanaryEvent();
			virtual ~CanaryEvent();
		private:
			static int m_type;
	};
};
