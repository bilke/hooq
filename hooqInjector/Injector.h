#pragma once

#include <QObject>
#include <QString>

namespace Hooq
{

/// Abstract class for platform-specific DLL injectors.
class Injector : public QObject
{
	Q_OBJECT
	public:
		Injector(QObject* parent = 0);
		virtual ~Injector();

		virtual void startAndAttach(const QString& application) = 0;
		virtual void attach(int processId) = 0;
};

} // namespace
