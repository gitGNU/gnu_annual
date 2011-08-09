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
#include "settingsdialog.h"
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QColor>
#include <QFileDialog>
#include <QFile>
#include <QSettings>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QColorDialog>
#include "card.h"


SettingsDialog::SettingsDialog(QSettings * _settings)
	: settings(_settings)
{
	setWindowTitle(tr("Settings"));
	filename = new QLineEdit;
	filename->setReadOnly(true);
	selectFile = new QPushButton(tr("Select File"));
	warningButton = new QPushButton(tr("Select Color"));
	criticalButton = new QPushButton(tr("Select Color"));
	criticalDays = new QSpinBox;
	warningDays = new QSpinBox;

	criticalDays->setRange(-1, 60);
	warningDays->setRange(-1, 60);

	warningButton->setToolTip(tr("Select the color in which a date is displayed whenever it is characterized as a warning."));
	criticalButton->setToolTip(tr("Select the color in which a date is displayed whenever it is characterized as a critical."));

	criticalDays->setToolTip(tr("Select the dayspan in which a day is considered as critical. The dayspan is the difference in days between today and one date."));
	warningDays->setToolTip(tr("Select the dayspan in which a day is considered as warning. The dayspan is the difference in days between today and one date."));

	selectFile->setToolTip(tr("You can use any database provided by a file which is encoded in the comma separated file format (CSV)."));
	selectFile->setWhatsThis(tr("Select file to use as database"));

	criticalDays->setValue(settings->value("criticalDays").toInt());
	warningDays->setValue(settings->value("warningDays").toInt());

	criticalColor = QColor(settings->value("criticalColor").toString());
	warningColor = QColor(settings->value("warningColor").toString());

	filename->setText(settings->value("file").toString());

	warningButton->setStyleSheet("color: " + warningColor.name() + ";");
	criticalButton->setStyleSheet("color: " + criticalColor.name() + ";");


	QHBoxLayout *dbLayout = new QHBoxLayout();
	dbLayout->addWidget(filename);
	dbLayout->addWidget(selectFile);

	QFormLayout *formLayout = new QFormLayout();
	formLayout->addRow(tr("Filename"), dbLayout);
	formLayout->addRow("Critical color", criticalButton);
	formLayout->addRow("Critical dayspan", criticalDays);
	formLayout->addRow("Warning color", warningButton);
	formLayout->addRow("Warning dayspan", warningDays);


	QDialogButtonBox *buttonbox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Apply);


	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->addLayout(formLayout);
	mainLayout->addWidget(buttonbox);
	setLayout(mainLayout);

	connect(selectFile, SIGNAL(clicked()), this, SLOT(selectFileSlot()));
	connect(warningButton, SIGNAL(clicked()), this, SLOT(warningButtonSlot()));
	connect(criticalButton, SIGNAL(clicked()), this, SLOT(criticalButtonSlot()));


	connect(buttonbox->button(QDialogButtonBox::Apply), SIGNAL(clicked()), this, SLOT(OnApply()));
	connect(buttonbox, SIGNAL(rejected()), this, SLOT(reject()));
	connect(buttonbox, SIGNAL(accepted()), this, SLOT(OnAccepted()));
	connect(buttonbox, SIGNAL(helpRequested()), this, SLOT(OnAccepted()));
}

void SettingsDialog::OnApply()
{
	settings->setValue("file", filename->text());
	settings->setValue("warningColor", warningColor.name());
	settings->setValue("criticalColor", criticalColor.name());
	settings->setValue("criticalDays", criticalDays->value());
	settings->setValue("warningDays", warningDays->value());
}

void SettingsDialog::selectFileSlot()
{
	QString str = QFileDialog::getOpenFileName(this, tr("Open file"), filename->text());
	if(str.isEmpty()) return;
	QFile file(str);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::critical(this, tr("File Error"), tr("Could not open file '%1' for reading").  arg(str));
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
	filename->setText(str);
}

void SettingsDialog::warningButtonSlot()
{
	QColor c = QColorDialog::getColor(warningColor, this, tr("Change color of warning posts"));
	if (!c.isValid()) return;
	warningColor = c;
	warningButton->setStyleSheet("color: " + warningColor.name() + ";");
}

void SettingsDialog::criticalButtonSlot()
{
	QColor c = QColorDialog::getColor(criticalColor, this, tr("Change color of critical posts"));
	if (!c.isValid()) return;
	criticalColor = c;
	criticalButton->setStyleSheet("color: " + criticalColor.name() + ";");

}
