#include "CanaryEvent.h"

namespace Hooq
{
	int CanaryEvent::m_type = QEvent::registerEventType();

	CanaryEvent::CanaryEvent()
	: QEvent(static_cast<QEvent::Type>(m_type))
	{
	}

	CanaryEvent::~CanaryEvent()
	{
	}
};
