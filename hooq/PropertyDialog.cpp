#include "PropertyDialog.h"

#include "ObjectInformation.h"
#include "PushButtonDelegate.h"
#include "VariantMapModel.h"

#include <QTableView>

PropertyDialog::PropertyDialog(QWidget* parent)
: QDialog(parent)
, m_model(0)
, m_view(new QTableView(this))
{
	PushButtonDelegate* delegate = new PushButtonDelegate(m_view, this);
	delegate->addButton(2, tr("Fetch"));
	delegate->addButton(3, tr("Compare"));
	m_view->setItemDelegate(delegate);
}

void PropertyDialog::show(const ObjectInformation& info)
{
	delete m_model;
	m_model = new VariantMapModel(info.properties(), this);
	m_view->setModel(m_model);
}
