/* annual - Reminder for annual events
 * Keeps track of all your anniversaries and hopefully reminds you at the right time.
 * Copyright (C) 2011 Dominik Köppl
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "listmodel.h"
#include "tablemodel.h"
#include "ioexception.h"

#include <QRunnable>
#include <QSettings>
#include <QTextStream>
#include <QFile>
#include <QCoreApplication>
#include <cstdlib>

namespace CommandLineColors
{
	static const QString red("\033[1;31m");
	static const QString yellow("\033[1;33m");
	static const QString gray("\033[1;37m");
	static const QString unset("\033[0m");
}

int main_cli()
{
	QSettings settings;
	QTextStream cerr(stderr, QIODevice::WriteOnly);
	QTextStream cout(stdout, QIODevice::WriteOnly);

	QString filename = settings.value("file").toString();
	if (filename.isNull())
		return EXIT_FAILURE;
	int warningDays = settings.value("warningDays").toInt();
	int criticalDays = settings.value("criticalDays").toInt();

	QFile f(settings.value("file").toString());
	if (!f.exists() || !f.open(QIODevice::ReadOnly | QIODevice::Text)
		|| !f.isReadable())
	{
		cerr << QObject::tr("Database ´%1´ not found").arg(filename);
		return EXIT_FAILURE;
	}

	try
	{
		TableModel tablemodel(settings.value("file").toString());
		ListModel listmodel(&tablemodel);


		QStringList arguments = QCoreApplication::arguments();
		if (arguments.indexOf("-q") != -1)
		{
			if (listmodel.rowCount() == 0)
				exit(0);
			if (listmodel.daysLeft(listmodel.index(0, 0)) > warningDays)
				return EXIT_SUCCESS;
		}
		for (int i = 0; i < listmodel.rowCount(); ++i)
		{
			QModelIndex index = listmodel.index(i, 0);
			const int daysleft = listmodel.daysLeft(index);
			if (daysleft > warningDays)
				continue;
			if (daysleft < criticalDays)
				cout << CommandLineColors::red;
			else
				cout << CommandLineColors::yellow;
			cout << listmodel.data(index).toString() << endl;
		}
		cout << CommandLineColors::unset;
		// cout << endl;
	}
	catch(const IOException & e)
	{
		cerr << QObject::tr("A database error occured: ") << e;
		return EXIT_FAILURE;
	}
	qApp->quit();
	return EXIT_SUCCESS;
}
