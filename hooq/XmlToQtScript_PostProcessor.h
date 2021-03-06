/* Hooq: Qt4 UI recording, playback, and testing toolkit.
	Copyright (C) 2010  Mendeley Limited <copyright@mendeley.com>
	Copyright (C) 2009  Frederick Emmott <mail@fredemmott.co.uk>

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#pragma once

#include "XmlToQtScript.h"

class XmlToQtScript::PostProcessor
{
	public:
		virtual ~PostProcessor();
		virtual void process(Item* iterator, QList<Item>* in) = 0;
	protected:
		PostProcessor(XmlToQtScript::Item::Inserter* inserter);
		PostProcessor(); // only if you are /not/ inserting any items

		/// Where to insert a new Item
		enum Position
		{
			Beginning         = 0, ///< At the top of the entire script
			BeforeCurrentItem = 1  ///< Just before the current item, in the block scope
		};
		void insertItem(const Item& item, Position position);

		XmlToQtScript::Item::Inserter* m_inserter;
};
