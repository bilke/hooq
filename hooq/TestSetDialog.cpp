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
#include "TestSetDialog.h"

#include <QDir>
#include <QFileDialog>
#include <QStringListModel>

TestSetDialog::TestSetDialog(QWidget* parent)
: QDialog(parent)
, m_model(new QStringListModel(this))
{
	setupUi(this);

	m_argumentsView->setModel(m_model);

	connect(
		m_browseButton,
		SIGNAL(clicked()),
		SLOT(browseForApplication())
	);
	connect(
		m_addArgument,
		SIGNAL(clicked()),
		SLOT(addArgument())
	);
}

void TestSetDialog::addArgument()
{
	m_model->insertRows(m_model->rowCount(), 1);
	m_model->setData(m_model->index(m_model->rowCount() - 1, 0), m_newArgument->text());
	m_newArgument->clear();
}

void TestSetDialog::browseForApplication()
{
	const QString fileName = QFileDialog::getOpenFileName(
		this,
		tr("Application Path")
	);
	if(!fileName.isEmpty())
	{
		m_application->setText(QDir::toNativeSeparators(fileName));
	}
}

QString TestSetDialog::name() const
{
	return m_name->text();
}

void TestSetDialog::setName(const QString& name)
{
	m_name->setText(name);
	Q_ASSERT(name == this->name());
}

QString TestSetDialog::application() const
{
	return QDir::fromNativeSeparators(m_application->text());
}

void TestSetDialog::setApplication(const QString& application)
{
	m_application->setText(QDir::toNativeSeparators(application));
	Q_ASSERT(application == this->application());
}

QStringList TestSetDialog::arguments() const
{
	return m_model->stringList();
}

void TestSetDialog::setArguments(const QStringList& arguments)
{
	m_model->setStringList(arguments);
	Q_ASSERT(arguments == this->arguments());
}
