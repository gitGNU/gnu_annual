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
#include <QPainter>
#include <QSettings>
#include "listmodel.h"
#include "listdelegate.h"

ListDelegate::ListDelegate(QSettings * _settings, QObject * parent)
  	: QItemDelegate(parent), settings(_settings)
	{}


void ListDelegate::paint(QPainter * painter,
						 const QStyleOptionViewItem & option,
						 const QModelIndex & index) const
{
	const ListModel *model = static_cast < const ListModel * >(index.model());
	if (!model)
		return;


	int criticalDays = settings->value("criticalDays").toInt();
	int warningDays = settings->value("warningDays").toInt();

	QColor criticalColor = QColor(settings->value("criticalColor").toString());
	QColor warningColor = QColor(settings->value("warningColor").toString());
	QStyleOptionViewItem o = option;
	if (model->daysLeft(index) < criticalDays)
		o.palette.setColor(QPalette::Text, criticalColor);
	else if (model->daysLeft(index) < warningDays)
		o.palette.setColor(QPalette::Text, warningColor);
	QItemDelegate::paint(painter, o, index);
}
