#include "Logger.h"
#include "../lib/ObjectHookName.h"

#include <Qt>

#include <QEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QObject>
#include <QTextStream>
#include <QTime>
#include <QUrl>

namespace Hooq
{

QFile Logger::m_logFile;
QTime Logger::m_timer;

void Logger::setLogFile(const QString& targetFile)
{
	m_logFile.close();
	m_logFile.setFileName(targetFile);
	m_logFile.open(QIODevice::WriteOnly | QFile::Truncate);
}

void Logger::activate()
{
	// Internal API... seems unavoidable
	QInternal::registerCallback(QInternal::EventNotifyCallback, &hook);
	m_timer.start();
}

bool Logger::hook(void** data)
{
	QObject* receiver = reinterpret_cast<QObject*>(data[0]);
	QEvent* event = reinterpret_cast<QEvent*>(data[1]);
	// returnType* returnValue = reinterpret_cast<returnType*>(data[2])
	switch(event->type())
	{
		case QEvent::KeyPress:
			outputEvent(ObjectHookName::objectPath(receiver), "keyPress", formattedKeyEvent(static_cast<QKeyEvent*>(event)));
			break;
		case QEvent::KeyRelease:
			outputEvent(ObjectHookName::objectPath(receiver), "keyRelease", formattedKeyEvent(static_cast<QKeyEvent*>(event)));
			break;
		case QEvent::MouseMove:
			outputEvent(ObjectHookName::objectPath(receiver), "mouseMove", formattedMouseEvent(static_cast<QMouseEvent*>(event)));
			break;
		case QEvent::MouseButtonPress:
			outputEvent(ObjectHookName::objectPath(receiver), "mouseButtonPress", formattedMouseEvent(static_cast<QMouseEvent*>(event)));
			break;
		case QEvent::MouseButtonRelease:
			outputEvent(ObjectHookName::objectPath(receiver), "mouseButtonRelease", formattedMouseEvent(static_cast<QMouseEvent*>(event)));
			break;
		case QEvent::MouseButtonDblClick:
			outputEvent(ObjectHookName::objectPath(receiver), "mouseButtonDoubleClick", formattedMouseEvent(static_cast<QMouseEvent*>(event)));
			break;
		default:
			break;

	}
	return false;
}

QList<QPair<QString, QString> > Logger::formattedKeyEvent(QKeyEvent* event)
{
	QList<QPair<QString, QString> > data;
	data.append(qMakePair(QString("key"), QString::number(event->key()))); // Qt::Key
	data.append(qMakePair(QString("modifiers"), QString::number(event->modifiers()))); // Qt::Modifiers
	data.append(qMakePair(QString("text"), event->text()));
	data.append(qMakePair(QString("isAutoRepeat"), event->isAutoRepeat() ? QString("true") : QString("false")));
	data.append(qMakePair(QString("count"), QString::number(event->count())));
	return data;
}

QList<QPair<QString, QString> > Logger::formattedMouseEvent(QMouseEvent* event)
{
	QList<QPair<QString, QString> > data;
	data.append(qMakePair(QString("x"), QString::number(event->x())));
	data.append(qMakePair(QString("y"), QString::number(event->y())));
	data.append(qMakePair(QString("button"), QString::number(event->button())));
	data.append(qMakePair(QString("buttons"), QString::number(event->buttons())));
	return data;
}

void Logger::outputEvent(const QString& object, const char* action, const QList<QPair<QString, QString> >& data)
{
	QTextStream out(&m_logFile);

	QUrl timeUrl;
	timeUrl.setScheme("msec");
	timeUrl.setHost(QString::number(m_timer.restart()));
	out << qPrintable(timeUrl.toString()) << endl;

	QUrl url;
	url.setScheme("qevent");
	url.setHost(object);
	url.setPath(action);
	url.setQueryItems(data);
	out << qPrintable(url.toString()) << endl;
}

} //namespace
