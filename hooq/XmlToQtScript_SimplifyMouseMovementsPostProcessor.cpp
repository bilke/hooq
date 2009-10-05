/*
	Hooq: Qt4 UI recording, playback, and testing toolkit.
	Copyright (C) 2009  Mendeley Limited <copyright@mendeley.com>
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
#include "XmlToQtScript_SimplifyMouseMovementsPostProcessor.h"

#include <QDebug>
#include <QString>
#include <QVariant>

XmlToQtScript::SimplifyMouseMovementsPostProcessor::SimplifyMouseMovementsPostProcessor()
{
}

void XmlToQtScript::SimplifyMouseMovementsPostProcessor::process(Item* iterator, QList<Item>* in, QList<Item>* out)
{
	if(iterator->method == "moveMouse")
	{
		const QVariant target = iterator->target;
		QVariantMap parameters = iterator->parameters.toMap();
		const QPoint origin = QPoint(
			parameters.value("x").toInt(),
			parameters.value("y").toInt()
		);
		int duration == 0;
		Q_FOREVER
		{
			Item msec;
			msec = in->takeFirst(); // msec << pointing at
			duration += msec.parameters.toInt();

			*iterator = in->takeFirst(); // ? Unknown
			if(iterator->method != "mouseMove" || iterator->target != target)
			{
//				qDebug() << Q_FUNC_INFO << __LINE__ << iterator->method;
				break;
			}
			parameters = iterator->parameters.toMap();
		}
		const QPoint destination = QPoint(
			parameters.value("x").toInt(),
			parameters.value("y").toInt()
		);
		parameters.remove("x");
		parameters.remove("y");
		parameters.insert("originX", origin.x());
		parameters.insert("originY", origin.y());
		parameters.insert("destinationX", destination.x());
		parameters.insert("destinationY", destination.y());
		parameters.insert("duration", duration);
		out->append(
			Item(
				target,
				"moveMouse",
				parameters
			)
		);
	}
}
