#pragma once

#include <QScriptable>
#include <QVariant>

class RemoteObjectPrototype : public QObject, public QScriptable
{
	Q_OBJECT
	public:
		RemoteObjectPrototype(const QString& path, QObject* parent = 0);
		~RemoteObjectPrototype();
	public slots:
		void moveMouse(const QVariantMap& parameters);
		void pressMouseButton(const QVariantMap& parameters);
		void releaseMouseButton(const QVariantMap& parameters);
		void mouseWheel(const QVariantMap& parameters);
		void pressKey(const QVariantMap& parameters);
		void releaseKey(const QVariantMap& parameters);
	private:
		const QString m_path;
};
