#include "XmlToQtScript_PostProcessor.h"

XmlToQtScript::PostProcessor::PostProcessor()
{
}

XmlToQtScript::PostProcessor::~PostProcessor()
{
}

XmlToQtScript::ForwardOnlyPostProcessor::ForwardOnlyPostProcessor()
: PostProcessor()
{
}

XmlToQtScript::ForwardOnlyPostProcessor::~ForwardOnlyPostProcessor()
{
}

XmlToQtScript::PostProcessor::Type XmlToQtScript::ForwardOnlyPostProcessor::type() const
{
	return ForwardOnly;
}
