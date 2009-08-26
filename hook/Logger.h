#pragma once

#include <QString>
#include <QTime>
#include <QXmlStreamWriter>

class QKeyEvent;
class QMouseEvent;

namespace Hooq
{

class Logger
{
	public:
		/// Install the hook.
		static void activate();
		static void deactivate();
	private:
		static QXmlStreamWriter m_writer;
		static QTime m_timer;
		/// The main hook.
		static bool hook(void** data);

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

		static void outputEvent(QObject* receiver, const char* event, const QXmlStreamAttributes& attributes);

		/// Return a list of parameters for a key event.
		static QXmlStreamAttributes keyEventAttributes(QKeyEvent* event);
		/// Return a list of parameters for a mouse press event.
		static QXmlStreamAttributes mouseEventAttributes(QMouseEvent* event);
};

} // namespace
