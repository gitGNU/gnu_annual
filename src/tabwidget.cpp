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
#include <QStackedLayout>
#include <QTabBar>
#include <QFrame>
#include <QResizeEvent>
#include <QSettings>
#include "tabwidget.h"

TabWidget::TabWidget(QSettings* _settings, QWidget* parent)
    : QWidget(parent), settings(_settings)
{
    frame = new QFrame(this);

    if(settings->value("hasWindowBorder").toBool() == true)
    {
        frame->setFrameStyle(QFrame::Box);
        frame->setLineWidth(2);
    }

    tabbar = new QTabBar(this);
    tabbar->setExpanding(true);
    tabbar->setStyleSheet("font: bold ");
    if(settings->value("showTabs", true).toBool() == false)
        tabbar->setVisible(false);

    QVBoxLayout* layout = new QVBoxLayout(this);
    setLayout(layout);
    stackedlayout = new QStackedLayout(frame);
    layout->addWidget(tabbar);
    layout->addWidget(frame);
    connect(tabbar, SIGNAL(currentChanged(int)), this, SLOT(setCurrentIndex(int)));

    resizeTabbar();

}

void TabWidget::addTab(QWidget *widget, const QIcon &icon, const QString &label, const QString& settingsname)
{
    tabbar->addTab(icon, label);
    stackedlayout->addWidget(widget);
    colorlist.append(settingsname);
    setCurrentIndex(0);
}

void TabWidget::setCurrentIndex(int index)
{
    if(currentindex < colorlist.size())
        tabbar->setTabTextColor(currentindex, QColor());
    if(index < colorlist.size() && settings->value(colorlist[index]).isValid())
    {
        QColor color = (settings->value(colorlist[index]).toString());
        if(color.isValid())
            tabbar->setTabTextColor(index, color);
    }


    currentindex = index;
    stackedlayout->setCurrentIndex(index);
    tabbar->setCurrentIndex(index);
    QPalette palette;
    palette.setColor(QPalette::Foreground, tabbar->tabTextColor(index));
    frame->setPalette(palette);

    resizeTabbar();

    emit currentChanged(index);
}
void TabWidget::resizeTabbar()
{
    QRect r = tabbar->geometry();
    r.setWidth(width()*1.5);
    tabbar->setGeometry(r);
}
