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
		enum Action
		{
			Record,
			Replay
		};

		Injector(QObject* parent = 0);
		virtual ~Injector();

		virtual void startAndAttach(const QString& application, Action action) = 0;
		virtual void attach(int processId, Action action) = 0;
};

} // namespace
