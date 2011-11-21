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
#include "listmodel.h"
#include "tablemodel.h"

int SortableRow::value() const
{
	return _model->data(_model->index(_row, TableModel::COLUMN_DAYSLEFT)).toInt();
}

bool operator<(const SortableRow & a, const SortableRow & b)
{
	return a.value() < b.value();
}

ListModel::ListModel(TableModel * _model, QObject * parent):QAbstractListModel(parent), model(_model)
											// , settings(_settings)
{
	connect(model, SIGNAL(dataChanged(QModelIndex, QModelIndex)), this,
			SLOT(dataChange(QModelIndex, QModelIndex)));
	connect(model, SIGNAL(rowsRemoved(QModelIndex, int, int)), this,
			SLOT(onRowsRemoved(QModelIndex, int, int)));
	connect(model, SIGNAL(rowsInserted(QModelIndex, int, int)), this,
			SLOT(rowsInsert(QModelIndex, int, int)));
	for (int i = 0; i < model->rowCount() - 1; ++i)
	{
		permut.push_back(SortableRow(model, i));
	}
	qSort(permut.begin(), permut.end());

}

void ListModel::rowsInsert(const QModelIndex & parent, int start, int end)
{
	Q_UNUSED(parent);
	Q_UNUSED(start);
	Q_UNUSED(end);
	beginResetModel();
	for (int i = 0; i < end - start + 1; ++i)
	{
		permut.push_back(SortableRow(model, permut.size()));
	}
	qSort(permut.begin(), permut.end());
	endResetModel();
}

void ListModel::onRowsRemoved(const QModelIndex & parent, int start, int end)
{
	Q_UNUSED(parent);
	Q_UNUSED(start);
	Q_UNUSED(end);
	beginResetModel();
	permut.clear();
	for (int i = 0; i < model->rowCount() - 1; ++i)
	{
		permut.push_back(SortableRow(model, i));
	}
	qSort(permut.begin(), permut.end());
	endResetModel();
}

void ListModel::dataChange(const QModelIndex & topLeft,
						   const QModelIndex & bottomRight)
{
	Q_UNUSED(topLeft);
	Q_UNUSED(bottomRight);
	beginResetModel();
	qSort(permut.begin(), permut.end());
	endResetModel();
}

int ListModel::daysLeft(const QModelIndex & index) const 
{
	return permut.at(index.row()).value();
}

static QString formatComment(Anniv::Type type, int daysleft)
{
	/* %n daysleft
	 * %1 Name
	 * %2 Date
	 * %3 optional: yeardiff
	 */

	switch (type)
	{
	case Anniv::ANNIVERSARY:
		return daysleft == 0 ? ListModel::tr("Today is %1, exactly at %2.")
			: ListModel::tr("In %n day(s) is %1, exactly at %2.", "", daysleft);
	case Anniv::BIRTHDAY:
		return daysleft == 0 ? ListModel::tr("Today, %1 has birthday, exactly at %2.")
		: ListModel::tr("In %n day(s) %1 has birthday, exactly at %2.", "", daysleft);
	case Anniv::DEATHDAY:
		return daysleft == 0 ? ListModel::tr("Today is %1's obit, exactly at %2.")
		: ListModel::tr("In %n day(s) is %1's obit, exactly at %2.", "", daysleft);
	case Anniv::NAMESDAY:
		return daysleft == 0 ? ListModel::tr("Today, %1 has namesday, exactly at %2.")
		: ListModel::tr("In %n day(s) %1 has namesday, exactly at %2.", "", daysleft);
	case Anniv::WEDDING:
		return daysleft == 0 ? ListModel::tr("Today is %1's %3th wedding day, exactly at %2.")
		: ListModel::tr("In %n day(s) is %1's %3th wedding day, exactly at %2.", "", daysleft);
	default:
		return daysleft == 0 ? ListModel::tr("Today is %1, exactly at %2.")
		: ListModel::tr("In %n day(s) is %1, exactly at %2.", "", daysleft);
	}
}

