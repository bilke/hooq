#include "XmlToQtScript_FilterQtInternalsPostProcessor.h"

#include <QDebug>

XmlToQtScript::FilterQtInternalsPostProcessor::FilterQtInternalsPostProcessor()
{
	qDebug() << Q_FUNC_INFO;
	m_internalClasses
		<< "QShapedPixmapWidget" // Internal to qdnd_x11.{cpp,h} - shows the drag pixmap.
	;
}

XmlToQtScript::FilterQtInternalsPostProcessor::~FilterQtInternalsPostProcessor()
{
}

void XmlToQtScript::FilterQtInternalsPostProcessor::process(Item* iterator, QList<Item>* in, QList<Item>* out)
{
	if(m_internalClasses.contains(iterator->targetClass))
	{
//		qDebug() << Q_FUNC_INFO << "Skipping" << iterator->target << iterator->targetClass << iterator->method << iterator->parameters;
		*iterator = Item();
		if(in->first().method == "msec")
		{
			in->takeFirst();
		}
	}
	/*
	else if(!iterator->target.isNull())
	{
		qDebug() << Q_FUNC_INFO << "Keeping" << iterator->target << iterator->targetClass;
	}
	*/
}
