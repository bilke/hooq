#include "XmlToQtScript_SimplifyStringsPostProcessor.h"

#include <QDebug>
#include <QString>
#include <QVariant>

Q_DECLARE_METATYPE(Qt::Key);
Q_DECLARE_METATYPE(Qt::KeyboardModifiers);

XmlToQtScript::SimplifyStringsPostProcessor::SimplifyStringsPostProcessor()
{
}

void XmlToQtScript::SimplifyStringsPostProcessor::process(Item* iterator, QList<Item>* in, QList<Item>* out) const
{
	if(iterator->method == "pressKey")
	{
		const QVariant target = iterator->target;
		QVariantMap parameters = iterator->parameters.toMap();
		Qt::KeyboardModifiers modifiers = parameters["modifiers"].value<Qt::KeyboardModifiers>();
		QString string;
		Item msec;
		while(! (modifiers & ~Qt::ShiftModifier) )
		{
			if(iterator->method == "pressKey")
			{
				switch(parameters["key"].value<Qt::Key>())
				{
					case Qt::Key_Shift:
						break;
					case Qt::Key_Return:
					case Qt::Key_Enter:
						string.append("\n");
					default:
						string.append(parameters.value("text").toString());
				}
			}
			
			if(in->count() < 2)
			{
				break;
			}

			msec = in->takeFirst(); // msec << pointing at
			*iterator = in->takeFirst(); // ?
			if(iterator->method != "pressKey" && iterator->method != "releaseKey")
			{
				qDebug() << __LINE__ << iterator->method;
				break;
			}
			parameters = iterator->parameters.toMap();
			modifiers = parameters["modifiers"].value<Qt::KeyboardModifiers>();
		}
		if(!string.isEmpty())
		{
			out->append(
				Item(
					target,
					"sendText",
					string
				)
			);
			out->append(msec);
		}
	}
}
