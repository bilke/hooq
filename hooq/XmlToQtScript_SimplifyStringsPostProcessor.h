#pragma once

#include "XmlToQtScript_PostProcessor.h"

class XmlToQtScript::SimplifyStringsPostProcessor : public PostProcessor
{
	public:
		SimplifyStringsPostProcessor();
		void process(Item* iterator, QList<Item>* in, QList<Item>* out);
};
