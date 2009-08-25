#pragma once

#include <QList>
#include <QPair>
#include <QString>

class QKeyEvent;
class QMouseEvent;

class GlobalEventHook
{
	public:
		static void activate();
	private:
		static bool hook(void** data);
		static void outputEvent(
			const QString& object,
			const char* action,
			const QList<QPair<QString, QString> >& data
		);

		static QString objectPath(QObject* object);
		static QString objectName(QObject* object);
		static QList<QPair<QString, QString> > formattedKeyEvent(QKeyEvent* event);
		static QList<QPair<QString, QString> > formattedMouseEvent(QMouseEvent* event);
};
