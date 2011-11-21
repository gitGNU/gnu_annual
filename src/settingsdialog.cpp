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
#include <QCheckBox>
#include <QRadioButton>
#include <QGroupBox>
#include <QApplication>
SettingsDialog::SettingsDialog(QSettings * _settings)
	: settings(_settings)
{
	setWindowTitle(tr("Settings"));
	filename = new QLineEdit;
	filename->setReadOnly(true);
	selectFile = new QPushButton(tr("Select File"));
	warningButton = new QPushButton(tr("Select Color"));
	criticalButton = new QPushButton(tr("Select Color"));
        colorEditButton = new QPushButton(tr("Select foreground of edit tab"));
        colorViewButton = new QPushButton(tr("Select foreground of view tab"));
	criticalDays = new QSpinBox;
        warningDays = new QSpinBox;
        showInserter = new QCheckBox(tr("Inserter Dockwidget"));
        showToolbar = new QCheckBox(tr("Tool-Bar"));
        showTabs = new QCheckBox(tr("Show Tabs?"));
        hasWindowBorder = new QCheckBox(tr("Has Border?"));
        setViewAsDefault = new QRadioButton(tr("View"));
        setEditAsDefault = new QRadioButton(tr("Edit"));


	criticalDays->setRange(-1, 60);
	warningDays->setRange(-1, 60);

        QPalette colorButtonPalette = QApplication::palette();
        colorButtonPalette.setColor(QPalette::Button, colorButtonPalette.color(QPalette::Window));
        warningButton->setPalette(colorButtonPalette);
        criticalButton->setPalette(colorButtonPalette);

	warningButton->setToolTip(tr("Select the color of dates which are displayed as warnings."));    
	criticalButton->setToolTip(tr("Select the color of dates which are displayed as critical."));
        colorEditButton->setToolTip(tr("Select the color of the foreground of the edit-tab"));
        colorViewButton->setToolTip(tr("Select the color of the foreground of the view-tab"));

	criticalDays->setToolTip(tr("Select the dayspan in which a day is considered as critical. The dayspan is the difference in days between today and one date."));
	warningDays->setToolTip(tr("Select the dayspan in which a day is considered as warning. The dayspan is the difference in days between today and one date."));


	selectFile->setToolTip(tr("You can use any database provided by a file which is encoded in the 'comma-separated file format' (CSV)."));
	selectFile->setWhatsThis(tr("Select file to use as database"));

	criticalDays->setValue(settings->value("criticalDays").toInt());
	warningDays->setValue(settings->value("warningDays").toInt());

	criticalColor = QColor(settings->value("criticalColor").toString());
	warningColor = QColor(settings->value("warningColor").toString());
        viewColor = QColor(settings->value("viewColor").toString());
        editColor = QColor(settings->value("editColor").toString());

	filename->setText(settings->value("file").toString());
        hasWindowBorder->setChecked(settings->value("hasWindowBorder", false).toBool());
        showInserter->setChecked(settings->value("showInserter", true).toBool());
        showToolbar->setChecked(settings->value("showToolbar", true).toBool());
        showTabs->setChecked(settings->value("showTabs", true).toBool());
        int defaultTab = settings->value("defaultTab").toInt();
        if(defaultTab == 0) setViewAsDefault->setChecked(true);
        else setEditAsDefault->setChecked(true);

	warningButton->setStyleSheet("color: " + warningColor.name() + ";");
	criticalButton->setStyleSheet("color: " + criticalColor.name() + ";");
        colorViewButton->setStyleSheet("color: " + viewColor.name() + ";");
        colorEditButton->setStyleSheet("color: " + editColor.name() + ";");



	QHBoxLayout *dbLayout = new QHBoxLayout();
	dbLayout->addWidget(filename);
	dbLayout->addWidget(selectFile);

	QFormLayout *formLayout = new QFormLayout();
	formLayout->addRow(tr("Filename"), dbLayout);
        formLayout->addRow(tr("Critical color"), criticalButton);
        formLayout->addRow(tr("Critical dayspan"), criticalDays);
        formLayout->addRow(tr("Warning color"), warningButton);
        formLayout->addRow(tr("Warning dayspan"), warningDays);
        formLayout->addRow(tr("Edit color"), colorEditButton);
        formLayout->addRow(tr("View color"), colorViewButton);



        QGroupBox* tabPopUpGroup = new QGroupBox(tr("Automatic Pop-Ups for"));
        QHBoxLayout* tabPopUpLayout = new QHBoxLayout;
        tabPopUpLayout->addWidget(showInserter);
        tabPopUpLayout->addWidget(showToolbar);
        tabPopUpGroup->setLayout(tabPopUpLayout);

        QHBoxLayout* checkboxLayout = new QHBoxLayout;
        checkboxLayout->addWidget(showTabs);
        checkboxLayout->addWidget(hasWindowBorder);


        QGroupBox* tabDefaultGroup = new QGroupBox(tr("Set tab on start-up"));
        QHBoxLayout* tabDefaultLayout = new QHBoxLayout;
        tabDefaultLayout->addWidget(setViewAsDefault);
        tabDefaultLayout->addWidget(setEditAsDefault);
        tabDefaultGroup->setLayout(tabDefaultLayout);

        QDialogButtonBox *buttonbox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Apply);

	QVBoxLayout *mainLayout = new QVBoxLayout();
        mainLayout->addLayout(formLayout);
        mainLayout->addLayout(checkboxLayout);
        mainLayout->addWidget(tabDefaultGroup);
        mainLayout->addWidget(tabPopUpGroup);
	mainLayout->addWidget(buttonbox);
	setLayout(mainLayout);

	connect(selectFile, SIGNAL(clicked()), this, SLOT(selectFileSlot()));
	connect(warningButton, SIGNAL(clicked()), this, SLOT(warningButtonSlot()));
	connect(criticalButton, SIGNAL(clicked()), this, SLOT(criticalButtonSlot()));

        connect(colorEditButton, SIGNAL(clicked()), this, SLOT(OnColorEditButton()));
        connect(colorViewButton, SIGNAL(clicked()), this, SLOT(OnColorViewButton()));

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
        settings->setValue("viewColor", viewColor.name());
        settings->setValue("editColor", editColor.name());
        settings->setValue("hasWindowBorder", hasWindowBorder->checkState() == Qt::Checked);
        settings->setValue("showInserter", showInserter->checkState() == Qt::Checked);
        settings->setValue("showToolbar", showToolbar->checkState() == Qt::Checked);
        settings->setValue("defaultTab", setViewAsDefault->isChecked() ? 0 : 1);
        settings->setValue("showTabs", showTabs->checkState() == Qt::Checked);
}

