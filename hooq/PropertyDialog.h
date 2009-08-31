#pragma once

#include <QDialog>

class ObjectInformation;
class VariantMapModel;

class QAbstractItemView;
class QModelIndex;
class QVariant;

class PropertyDialog : public QDialog
{
	Q_OBJECT
	public:
		PropertyDialog(QWidget* parent);
		void show(const ObjectInformation&);
	signals:
		void fetchRequested(const QString& path, const QString& property);
		void compareRequested(const QString& path, const QString& property, const QVariant& value);
	private:
		void handleClick(const QModelIndex& index);

		VariantMapModel* m_model;
		QAbstractItemView* m_view;
};
