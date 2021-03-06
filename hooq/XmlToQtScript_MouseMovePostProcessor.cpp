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
#include "XmlToQtScript_MouseMovePostProcessor.h"

XmlToQtScript::StripMouseMovementsPostProcessor::StripMouseMovementsPostProcessor()
{
}

void XmlToQtScript::StripMouseMovementsPostProcessor::process(Item* iterator, QList<Item>* in)
{
	if(iterator->method == "moveMouse")
	{
		if(!in->isEmpty())
		{
			Q_ASSERT(in->first().method == "msleep");
			*iterator = in->takeFirst(); // SKIP
			Q_ASSERT(!in->isEmpty()); // MouseMove+msleep should *REALLY* not be the last thing
			*iterator = in->takeFirst();
			process(iterator, in);
		}
		else
		{
			*iterator = Item();
		}
	}
}
