#pragma once

#include <QScriptable>
#include <QVariant>

class RemoteObjectPrototype : public QObject, public QScriptable
{
	Q_OBJECT
	public:
		RemoteObjectPrototype(const QString& path, QObject* parent = 0);
		~RemoteObjectPrototype();
		
		QString path() const;
	public slots:
		void moveMouse(const QVariantMap& parameters);
		void pressMouseButton(const QVariantMap& parameters);
		void releaseMouseButton(const QVariantMap& parameters);
		void mouseWheel(const QVariantMap& parameters);
		void pressKey(const QVariantMap& parameters);
		void releaseKey(const QVariantMap& parameters);
		void shortcut(const QVariantMap& parameters);
	signals:
		void mouseMoveEvent(const QString& path, const QPoint& position, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers);
		void mousePressEvent(const QString& path, const QPoint& position, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers);
		void mouseReleaseEvent(const QString& path, const QPoint& position, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers);
		void wheelEvent(const QString& path, const QPoint& position, int delta, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers, Qt::Orientation orientation);
		void keyPressEvent(const QString& path, int key, Qt::KeyboardModifiers modifiers, const QString& text, bool autorepeat, ushort count);
		void keyReleaseEvent(const QString& path, int key, Qt::KeyboardModifiers modifiers, const QString& text, bool autorepeat, ushort count);
	private:
		const QString m_path;
};
