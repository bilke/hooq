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
#include <QVariant>
#include <QXmlStreamReader>

class XmlToQtScript : private QXmlStreamReader
{
	public:
		enum Option
		{
			NoOptions = 0,
			SkipMouseMovements =  1 << 0
		};
		Q_DECLARE_FLAGS(Options, Option);

		QString parse(QIODevice* xml, Options);
		static QString escapeString(const QString&);
	private:
		struct Item
		{
			Item(const QString& target, const QString& method, const QVariant& parameters);
			QString target;
			QString method;
			QVariant parameters; // Either a scalar, or QVariantMap
		};
		QString itemString(const QList<Item>& items) const;
		static QString parametersString(const QVariant& parameters);

		QString parseDocument();
		QString parseHooq();

		Item parseMsec();

		Item parseFocusEvent();
		Item parseKeyEvent();
		Item parseMouseEvent();
		Item parseWheelEvent();
		Item parseShortcutEvent();
		Item parseContextMenuEvent();

		static QString stringForMouseButton(int button);
		static QString stringForMouseButtons(int buttons);

		static QString stringForModifier(int modifier);
		static QString stringForModifiers(int modifier);

		static QString stringForFocusReason(int focusReason);

		void skipElement();

		Options m_options;

		// Abstract PostProcessors
		class ForwardOnlyPostProcessor;
		class PostProcessor;

		// Instantiable PostProcessors
		class StripMouseMovementsPostProcessor;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(XmlToQtScript::Options);
