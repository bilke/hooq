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
		struct Variable	{ public: Variable(); Variable(const QString& name); QString name; };
		struct Item
		{
			Item();
			Item(const QString& method, const QVariant& parameters);
			Item(const QVariant& target, const QString& targetClass, const QString& method, const QVariant& parameters);
			bool isValid() const;
			QVariant target; // string or Variable
			QString targetClass;
			QString method;
			QVariant parameters; // A scalar, QVariantMap, or Item
		};
		enum Option
		{
			NoOptions = 0,
			SkipMouseMovements     = 1 << 0,
			SimplifyStrings        = 1 << 1,
			ObjectVariables        = 1 << 2,
			StringVariables        = 1 << 3,
			SimplifyMouseMovements = 1 << 4,
			FilterQtInternals      = 1 << 5
		};
		Q_DECLARE_FLAGS(Options, Option);

		QString parse(QIODevice* xml, Options);
		static QString escapeString(const QString&);
	private:
		QString itemString(const QList<Item>& items) const;
		/// Raw serialization - you probably want itemString instead
		QString serialize(const QList<Item>& items) const;
		QString parametersString(const QVariant& parameters) const;

		QString parseDocument();
		QString parseHooq();

		Item parseMsec();

		Item parseCloseEvent();
		Item parseFocusEvent();
		Item parseKeyEvent();
		Item parseMouseEvent();
		Item parseWheelEvent();
		Item parseShortcutEvent();
		Item parseContextMenuEvent();
		Item parseDragAndDropEvent();

		static QString stringForMouseButton(int button);
		static QString stringForMouseButtons(int buttons);

		static QString stringForModifier(int modifier);
		static QString stringForModifiers(int modifier);

		static QString stringForFocusReason(int focusReason);

		void skipElement();

		Options m_options;

		// Abstract PostProcessors
		class PostProcessor;

		// Instantiable PostProcessors
		class SimplifyStringsPostProcessor;
		class StringVariablesPostProcessor;
		class StripMouseMovementsPostProcessor;
		class ObjectVariablesPostProcessor;
		class SimplifyMouseMovementsPostProcessor;
		class FilterQtInternalsPostProcessor;
};

Q_DECLARE_METATYPE(XmlToQtScript::Variable);
Q_DECLARE_METATYPE(XmlToQtScript::Item);

Q_DECLARE_OPERATORS_FOR_FLAGS(XmlToQtScript::Options);
