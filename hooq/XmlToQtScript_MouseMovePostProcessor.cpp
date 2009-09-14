#include "XmlToQtScript_MouseMovePostProcessor.h"

XmlToQtScript::StripMouseMovementsPostProcessor::StripMouseMovementsPostProcessor()
{
}

void XmlToQtScript::StripMouseMovementsPostProcessor::process(Item* iterator, QList<Item>* in, QStringList* out, bool* skip) const
{
	Q_UNUSED(out);
	if(iterator->method == "moveMouse")
	{
		Q_ASSERT(in->first().method == "msleep");
		in->takeFirst();
		*skip = true;
	}
}
