#include "Logger.h"

#include "../common/Communication.h"
#include "../common/ObjectHookName.h"

#include <Qt>

#include <QCoreApplication>
#include <QDebug>
#include <QEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QObject>
#include <QTextStream>
#include <QTime>

namespace Hooq
{
QPointer<Logger> Logger::m_instance;

Logger* Logger::instance(QIODevice* device)
{
	m_instance = QPointer<Logger>(new Logger(device));
	return m_instance.data();
}

Logger* Logger::instance()
{
	return m_instance.data();
}

Logger::Logger(QIODevice* device)
{
	m_writer.setDevice(device);
	m_writer.setAutoFormatting(true);
	m_writer.writeStartDocument();
	m_writer.writeStartElement("hooq");
	m_timer.start();
	activate();
	connect(
		device,
		SIGNAL(readyRead()),
		SLOT(readInput())
	);
}

void Logger::readInput()
{
	if(m_writer.device()->readLine().constData() == QLatin1String("STOP"))
	{
		emit finished();
		deleteLater();
	}
}

Logger::~Logger()
{
	m_writer.writeEndDocument();
}

void Logger::activate()
{
	qAddPostRoutine(deactivate);
	// Internal API... seems unavoidable
	QInternal::registerCallback(QInternal::EventNotifyCallback, &hook);
}

void Logger::deactivate()
{
	delete instance();
	QInternal::unregisterCallback(QInternal::EventNotifyCallback, &hook);
}

bool Logger::hook(void** data)
{
	QObject* receiver = reinterpret_cast<QObject*>(data[0]);
	QEvent* event = reinterpret_cast<QEvent*>(data[1]);
	// returnType* returnValue = reinterpret_cast<returnType*>(data[2])
	instance()->hook(receiver, event);
	return false;
}

void Logger::hook(QObject* receiver, QEvent* event)
{
	switch(event->type())
	{
		case QEvent::KeyPress:
			outputEvent(receiver, "keyPress", keyEventAttributes(static_cast<QKeyEvent*>(event)));
			break;
		case QEvent::KeyRelease:
			outputEvent(receiver, "keyRelease", keyEventAttributes(static_cast<QKeyEvent*>(event)));
			break;
		case QEvent::MouseMove:
			outputEvent(receiver, "mouseMove", mouseEventAttributes(static_cast<QMouseEvent*>(event)));
			break;
		case QEvent::MouseButtonPress:
			outputEvent(receiver, "mouseButtonPress", mouseEventAttributes(static_cast<QMouseEvent*>(event)));
			break;
		case QEvent::MouseButtonRelease:
			outputEvent(receiver, "mouseButtonRelease", mouseEventAttributes(static_cast<QMouseEvent*>(event)));
			break;
		case QEvent::MouseButtonDblClick:
			outputEvent(receiver, "mouseButtonDoubleClick", mouseEventAttributes(static_cast<QMouseEvent*>(event)));
			break;
		default:
			break;

	}
}

void Logger::outputEvent(QObject* receiver, const char* event, const QXmlStreamAttributes& attributes)
{
	m_writer.writeTextElement("msec", QString::number(m_timer.restart()));
	
	m_writer.writeStartElement(event);
	m_writer.writeAttributes(attributes);
	m_writer.writeAttribute("target", ObjectHookName::objectPath(receiver));
	m_writer.writeEndElement(); //event;
}

QXmlStreamAttributes Logger::keyEventAttributes(QKeyEvent* event)
{
	QXmlStreamAttributes data;
	data.append("key", QString::number(event->key())); // Qt::Key
	data.append("modifiers", QString::number(event->modifiers())); // Qt::Modifiers
	data.append("text", event->text());
	data.append("isAutoRepeat", event->isAutoRepeat() ? "true" : "false");
	data.append("count", QString::number(event->count()));
	return data;
}

QXmlStreamAttributes Logger::mouseEventAttributes(QMouseEvent* event)
{
	QXmlStreamAttributes data;
	data.append("x", QString::number(event->x()));
	data.append("y", QString::number(event->y()));
	data.append("button", QString::number(event->button()));
	data.append("buttons", QString::number(event->buttons()));
	return data;
}

} //namespace
