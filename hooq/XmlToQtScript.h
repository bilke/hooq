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

		void skipElement();
};
