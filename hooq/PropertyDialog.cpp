#include "PropertyDialog.h"

#include "ObjectInformation.h"
#include "PushButtonDelegate.h"
#include "VariantMapModel.h"

#include <QDebug>

PropertyDialog::PropertyDialog(const ObjectInformation& info, QWidget* parent)
: QDialog(parent)
{
	setupUi(this);

	PushButtonDelegate* delegate = new PushButtonDelegate(m_view, this);
	delegate->addButton(2, tr("Fetch"));
	delegate->addButton(3, tr("Compare"));
	m_view->setItemDelegate(delegate);

	VariantMapModel* model = new VariantMapModel(info.properties(), this);
	m_view->setModel(model);

	m_view->header()->setStretchLastSection(false);
	m_view->header()->setResizeMode(0, QHeaderView::Stretch);
	m_view->header()->setResizeMode(1, QHeaderView::Stretch);
	m_view->header()->setResizeMode(2, QHeaderView::Fixed);
	m_view->header()->setResizeMode(3, QHeaderView::Fixed);

	m_objectLabel->setText(info.name());
	m_typeLabel->setText(info.className());
	m_path = info.path();
}
