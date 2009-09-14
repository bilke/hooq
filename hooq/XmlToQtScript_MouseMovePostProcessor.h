#pragma once

#include "XmlToQtScript_PostProcessor.h"

class XmlToQtScript::StripMouseMovementsPostProcessor : public ForwardOnlyPostProcessor
{
	public:
		StripMouseMovementsPostProcessor();
		void process(Item* iterator, QList<Item>* in, QList<Item>* out) const;
};
