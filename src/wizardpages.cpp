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
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QByteArray>
#include <QTextStream>
#include <QTextCodec>
#include <QGroupBox>
#include <QRadioButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "wizardpages.h"
#include "card.h"
#include "config.h"

IntroductionPage::IntroductionPage(QWidget* parent)
	: QWizardPage(parent)
{
	setTitle(tr("Setting up %1").arg(APPLICATION_NAME));

	QLabel *label = new QLabel(tr("This wizard helps you to setup %1. Please follow the next pages to successfully setup %1.").arg(APPLICATION_NAME));
	label->setWordWrap(true);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(label);
	setLayout(layout);
}

LicensePage::LicensePage(QWidget* parent) 
	: QWizardPage(parent), textbrowser(new QTextBrowser(this))
{
	setTitle(tr("Accept License"));
	setSubTitle(tr("Please read the following license and apply to it to proceed."));


	QString res(":/COPYING");

	if (!QFile::exists(res))
		return;
	QFile file(res);
	if (!file.open(QFile::ReadOnly))
		return;

	QByteArray data = file.readAll();
	QTextCodec *codec = Qt::codecForHtml(data);
	QString str = codec->toUnicode(data);

	textbrowser->setHtml(str);

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(textbrowser);

	QGroupBox *groupbox = new QGroupBox(tr("Do you apply to the license?"));
	QRadioButton *accept = new QRadioButton(tr("I comply with the license"));
	QRadioButton *reject = new QRadioButton(tr("I refuse the license"));
	reject->setChecked(true);
	QVBoxLayout *grouplayout = new QVBoxLayout();
	grouplayout->addWidget(accept);
	grouplayout->addWidget(reject);
	groupbox->setLayout(grouplayout);
	layout->addWidget(groupbox);
	setLayout(layout);
	registerField("accept*", accept);
}



WizardFilePage::WizardFilePage(QWidget* parent)
	: QWizardPage(parent)
{
	QHBoxLayout *layout = new QHBoxLayout();
	QVBoxLayout *vlayout = new QVBoxLayout();
	fileNameEdit = new QLineEdit(this);
	fileNameEdit->setReadOnly(true);
	buttonExistingFile = new QPushButton(tr("Load Database"), this);
	buttonNewFile = new QPushButton(tr("Create Database"), this);
	setTitle(tr("Load database"));
	setSubTitle(tr("Specify filename of database to load"));
	registerField("filename*", fileNameEdit);
	registerField("buttonExist", buttonExistingFile);
	registerField("buttonNew", buttonNewFile);
	connect(buttonExistingFile, SIGNAL(clicked()), this,
			SLOT(existingClicked()));
	connect(buttonNewFile, SIGNAL(clicked()), this, SLOT(newClicked()));
	layout->addWidget(fileNameEdit);
	layout->addWidget(buttonExistingFile);
	vlayout->addLayout(layout);
	vlayout->addWidget(buttonNewFile);

	setLayout(vlayout);
}

void WizardFilePage::newClicked()
{
	QString str = QFileDialog::getSaveFileName(this, tr("Open file"), _filename);
	if (str.isEmpty()) return;
	QFile file(str);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QMessageBox::critical(this, tr("File Error"), tr("Could not open file '%1' for writing.").arg(str));
		return;
	}
	QTextStream stream(&file);
	stream << "#Type;Date;Name;Comment" << endl;
	file.close();
	fileNameEdit->setText(str);
	_filename = str;

}

void WizardFilePage::existingClicked()
{
	QString str = QFileDialog::getOpenFileName(this, tr("Open file"), _filename);
	if (str.isEmpty()) return;
	QFile file(str);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::critical(this, tr("File Error"), tr("Could not open file '%1' for reading").arg(str));
		return;
	}
	try
	{
		readCards(&file);
	}
	catch(const IOException & e)
	{
		QMessageBox::critical(this, tr("Error while reading file %1").arg(str), e);
		return;
	}

	file.close();
	fileNameEdit->setText(str);
	_filename = str;

}
