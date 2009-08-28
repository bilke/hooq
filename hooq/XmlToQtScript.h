#pragma once

#include <QXmlStreamReader>

class XmlToQtScript : private QXmlStreamReader
{
	public:
		QString parse(QIODevice* xml);
	private:
		QString parseDocument();
		QString parseHooq();
		QString parseMsec();
		QString parseKeyEvent();
		QString parseMouseEvent();
		QString parseWheelEvent();

		static QString stringForMouseButton(int button);
		static QString stringForMouseButtons(int buttons);

		static QString stringForModifier(int modifier);
		static QString stringForModifiers(int modifier);

		void skipElement();
};
