#pragma once

#include <QWidget>

class QScriptContext;

class BacktraceWidget : public QWidget
{
	Q_OBJECT;
	public:
		BacktraceWidget(QScriptContext* context, QWidget* parent);
};
