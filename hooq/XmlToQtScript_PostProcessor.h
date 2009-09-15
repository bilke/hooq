#pragma once

#include "XmlToQtScript.h"

class XmlToQtScript::PostProcessor
{
	public:
		virtual ~PostProcessor();
		virtual void process(Item* iterator, QList<Item>* in, QList<Item>* out) = 0;
	protected:
		PostProcessor();
};
