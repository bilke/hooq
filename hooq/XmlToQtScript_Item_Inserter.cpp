/*
	Hooq: Qt4 UI recording, playback, and testing toolkit.
	Copyright (C) 2010  Mendeley Limited <copyright@mendeley.com>

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

#include "XmlToQtScript_Item_Inserter.h"

XmlToQtScript::Item::Inserter::Inserter(QList<Item>* rootList)
: m_rootList(rootList)
{
	reset();
}

void XmlToQtScript::Item::Inserter::reset()
{
	m_originalStart = 0;
	m_itemIndex = m_rootList->count() - 1;
	m_appendedItems = 0;
}

void XmlToQtScript::Item::Inserter::insert(const Item& item, Position position)
{
	switch(position)
	{
		case Beginning:
			m_rootList->insert(m_originalStart, item);
			++m_originalStart;
			++m_itemIndex;
			break;
		case BeforeCurrentItem:
			m_rootList->insert(m_itemIndex, item);
			++m_itemIndex;
			break;
/*		case AfterCurrentItem:
			++m_appendedItems;
			m_rootList->insert(m_itemIndex + m_appendedItems, item);
			break; */
	}
}

void XmlToQtScript::Item::Inserter::next()
{
	m_itemIndex += m_appendedItems + 1;
	m_appendedItems = 0;
}
