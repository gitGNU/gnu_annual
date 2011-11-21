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
#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H
#include <QDialog>
#include <QColor>

class QSpinBox;
class QLineEdit;
class QPushButton;
class QColor;
class QSettings;
class QCheckBox;
class QRadioButton;

class SettingsDialog : public QDialog
{
  Q_OBJECT 
  public:
	SettingsDialog(QSettings * _settings);
  private:
	QSpinBox * criticalDays;
	QSpinBox *warningDays;

	QLineEdit *filename;
	QPushButton *selectFile;
	QPushButton *warningButton;
	QPushButton *criticalButton;
        QPushButton *colorEditButton;
        QPushButton *colorViewButton;

	QColor criticalColor;
	QColor warningColor;
        QColor editColor;
        QColor viewColor;
	QSettings *settings;

        QCheckBox* hasWindowBorder;
    QCheckBox* showInserter;
    QCheckBox* showToolbar;
 QCheckBox* showTabs;

    QRadioButton* setViewAsDefault;
    QRadioButton* setEditAsDefault;


  private slots:
	void OnAccepted()
	{
		OnApply();
		accept();
	}
	void OnApply();
	void selectFileSlot();
	void warningButtonSlot();
	void criticalButtonSlot();
        void OnColorViewButton();
        void OnColorEditButton();
};
#endif // SETTINGSDIALOG_H

