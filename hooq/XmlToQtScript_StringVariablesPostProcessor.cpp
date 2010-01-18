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
#include "XmlToQtScript_StringVariablesPostProcessor.h"

#include <QSet>
#include <QStringList>

XmlToQtScript::StringVariablesPostProcessor::StringVariablesPostProcessor(XmlToQtScript::Item::Inserter* inserter)
: PostProcessor(inserter)
{
}

void XmlToQtScript::StringVariablesPostProcessor::process(Item* iterator, QList<Item>* in)
{
	Q_UNUSED(in);
	if(iterator->method != "sendText")
	{
		return;
	}
	Q_ASSERT(iterator->parameters.type() == QVariant::String);
	const QString value = iterator->parameters.toString();

	if(!m_variables.contains(value))
	{
		const int position = m_variables.count();
		m_variables.append(value);
		insertItem(
			Item(QString("var string%1 = \"%2\"").arg(position).arg(XmlToQtScript::escapeString(value)), QVariant()),
			Beginning
		);
	}
	Q_ASSERT(m_variables.contains(value));
	iterator->parameters = QVariant::fromValue(Variable(QString("string%1").arg(m_variables.indexOf(value))));
}
