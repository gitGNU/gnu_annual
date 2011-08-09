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
#include "undocommand.h"

AddCardCommand::AddCardCommand(TableModel * model,
							   const Card & _card):UndoCommand(model),
card(_card), row(0)
{
	setText(QObject::tr("%1 in row %2 added.").arg(card).arg(row));
}

void AddCardCommand::undo()
{
	model->removeRows(row, 1, QModelIndex());
}

void AddCardCommand::redo()
{
	row = model->appendCard(card);
}


AddCardsCommand::AddCardsCommand(TableModel * model,
								 const QList < Card >
								 &_cards):UndoCommand(model), cards(_cards)
{
	for (int i = 0; i < cards.size(); ++i)
		rows.push_back(0);
	createTitle();
}

void AddCardsCommand::createTitle()
{
	QString title;
	for (int i = 0; i < cards.size(); ++i)
	{
		title.append(cards.at(i).name());
		title.append(',');
	}
	title.remove(title.size() - 1, 1);

	setText(QObject::tr("Cards (%1) added").arg(title));
}

void AddCardsCommand::undo()
{
	QList < int >::const_iterator it = rows.constEnd();
	--it;
	while (it != rows.constBegin())
	{
		model->removeRows(*it--, 1, QModelIndex());
	}
	model->removeRows(*it, 1, QModelIndex());

}

void AddCardsCommand::redo()
{
	for (int i = 0; i < cards.size(); ++i)
		rows[i] = model->appendCard(cards[i]);
}

bool AddCardsCommand::mergeWith(const QUndoCommand * command)
{
	if (command->id() != id())
		return false;
	if (dynamic_cast < const UndoCommand * >(command) == NULL)
		return false;
	const AddCardsCommand *c =
		dynamic_cast < const AddCardsCommand * >(command);
	if (c->model != model)
		return false;
	cards.append(c->cards);
	rows.append(c->rows);
	createTitle();

	return true;
}

DeleteCardCommand::DeleteCardCommand(TableModel * model, int _row):UndoCommand(model), row(_row),
card(model->
	 at(row))
{
	setText(QObject::tr("%1 in row %2 removed.").arg(card).arg(row));
}

void DeleteCardCommand::undo()
{
	model->insertCard(card, row);
}

void DeleteCardCommand::redo()
{
	model->removeRows(row, 1, QModelIndex());
}


ChangeCardCommand::ChangeCardCommand(TableModel * model, int _row,
									 TableModel::COLUMNS column,
									 const Card & _newcard):UndoCommand(model),
row(_row), oldcard(model->at(_row)), newcard(_newcard)
{
	switch (column)
	{
	case TableModel::COLUMN_COMMENT:
		flags = FIELD_COMMENT;
		break;
	case TableModel::COLUMN_DATE:
		flags = FIELD_DATE;
		break;
	case TableModel::COLUMN_NAME:
		flags = FIELD_NAME;
		break;
	case TableModel::COLUMN_TYPE:
		flags = FIELD_TYPE;
		break;
	case TableModel::COLUMN_YEARENABLED:
		flags = FIELD_YEAR;
		break;

	default:
		Q_ASSERT(false);
	}

	setText(QObject::tr("%1 in row %2 changed to %3.").arg(oldcard).arg(row).
			arg(newcard));
}

void ChangeCardCommand::undo()
{
	if (flags & FIELD_COMMENT)
		model->setData(row, TableModel::COLUMN_COMMENT, oldcard.comment());
	if (flags & FIELD_NAME)
		model->setData(row, TableModel::COLUMN_NAME, oldcard.name());
	if (flags & FIELD_DATE)
		model->setData(row, TableModel::COLUMN_DATE, oldcard.date());
	if (flags & FIELD_TYPE)
		model->setData(row, TableModel::COLUMN_TYPE, oldcard.type());
	if (flags & FIELD_YEAR)
		model->setData(row, TableModel::COLUMN_YEARENABLED, oldcard.hasYear());
}

void ChangeCardCommand::redo()
{
	if (flags & FIELD_COMMENT)
		model->setData(row, TableModel::COLUMN_COMMENT, newcard.comment());
	if (flags & FIELD_NAME)
		model->setData(row, TableModel::COLUMN_NAME, newcard.name());
	if (flags & FIELD_DATE)
		model->setData(row, TableModel::COLUMN_DATE, newcard.date());
	if (flags & FIELD_TYPE)
		model->setData(row, TableModel::COLUMN_TYPE, newcard.type());
	if (flags & FIELD_YEAR)
		model->setData(row, TableModel::COLUMN_YEARENABLED,
					   newcard.hasYear()? Qt::Checked : Qt::Unchecked);

}

bool ChangeCardCommand::mergeWith(const QUndoCommand * command)
{
	if (command->id() != id())
		return false;
	if (dynamic_cast < const UndoCommand * >(command) == NULL)
		return false;
	const ChangeCardCommand *c =
		dynamic_cast < const ChangeCardCommand * >(command);
	if (c->model != model)
		return false;
	if (c->row != row)
		return false;
	if ((c->flags & flags) != 0)
		return false;
	if (c->flags | FIELD_NAME)
		newcard.setName(c->newcard.name());
	if (c->flags | FIELD_DATE)
		newcard.setDate(c->newcard.date());
	if (c->flags | FIELD_TYPE)
		newcard.setType(c->newcard.type());
	if (c->flags | FIELD_COMMENT)
		newcard.setComment(c->newcard.comment());
	if (c->flags | FIELD_YEAR)
		newcard.enableYear(c->newcard.hasYear());
	flags |= c->flags;
	setText(QObject::tr("%1 in row %2 changed to %3.").arg(oldcard).arg(row).
			arg(newcard));
	return true;
}
