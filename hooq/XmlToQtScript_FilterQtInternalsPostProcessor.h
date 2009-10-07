#pragma once
#include "XmlToQtScript_PostProcessor.h"

#include <QStringList>

class XmlToQtScript::FilterQtInternalsPostProcessor : public XmlToQtScript::PostProcessor
{
	public:
		FilterQtInternalsPostProcessor();
		~FilterQtInternalsPostProcessor();
		void process(Item* iterator, QList<Item>* in, QList<Item>* out);
	private:
		QStringList m_internalClasses;
};
