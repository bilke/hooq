#pragma  once

#include <QFile>
#include <QHash>
#include <QObject>
#include <QPointer>
#include <QString>
#include <QXmlStreamReader>

namespace Hooq
{

class Player: public QObject, private QXmlStreamReader
{
	Q_OBJECT;
	public:
		static void run();
		static void setLogFile(const QString& targetFilePath);
	private slots:
		void readNext();
	private:
		void handleElement();
		static QFile m_logFile;
//		QHash<QString, QPointer<QObject> > m_objectCache;
		QObject* findObject(const QString& path);

		void postKeyEvent(int type);
		void postMouseEvent(int type);
		void sleep(int msec);
};

} // namespace
