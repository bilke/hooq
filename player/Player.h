#pragma  once

#include <QObject>
#include <QString>
#include <QXmlStreamReader>

namespace Hooq
{

class Player: public QObject, private QXmlStreamReader
{
	Q_OBJECT;
	public:
		static void run();
	private slots:
		void readNext();
	private:
		void handleElement();
		QObject* findObject(const QString& path);

		void postKeyEvent(int type);
		void postMouseEvent(int type);
		void sleep(int msec);
};

} // namespace
