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
		Player(QIODevice* device);
	signals:
		void finished();
	private slots:
		void readNext();
	private:
		void handleElement();
		QObject* findObject(const QString& path);

		void postKeyEvent(int type);
		void postMouseEvent(int type);
		void postWheelEvent();
		void sleep(int msec);
};

} // namespace
