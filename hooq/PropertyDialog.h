#pragma once

#include <QDialog>

class ObjectInformation;

class QModelIndex;
class QVariant;

class PropertyDialog : public QDialog
{
	Q_OBJECT
	public:
		PropertyDialog(QWidget* parent);
		show(const ObjectInformation&);
	signals:
		emit fetchRequested(const QString& path, const QString& property);
		emit compareRequested(const QString& path, const QString& property, const QVariant& value);
	private:
		void handleClick(const QModelIndex& index);
};
