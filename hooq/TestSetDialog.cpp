#include "TestSetDialog.h"

#include <QDir>
#include <QFileDialog>
#include <QStringListModel>

TestSetDialog::TestSetDialog(QWidget* parent)
: QDialog(parent)
, m_model(new QStringListModel(this))
, m_readOnly(false)
{
	setupUi(this);

	connect(
		m_browseButton,
		SIGNAL(clicked()),
		SLOT(browseForApplication())
	);
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

bool TestSetDialog::isReadOnly() const
{
	return m_readOnly;
}

void TestSetDialog::setReadOnly(bool readOnly)
{
	if(readOnly == isReadOnly())
	{
		return;
	}
	m_readOnly = readOnly;

	m_name->setReadOnly(readOnly);
	m_application->setReadOnly(readOnly);
	m_browseButton->setVisible(!readOnly);
	m_newArgument->setVisible(!readOnly);
	m_addArgument->setVisible(!readOnly);
}

QString TestSetDialog::name() const
{
	return m_name->text();
}

QString TestSetDialog::application() const
{
	return QDir::fromNativeSeparators(m_application->text());
}

QStringList TestSetDialog::arguments() const
{
	return m_model->stringList();
}
