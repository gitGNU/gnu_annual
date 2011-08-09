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
#include "aboutdialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QTabWidget>
#include <QTextBrowser>
#include <QFile>
#include <QMessageBox>
#include <QTextCodec>
#include <QTextStream>
#include "config.h"

AboutDialog::AboutDialog()
{
	QVBoxLayout *mainlayout = new QVBoxLayout();

	QLabel *title =
		new QLabel("<h1>" APPLICATION_NAME "</h1><h2>" APPLICATION_VERSION
				   "</h2><h3>" + tr("Using Qt-Version %1").arg(qVersion()) +
				   "</h3>");
	QLabel *icon = new QLabel(this);
	icon->setPixmap(QPixmap(":/icons/program.png"));


	QHBoxLayout *titlelayout = new QHBoxLayout();
	titlelayout->addWidget(icon);
	titlelayout->addWidget(title);

	QTabWidget *tabwidget = new QTabWidget(this);
	tabwidget->addTab(createLabel(":/ABOUT"), tr("About"));
	tabwidget->addTab(createLabel(":/AUTHORS"), tr("Authors"));
	tabwidget->addTab(createLabel(":/THANKS"), tr("Thanks"));
	tabwidget->addTab(createBrowser(":/COPYING"), tr("License"));

	QDialogButtonBox *closeButton =
		new QDialogButtonBox(QDialogButtonBox::Close);
	connect(closeButton, SIGNAL(rejected()), this, SLOT(reject()));
	mainlayout->addLayout(titlelayout);
	mainlayout->addWidget(tabwidget);
	mainlayout->addWidget(closeButton);

	setLayout(mainlayout);
}

QTextBrowser *AboutDialog::createBrowser(QString res)
{
	if (!QFile::exists(res))
	{
		QMessageBox::critical(this, tr("Description error"),
							  tr("Could not open description of %1").arg(res));
		return 0;
	}
	QFile file(res);
	if (!file.open(QFile::ReadOnly))
	{
		QMessageBox::critical(this, tr("Description error"),
							  tr("Could not read description of %1").arg(res));
		return 0;
	}
	QByteArray data = file.readAll();
	QTextCodec *codec = Qt::codecForHtml(data);
	QString text = codec->toUnicode(data);
	QTextBrowser *browser = new QTextBrowser(this);
	browser->setHtml(text);
	return browser;
}

QLabel *AboutDialog::createLabel(QString res)
{
	if (!QFile::exists(res))
	{
		QMessageBox::critical(this, tr("Description error"),
							  tr("Could not open description of %1").arg(res));
		return 0;
	}
	QFile file(res);
	if (!file.open(QFile::ReadOnly))
	{
		QMessageBox::critical(this, tr("Description error"),
							  tr("Could not read description of %1").arg(res));
		return 0;
	}
	QTextStream stream(&file);
	return new QLabel(stream.readAll(), this);
}