static QString formatCommentWithYear(Anniv::Type type, int daysleft)
{
	switch (type)
	{
	case Anniv::BIRTHDAY:
		return daysleft == 0 ? ListModel::tr("Today at %4 %1 becomes %3, %1 was born on %2.")
		: ListModel::tr("In %n day(s) at %4 %1 becomes %3, %1 was born on %2.", "", daysleft);
	case Anniv::ANNIVERSARY:
		return daysleft == 0 ? ListModel::tr("Today at %4 is %1 for the %3th time. This day originated on %2.")
		: ListModel::tr("In %n day(s) at %4 is %1 for the %3th time. This day originated on %2.","", daysleft);
	case Anniv::DEATHDAY:
		return daysleft == 0 ? ListModel::tr("Today at %4 %1 is dead for %3 years. %1 died on %3.")
		: ListModel::tr("In %n day(s) at %4 %1 is dead for %3 years. %1 died on %3.", "", daysleft);
	case Anniv::NAMESDAY:
		return daysleft == 0 ? ListModel::tr("Today at %4 %1 has namesday. This day originated on %2, thus it is %3 years old.")
		: ListModel::tr("In %n day(s) at %4 %1 has namesday. This day originated on %2, thus it is %3 years old.", "", daysleft);
	case Anniv::WEDDING:
		return daysleft == 0 ? ListModel::tr("Today at %4 is %1's %3th wedding day. %1 is married since %2.")
		: ListModel::tr("In %n day(s) at %4 is %1's %3th wedding day. %1 is married since %2.", "", daysleft);
	default:
		return daysleft == 0 ? ListModel::tr("Today at %4 is %1 for the %3th time. This day originated on %2.")
		: ListModel::tr("In %n day(s) at %4 is %1 for the %3th time. This day originated on %2.", "", daysleft);
	}
}


QVariant ListModel::data(const QModelIndex & index, int role) const
{
	if (!index.isValid())
		return QVariant();
	if (index.row() >= rowCount())
		return QVariant();
	if (role == Qt::DisplayRole)
	{
		int row = permut.at(index.row()).row();

		Anniv::Type type = Anniv::getType(model->data(model->index(row, TableModel::COLUMN_TYPE)).toString());

		bool hasYear = model->data(model->index(row, TableModel::COLUMN_YEARENABLED), Qt::CheckStateRole).toInt() == Qt::Checked;

		QDate date = hasYear ? 
							model->data(model->index(row, TableModel::COLUMN_DATE)).toDate() : 
							QDate::fromString(model->data(model->index(row,TableModel::COLUMN_DATE)). toString(),Anniv::instance().shortDateFormat());
		QString format;
		int daysleft = model->data(model->index(row, TableModel::COLUMN_DAYSLEFT)).toInt();


		QDate dateNow(QDate::currentDate().year(), date.month(), date.day());

		if(!hasYear)
		{
			format = formatComment(type, daysleft).arg(model->data(model->index(row, TableModel::COLUMN_NAME)).toString())
												  .arg(dateNow.toString(tr("dddd, d MMMM")));
		}
		else
		{
			format = formatCommentWithYear(type, daysleft);
			format = format.arg(model->data(model->index(row, TableModel::COLUMN_NAME)).toString())
						   .arg(date.toString(tr("dddd, d MMMM")))
						   .arg(dateNow.year() - date.year())
						   .arg(dateNow.toString(tr("dddd")));
		}

//						.arg(hasYear ? date.toString(Qt::SystemLocaleLongDate) : date. toString(tr("dddd, d MMMM")));
//		if(hasYear) format = format.arg(QDate::currentDate().year() - date.year());

		return format;

	}
	else if (role == Qt::DecorationRole)
	{
		const Anniv& anniv = Anniv::instance();
		return anniv.getIcon(anniv.getTypeTranslated(model->data(model->index(permut.at(index.row()).row(), TableModel::COLUMN_TYPE)).toString()));
	}
	else
		return QVariant();
}

int ListModel::rowCount(const QModelIndex & parent) const 
{
	Q_UNUSED(parent);
	return permut.size();
}

QVariant ListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	Q_UNUSED(section);
	Q_UNUSED(orientation);
	if (role == Qt::DisplayRole)
		return tr("Reminders");
	else
		return QVariant();
}
