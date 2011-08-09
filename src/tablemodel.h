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


#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include "card.h"
class UndoCommand;
#include <QFile>
#include <QAbstractTableModel>

class TableModel : public QAbstractTableModel
{
  Q_OBJECT 
  public:
	enum COLUMNS
	{
		COLUMN_TYPE = 0,
		COLUMN_NAME,
		COLUMN_DATE,
		COLUMN_YEARENABLED,
		COLUMN_COMMENT,
		COLUMN_DAYSLEFT,
		COLUMN_END
	};


	  TableModel(QString filename, QObject * parent = 0);

	const Card & at(int i)
	{
		return cards.at(i);
	}

	int rowCount(const QModelIndex & parent = QModelIndex())const
	{
		Q_UNUSED(parent);
		return cards.size() + 1;
	}
	int columnCount(const QModelIndex & parent = QModelIndex())const
	{
		Q_UNUSED(parent);
		return COLUMN_END;
	}
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole)const;

	QVariant headerData(int section, Qt::Orientation orientation, int role =
						Qt::DisplayRole) const;
	bool setData(const QModelIndex & index, const QVariant & value, int role =
				 Qt::EditRole);
	bool setData(int row, int column, const QVariant & value);
	Qt::ItemFlags flags(const QModelIndex & index) const;

	bool insertRows(int row, int count, const QModelIndex & parent =
					QModelIndex());

	bool removeRows(int row, int count, const QModelIndex & parent =
					QModelIndex());

	bool removeColumns(int column, int count, const QModelIndex & parent =
					   QModelIndex())
	{
		Q_UNUSED(column);
		Q_UNUSED(count);
		Q_UNUSED(parent);
		return false;
	}
	bool insertColumns(int column, int count, const QModelIndex & parent =
					   QModelIndex())
	{
		Q_UNUSED(column);
		Q_UNUSED(count);
		Q_UNUSED(parent);
		return false;
	}
	QStringList mimeTypes() const;
	bool dropMimeData(const QMimeData * data, Qt::DropAction action, int row,
					  int column, const QModelIndex & parent);


	public slots:int appendCard(const Card & c);
	void insertCard(const Card & c, int row);
	void save();
  signals:
	void commandInvoked(UndoCommand *);
  private:
	QFile * file;
	QList < Card > cards;
	// * settings;
};

#endif //TABLEMODEL_H

