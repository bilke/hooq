#pragma  once

#include <QFile>
#include <QHash>
#include <QObject>
#include <QPointer>
#include <QString>
#include <QTextStream>

class QUrl;

class GlobalEventPlayer : public QObject
{
	Q_OBJECT;
	public:
		static void run();
		static void setLogFile(const QString& targetFilePath);
	private slots:
		void readNext();
	private:
		GlobalEventPlayer(QObject* parent = 0);
		~GlobalEventPlayer();

		QTextStream m_logStream;
		static QFile m_logFile;
		QHash<QString, QPointer<QObject> > m_objectCache;
		QObject* findObject(const QString& path);

		void postKeyEvent(QObject* object, int type, const QUrl& url);
		void postMouseEvent(QObject* object, int type, const QUrl& url);
		void sleep(int msec);
};
