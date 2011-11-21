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
#ifndef TABWIDGET_H
#define TABWIDGET_H
#include <QWidget>
class QTabBar;
class QStackedLayout;
class QFrame;
class QSettings;
class QStringList;

class TabWidget : public QWidget
{
    Q_OBJECT
private:
    QTabBar* tabbar;
    QStackedLayout* stackedlayout;
    QFrame* frame;
    QSettings* settings;
    QStringList colorlist;
    public:
    TabWidget(QSettings* _settings, QWidget* parent = 0);
    void addTab(QWidget *widget, const QIcon &icon, const QString &label, const QString& settingsname);
    int currentIndex () const
    {
        return currentindex;
    }
protected:
    void resizeEvent ( QResizeEvent * )
    {
        resizeTabbar();
    }
private:
    int currentindex;
    void resizeTabbar();

public slots:
    void setCurrentIndex(int index);
signals:
    void currentChanged(int);
};





#endif // TABWIDGET_H
