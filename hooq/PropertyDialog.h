#pragma once

#include "ui_PropertyDialog.h"

#include <QDialog>

class ObjectInformation;
class VariantMapModel;

class QTreeView;
class QModelIndex;
class QVariant;

class PropertyDialog : public QDialog, private Ui::PropertyDialog
{
	Q_OBJECT
	public:
		PropertyDialog(const ObjectInformation&, QWidget* parent);
	signals:
		void fetchRequested(const QString& path, const QString& property);
		void compareRequested(const QString& path, const QString& property, const QVariant& value);
	private:
		void handleClick(const QModelIndex& index);

		QString m_path;
};
