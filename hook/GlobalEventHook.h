#pragma once

#include <QList>
#include <QPair>
#include <QString>

class QKeyEvent;
class QMouseEvent;

class GlobalEventHook
{
	public:
		/// Install the hook.
		static void activate();
	private:
		/// The main hook.
		static bool hook(void** data);

		/// Print out an event.
		static void outputEvent(
			const QString& object,
			const char* action,
			const QList<QPair<QString, QString> >& data
		);

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
		static QList<QPair<QString, QString> > formattedKeyEvent(QKeyEvent* event);
		/// Return a list of parameters for a mouse press event.
		static QList<QPair<QString, QString> > formattedMouseEvent(QMouseEvent* event);
};