void SettingsDialog::selectFileSlot()
{
	QString str = QFileDialog::getOpenFileName(this, tr("Open file"), filename->text());
	if(str.isEmpty()) return;
	QFile file(str);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::critical(this, tr("File Error"), tr("Could not read file '%1'.").  arg(str));
		return;
	}
	try
	{
		readCards(&file);
	}
	catch(const IOException & e)
	{
		QMessageBox::critical(this, tr("Error while reading file '%1'.").arg(str), e);
		return;
	}

	file.close();
	filename->setText(str);
}

void SettingsDialog::warningButtonSlot()
{
	QColor c = QColorDialog::getColor(warningColor, this, tr("Change color of warning entries"));
	if (!c.isValid()) return;
	warningColor = c;
	warningButton->setStyleSheet("color: " + warningColor.name() + ";");
}

void SettingsDialog::criticalButtonSlot()
{
	QColor c = QColorDialog::getColor(criticalColor, this, tr("Change color of critical entries"));
	if (!c.isValid()) return;
	criticalColor = c;
	criticalButton->setStyleSheet("color: " + criticalColor.name() + ";");

}
void SettingsDialog::OnColorEditButton()
{
        QColor c = QColorDialog::getColor(criticalColor, this, tr("Change color of edit tab"));
        if (!c.isValid()) return;
        editColor = c;
        colorEditButton->setStyleSheet("color: " + editColor.name() + ";");
}
void SettingsDialog::OnColorViewButton()
{
        QColor c = QColorDialog::getColor(criticalColor, this, tr("Change color of view tab"));
        if (!c.isValid()) return;
        viewColor = c;
        colorViewButton->setStyleSheet("color: " + viewColor.name() + ";");
}
