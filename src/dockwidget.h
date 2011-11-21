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
#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H
#include <QDockWidget>
class QSettings;

class DockWidget : public QDockWidget
{
    Q_OBJECT
    QSettings* settings;
    QString title;
public:
    DockWidget(QSettings* _settings, const QString & _title, QWidget * parent = 0, Qt::WindowFlags flags = 0 );
protected:
    void closeEvent ( QCloseEvent* );
signals:
    void windowClosed();
};

#endif // DOCKWIDGET_H
