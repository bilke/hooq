#pragma once
/*
	Hooq: Qt4 UI recording, playback, and testing toolkit.
	Copyright (C) 2010  Mendeley Limited <copyright@mendeley.com>

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

#include <QScriptable>
#include <QStringList>

class RemoteApplicationPrototype : public QObject, public QScriptable
{
	Q_OBJECT;
	public:
		RemoteApplicationPrototype(QObject* parent = 0);
		RemoteApplicationPrototype(const QString& path, const QStringList& arguments, QObject* parent = 0);
		~RemoteApplicationPrototype();

		bool useDefaults() const;
		QString path() const;
		QStringList arguments() const;
	public slots:
		void close();
	private:
		QString m_path;
		QStringList m_arguments;
};
