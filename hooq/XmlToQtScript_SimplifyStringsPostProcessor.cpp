/*
	Hooq: Qt4 UI recording, playback, and testing toolkit.
	Copyright (C) 2010  Mendeley Limited <copyright@mendeley.com>
	Copyright (C) 2009  Frederick Emmott <mail@fredemmott.co.uk>

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#include "XmlToQtScript_SimplifyStringsPostProcessor.h"

#include <QDebug>
#include <QString>
#include <QVariant>

Q_DECLARE_METATYPE(Qt::Key);
Q_DECLARE_METATYPE(Qt::KeyboardModifiers);

XmlToQtScript::SimplifyStringsPostProcessor::SimplifyStringsPostProcessor(XmlToQtScript::Item::Inserter* inserter)
: PostProcessor(inserter)
{
}

void XmlToQtScript::SimplifyStringsPostProcessor::process(Item* iterator, QList<Item>* in)
{
	if(iterator->method == "pressKey")
	{
		const QVariant target = iterator->target;
		const QString targetClass = iterator->targetClass;
		QVariantMap parameters = iterator->parameters.toMap();
		Qt::KeyboardModifiers modifiers = parameters["modifiers"].value<Qt::KeyboardModifiers>();
		QString string;
		Item msec;
		while(! (modifiers & ~Qt::ShiftModifier) )
		{
			if(iterator->method == "pressKey")
			{
				if(in->count() < 2) // no release key, let it stay as a press
				{
					break;
				}

				// Now, check the next non-sleep event is a keyrelease
				const QString nextMethod = in->at(1).method;
				if(
					!(
						(nextMethod == "releaseKey" || nextMethod == "pressKey")
						&&
						in->at(1).target == target
					)
				)
				{
					break;
				}

				switch(parameters["key"].value<Qt::Key>())
				{
					case Qt::Key_Backspace:
						string.chop(1);
						break;
					case Qt::Key_Shift:
						break;
					case Qt::Key_Return:
					case Qt::Key_Enter:
						string.append("\n");
						break;
					default:
						string.append(parameters.value("text").toString());
				}
			}
			
			if(in->isEmpty())
			{
				break;
			}
			msec = in->takeFirst(); // msec << pointing at
			*iterator = in->takeFirst(); // ?
			if(iterator->method != "pressKey" && iterator->method != "releaseKey")
			{
//				qDebug() << Q_FUNC_INFO << __LINE__ << iterator->method;
				break;
			}
			parameters = iterator->parameters.toMap();
			modifiers = parameters["modifiers"].value<Qt::KeyboardModifiers>();
		}
		if(!string.isEmpty())
		{
			insertItem(
				Item(
					target,
					targetClass,
					"sendText",
					string
				),
				BeforeCurrentItem
			);
			insertItem(msec, BeforeCurrentItem);
		}
	}
}
