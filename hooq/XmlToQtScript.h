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
#pragma once

#include <QFlags>
#include <QXmlStreamReader>

class XmlToQtScript : private QXmlStreamReader
{
	public:
		enum Option
		{
			NoOptions = 0x0,
			SkipMouseMovements = 0x1,
		};
		Q_DECLARE_FLAGS(Options, Option);

		QString parse(QIODevice* xml, Options);
		static QString escapeString(const QString&);
	private:
		QString parseDocument();
		QString parseHooq();
		QString parseMsec();

		QString parseFocusEvent();
		QString parseKeyEvent();
		QString parseMouseEvent();
		QString parseWheelEvent();
		QString parseShortcutEvent();
		QString parseContextMenuEvent();

		static QString stringForMouseButton(int button);
		static QString stringForMouseButtons(int buttons);

		static QString stringForModifier(int modifier);
		static QString stringForModifiers(int modifier);

		static QString stringForFocusReason(int focusReason);

		void skipElement();

		Options m_options;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(XmlToQtScript::Options);
