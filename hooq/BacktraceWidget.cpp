#include "BacktraceWidget.h"

#include "ScriptBacktraceModel.h"

#include <QTableView>
#include <QVBoxLayout>

BacktraceWidget::BacktraceWidget(QScriptContext* context, QWidget* parent)
: QWidget(parent)
{
	QVBoxLayout* layout = new QVBoxLayout(this);
	QTableView* view = new QTableView(this);
	layout->addWidget(view);

	view->setHorizontalHeader(false);
	view->setModel(new ScriptBacktraceModel(context, this));
}
