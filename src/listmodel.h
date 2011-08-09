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
#ifndef LISTMODEL_H
#define LISTMODEL_H
#include <QAbstractListModel>
#include <QStringList>
class TableModel;
class SortableRow;

class SortableRow
{
  public:
	SortableRow(TableModel * model, int row)
		: _model(model), _row(row)
	{
	}
	int row() const
	{
		return _row;
	}
	int value() const;
  private:
	  TableModel * _model;
	int _row;
};



class ListModel:public QAbstractListModel
{
  Q_OBJECT public:
	ListModel(TableModel * _model, QObject * parent = 0);
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation,
						int role) const;
	int rowCount(const QModelIndex & parent = QModelIndex())const;

	int daysLeft(const QModelIndex & index) const;

	private slots:
		void dataChange(const QModelIndex & topLeft, const QModelIndex & bottomRight);
	void onRowsRemoved(const QModelIndex & parent, int start, int end);
	void rowsInsert(const QModelIndex & parent, int start, int end);
  private:
	  TableModel * model;
	QStringList lines;
	  QList < SortableRow > permut;
};
#endif // LISTMODEL_H

