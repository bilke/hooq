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
#include "XmlToQtScript_ObjectVariablesPostProcessor.h"

#include <QSet>
#include <QStringList>

XmlToQtScript::ObjectVariablesPostProcessor::ObjectVariablesPostProcessor(XmlToQtScript::Item::Inserter* inserter)
: PostProcessor(inserter)
{
}

void XmlToQtScript::ObjectVariablesPostProcessor::process(Item* iterator, QList<Item>* in)
{
	Q_UNUSED(in);
	if(iterator->target.isNull())
	{
		return;
	}
	if(iterator->target.canConvert<Variable>())
	{
		Q_ASSERT(!iterator->target.canConvert<Variable>());
		return; // huh, what? How did we get here?
	}
	const QString target = iterator->target.toString();
	if(!m_variables.contains(target))
	{
		QStringList parts = target.split("::");
		QString name = toKeyword(parts.takeLast());
		const QSet<QString> values = m_variables.values().toSet();
		if(values.contains(name))
		{
			if(!parts.isEmpty())
			{
				name.prepend(toKeyword(parts.takeLast()) + "__");
			}
			if(values.contains(name))
			{
				const QString pattern = name + "_%1";
				int count = 2;
				while(values.contains(pattern.arg(count)))
				{
					++count;
				}
				name = pattern.arg(count);
			}
		}
		m_variables.insert(target, name);
		insertItem(
			Item(QString("var %1 = objectFromPath").arg(name), target),
			BeforeCurrentItem
		);
	}
	iterator->target = QVariant::fromValue(Variable(m_variables.value(target)));
}

QString XmlToQtScript::ObjectVariablesPostProcessor::toKeyword(const QString& in)
{
	QString out;
	Q_FOREACH(const QChar character, in)
	{
		if(character.isLetterOrNumber())
		{
			out.append(character);
		}
		else
		{
			out.append("_");
		}
	}
	return out;
}
