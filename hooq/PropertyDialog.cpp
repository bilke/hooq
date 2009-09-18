/*
	Hooq: Qt4 UI recording, playback, and testing toolkit.
	Copyright (C) 2009  Mendeley Limited <copyright@mendeley.com>
	Copyright (C) 2009  Frederick Emmott <mail@fredemmott.co.uk>

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#include "PropertyDialog.h"

#include "ColumnClickMapper.h"
#include "ObjectInformation.h"
#include "PushButtonDelegate.h"
#include "VariantMapModel.h"

#include <QDebug>
#include <QHeaderView>

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

	ColumnClickMapper* mapper = new ColumnClickMapper(m_view);
	mapper->addMapping(2, this, SLOT(fetch(QModelIndex)));
	mapper->addMapping(3, this, SLOT(compare(QModelIndex)));
}

void PropertyDialog::fetch(const QModelIndex& index)
{
	const QString key = index.data(VariantMapModel::PropertyNameRole).toString();
	emit fetchRequested(m_path, key);
	deleteLater();
}

void PropertyDialog::compare(const QModelIndex& index)
{
	const QString key = index.data(VariantMapModel::PropertyNameRole).toString();
	const QVariant value = index.data(VariantMapModel::PropertyValueRole);
	emit compareRequested(m_path, key, value);
	deleteLater();
}
