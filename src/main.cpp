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

#include <QtGui/QApplication>
#include <QTextStream>

#include "mainwindow.h"
#include "config.h"

int main_cli();

#include <QTranslator>
#include <QLibraryInfo>

static void setEnv()
{
	QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
	QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);
	QCoreApplication::setApplicationName(APPLICATION_NAME);

	QTranslator qttranslator;
	qttranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	QCoreApplication::instance()->installTranslator(&qttranslator);
	QTranslator translator;
	translator.load(APPLICATION_NAME "_" + QLocale::system().name());
	QCoreApplication::instance()->installTranslator(&translator);
}

static void showHelp()
{
	QTextStream cout(stdout, QIODevice::WriteOnly);
	cout << APPLICATION_NAME << " - " << APPLICATION_VERSION << endl
		<< QObject::tr("Arguments: ") << endl
		<< "  -h\t\t" << QObject::
		tr("Print Help (this message) and exit") << endl << "  -v\t\t" <<
		QObject::
		tr("Print version information and exit") << endl << "  -l\t\t" <<
		QObject::
		tr("Do not use graphical interface - just output to console") << endl
		<< "  -q\t\t" << QObject::
		tr("Don't popup a window if there's nothing to remind") << endl;
}

int main(int argc, char *argv[])	// TODO: -v -> version!
{
	for (int i = 1; i < argc; ++i)
	{
		if (strcmp("-q", argv[i]) == 0)
			continue;
		if (strcmp("-l", argv[i]) == 0)
		{
			QCoreApplication app(argc, argv);
			setEnv();
			return main_cli();
		}
		if (strcmp("-h", argv[i]) == 0 || strcmp("--help", argv[i]) == 0)
		{
			showHelp();
			return EXIT_SUCCESS;
		}
		if (strcmp("-v", argv[i]) == 0 || strcmp("--version", argv[i]) == 0)
		{
			QTextStream cout(stdout, QIODevice::WriteOnly);
			cout << APPLICATION_NAME << " - " << APPLICATION_VERSION << endl
				<< "Copyright (C) 2011 Dominik Koeppl" << endl
				<<
				"License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>"
				<< endl <<
				"This is free software: you are free to change and redistribute it."
				<< endl <<
				"There is NO WARRANTY, to the extent permitted by law." <<
				endl;
			return EXIT_SUCCESS;
		}

		QTextStream cerr(stderr, QIODevice::WriteOnly);
		cerr << QObject::tr("Unknown argument: %1").arg(argv[i]) << endl;
		showHelp();
		return EXIT_FAILURE;
	}
	QApplication app(argc, argv);
	app.setWindowIcon(QIcon(":/icons/program.png"));
	setEnv();
	try
	{
		MainWindow w;
		w.show();
		return app.exec();
	}
	catch(int)
	{
		return EXIT_FAILURE;
	}

}
