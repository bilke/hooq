#pragma once

#include <QObject>
#include <QPointer>
#include <QString>
#include <QTime>
#include <QXmlStreamWriter>

class QKeyEvent;
class QMouseEvent;
class QWheelEvent;

namespace Hooq
{

class Logger : public QObject
{
	Q_OBJECT
	public:
		~Logger();
		static Logger* instance(QIODevice*);
	signals:
		void finished();
	private slots:
		void readInput();
	private:
		static Logger* instance();
		Logger(QIODevice* device);
		static QPointer<Logger> m_instance;
		static void activate();
		static void deactivate();

		/// The main hook.
		static bool hook(void** data);
		void hook(QObject* receiver, QEvent* event);

		QXmlStreamWriter m_writer;
		QTime m_timer;

		void outputEvent(QObject* receiver, const char* event, const QXmlStreamAttributes& attributes);

		/** Get a name for an object.
		 * This will be, in order of preference:
		 * - the objects' name, if it has one
		 * - the classname + ":n", where 'n' is the position within the children of the same class with the same parent
		 * - the classname + ":0" if the object has no parent
		 */
		static QString objectName(QObject* object);

		/** Get a dotted (DNS-style) object path.
		 * @see objectName
		 */
		static QString objectPath(QObject* object);

		/// Return a list of parameters for a key event.
		static QXmlStreamAttributes keyEventAttributes(QKeyEvent* event);
		/// Return a list of parameters for a mouse press/release/move event.
		static QXmlStreamAttributes mouseEventAttributes(QMouseEvent* event);
		/// Return a list of parameters for a moue wheel event
		static QXmlStreamAttributes wheelEventAttributes(QWheelEvent* event);

};

} // namespace
