#include "Logger.h"

#include "../lib/Communication.h"
#include "../lib/ObjectHookName.h"

#include <Qt>

#include <QCoreApplication>
#include <QDebug>
#include <QEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QLocalSocket>
#include <QObject>
#include <QTextStream>
#include <QTime>

namespace Hooq
{

QXmlStreamWriter Logger::m_writer;

QTime Logger::m_timer;

void Logger::activate()
{

	delete(m_writer.device());
	QLocalSocket* socket = new QLocalSocket();
	socket->connectToServer(Communication::serverName());
	//qDebug() << Q_FUNC_INFO << "Socket name:" << Communication::serverName();
	socket->waitForConnected(1000);
	Q_ASSERT(socket->state() == QLocalSocket::ConnectedState);
	m_writer.setDevice(socket);

	qAddPostRoutine(deactivate);
	m_writer.setAutoFormatting(true);
	m_writer.writeStartDocument();
	m_writer.writeStartElement("hooq");

	// Internal API... seems unavoidable
	QInternal::registerCallback(QInternal::EventNotifyCallback, &hook);

	m_timer.start();
}

void Logger::deactivate()
{
	m_writer.writeEndDocument();
	qobject_cast<QLocalSocket*>(m_writer.device())->flush();
	m_writer.device()->close();
	delete m_writer.device();
}

bool Logger::hook(void** data)
{
	QObject* receiver = reinterpret_cast<QObject*>(data[0]);
	QEvent* event = reinterpret_cast<QEvent*>(data[1]);
	// returnType* returnValue = reinterpret_cast<returnType*>(data[2])
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
	return false;
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
