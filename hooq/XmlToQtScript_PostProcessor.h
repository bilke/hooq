#pragma once

#include "XmlToQtScript.h"

class XmlToQtScript::PostProcessor
{
	public:
		enum Type
		{
			ForwardOnly
		};
		virtual ~PostProcessor();

		virtual Type type() const = 0;
	protected:
		PostProcessor();
};

class XmlToQtScript::ForwardOnlyPostProcessor : public XmlToQtScript::PostProcessor
{
	public:
		Type type() const;
		virtual ~ForwardOnlyPostProcessor();
		virtual void process(Item* iterator, QList<Item>* in, QList<Item>* out) const = 0;
	protected:
		ForwardOnlyPostProcessor();
};
