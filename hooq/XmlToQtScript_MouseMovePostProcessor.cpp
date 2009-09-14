#include "XmlToQtScript_MouseMovePostProcessor.h"

XmlToQtScript::StripMouseMovementsPostProcessor::StripMouseMovementsPostProcessor()
{
}

void XmlToQtScript::StripMouseMovementsPostProcessor::process(Item* iterator, QList<Item>* in, QList<Item>* out) const
{
	Q_UNUSED(out);
	if(iterator->method == "moveMouse")
	{
		Q_ASSERT(in->first().method == "msleep");
		*iterator = in->takeFirst(); // SKIP
		Q_ASSERT(!in->isEmpty()); // MouseMove+msleep should *REALLY* not be the last thing
		*iterator = in->takeFirst();
		process(iterator, in, out);
	}
}
