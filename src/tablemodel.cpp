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
#include "tablemodel.h"
#include "undocommand.h"

#include <QMimeData>

TableModel::TableModel(QString filename, QObject * parent):QAbstractTableModel(parent), file(new QFile(filename, this)), cards(readCards(file))
{
	setSupportedDragActions(Qt::CopyAction | Qt::MoveAction);
	file->close();
}

void TableModel::save()
{
	if (file->isOpen())
		file->close();
	file->open(QIODevice::WriteOnly);
	writeCards(file, cards);
	file->close();
}

QVariant TableModel::data(const QModelIndex & index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (index.row() < cards.size() && role == Qt::CheckStateRole
		&& index.column() == COLUMN_YEARENABLED)
		return cards.at(index.row()).hasYear()? Qt::Checked : Qt::Unchecked;
	if (role != Qt::DisplayRole && role != Qt::EditRole)
		return QVariant();
	if (index.row() == cards.size() && index.column() == COLUMN_DATE)
		return QDate();			// Give a QDate to the delagate to show
								// QDateEdit
	if (index.row() >= cards.size())
		return QVariant(tr("(null)"));



	const Card & ref = cards.at(index.row());
	switch (index.column())
	{
	case COLUMN_TYPE:
		return Anniv::instance().getString(ref.type());
	case COLUMN_NAME:
		return ref.name();
	case COLUMN_DATE:
		{
			if (ref.hasYear())
				return ref.date();
			return ref.date().toString(Anniv::instance().shortDateFormat());
		}
	case COLUMN_YEARENABLED:
		return ref.hasYear()? tr("with") : tr("without");
	case COLUMN_COMMENT:
		return ref.comment();
	case COLUMN_DAYSLEFT:
		return ref.daysLeft();
	default:
		return QVariant();
	}
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (section < 0)
		return QVariant();
	if (orientation == Qt::Vertical && role == Qt::DecorationRole
		&& section < cards.size())
		return Anniv::instance().getIcon(cards.at(section).type());
	if (role != Qt::DisplayRole)
		return QVariant();
	if (orientation == Qt::Horizontal)
	{
		switch (section)
		{
		case COLUMN_TYPE:
			return tr("Type");
		case COLUMN_NAME:
			return tr("Name");
		case COLUMN_DATE:
			return tr("Date");
		case COLUMN_YEARENABLED:
			return tr("Year");
		case COLUMN_COMMENT:
			return tr("Comment");
		case COLUMN_DAYSLEFT:
			return tr("Days left");
		default:
			return QVariant();
		}
	}
	else
	{
		if (section >= cards.size())
			return QVariant();
		return cards.at(section).name();
	}
}

bool TableModel::setData(int row, int column, const QVariant & value)
{
	if (row >= cards.size())
		return false;
	Card & ref = cards[row];
	switch (column)
	{
	case COLUMN_TYPE:
		ref.setType(value.toInt());
		break;
	case COLUMN_NAME:
		ref.setName(value.toString());
		break;
	case COLUMN_DATE:
		ref.setDate(value.toDate());
		break;
	case COLUMN_YEARENABLED:
		ref.enableYear(value.toInt() == Qt::Checked);
		break;
	case COLUMN_COMMENT:
		ref.setComment(value.toString());
		break;
	default:
		return false;
	}
	emit dataChanged(index(row, column), index(row, column));
	return true;
}

bool TableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	Q_UNUSED(role);
	if (!index.isValid())
		return false;
	if (index.row() > cards.size())
		return false;
	bool isNew = false;
	if (index.row() == cards.size())
		isNew = true;			// appendCard(Card());

	Card newcard = isNew ? Card() : cards[index.row()];
	switch (index.column())
	{
	case COLUMN_TYPE:
		newcard.setType(value.toInt());
		break;
	case COLUMN_NAME:
		newcard.setName(value.toString());
		break;
	case COLUMN_DATE:
		newcard.setDate(value.toDate());
		break;
	case COLUMN_YEARENABLED:
		newcard.enableYear(value.toInt() == Qt::Checked);
		break;
	case COLUMN_COMMENT:
		newcard.setComment(value.toString());
		break;
	default:
		return false;
	}
	if (isNew)
		emit commandInvoked(new AddCardCommand(this, newcard));
	else
if (cards[index.row()] != newcard)
	emit commandInvoked(new
						ChangeCardCommand(this, index.row(),
										  static_cast < COLUMNS >
										  (index.column()), newcard));
	return true;
}

Qt::ItemFlags TableModel::flags(const QModelIndex & index) const
{

	if (!index.isValid())
		return Qt::ItemIsDropEnabled | Qt::ItemIsEnabled;
	if (index.column() >= columnCount() || index.row() >= rowCount())
		return Qt::ItemIsEnabled;
	if (index.column() == COLUMN_DAYSLEFT)
		return 0;				// Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	if (index.column() == COLUMN_YEARENABLED)
		return Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
	return Qt::ItemIsDragEnabled | Qt::ItemIsEditable | Qt::
		ItemIsEnabled | Qt::ItemIsSelectable;


}

#include <QDebug>
#include <QBuffer>
#include <QMessageBox>
QStringList TableModel::mimeTypes() const 
{
	return QStringList() << "text/csv";
}

bool TableModel::dropMimeData(const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent)
{
	if (action == Qt::IgnoreAction)
		return true;
	if (parent.isValid() && (row == -1 || column == -1))
	{
		row = parent.row();
		column = parent.column();
	}
	if (!data->hasFormat("text/csv"))
		return false;
	QByteArray bdata = data->data("text/csv");
	QBuffer *buffer = new QBuffer(&bdata);
	buffer->open(QIODevice::ReadOnly);
	buffer->seek(0);

	try
	{
		QList < Card > list = readCards(buffer);
		emit commandInvoked(new AddCardsCommand(this, list));
		return true;
	}
	catch(const IOException & e)
	{
		QMessageBox::critical(0, tr("Cannot drop data"),
							  tr
							  ("Your drop-action cannot be accomplished, because your data is erroneous.\nTranformation caused the error: %1").
							  arg(e.message()));

	}

	return false;
}


bool TableModel::insertRows(int row, int count, const QModelIndex & parent)
{
	Q_UNUSED(parent);
	if (count < 1)
		return false;

	if (row < 0)
		row = 0;
	else if (row > rowCount())
		row = rowCount();

	beginInsertRows(QModelIndex(), row, row + count - 1);
	for (int i = 0; i < count; ++i)
	{
		cards.insert(row + i, Card());
	}
	endInsertRows();


	return true;
}

bool TableModel::removeRows(int row, int count, const QModelIndex & parent)
{
	Q_UNUSED(parent);
	if (count < 1)
		return false;
	if (row < 0)
		row = 0;
	if (row >= rowCount())
		row = rowCount() - 1;
	if (count + row >= rowCount())
		count = rowCount() - row - 1;

	beginRemoveRows(QModelIndex(), row, row + count - 1);
	for (int i = 0; i < count; ++i)
	{
		cards.removeAt(i + row);
	}
	endRemoveRows();
	return true;

}

int TableModel::appendCard(const Card & c)
{
	beginInsertRows(QModelIndex(), rowCount(), rowCount());
	cards.push_back(c);
	endInsertRows();
	return cards.size() - 1;
}

void TableModel::insertCard(const Card & c, int row)
{
	if (row < 0)
		row = 0;
	else if (row > rowCount())
		row = rowCount();

	beginInsertRows(QModelIndex(), row, row);
	cards.insert(row, c);
	endInsertRows();
}

