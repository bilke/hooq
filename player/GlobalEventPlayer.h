#pragma  once

#include <QFile>
#include <QHash>
#include <QObject>
#include <QPointer>
#include <QString>

class QUrl;

class GlobalEventPlayer
{
	public:
		static void run();
		static void setLogFile(const QString& targetFilePath);
	private:
		static QFile m_logFile;
		static QHash<QString, QPointer<QObject> > m_objectCache;
		static QObject* findObject(const QString& path);

		static void postMouseEvent(QObject* object, int type, const QUrl& url);
		static void sleep(int msec);
};
