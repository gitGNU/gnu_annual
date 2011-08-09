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

#ifndef UNDOCOMMAND_H
#define UNDOCOMMAND_H
#include <QUndoCommand>
#include "tablemodel.h"

class UndoCommand:public QUndoCommand
{
  protected:
	UndoCommand(TableModel * _model):model(_model)
	{
	}
	enum
	{
		ADD_COMMAND = 0,
		ADDS_COMMAND,
		DELETE_COMMAND,
		CHANGE_COMMAND
	};
	TableModel *model;
};


class AddCardCommand:public UndoCommand
{
  public:
	AddCardCommand(TableModel * model, const Card & card);

	virtual void undo();
	virtual void redo();
	virtual int id() const
	{
		return ADD_COMMAND;
	}
  private:
	const Card card;
	int row;
};

class AddCardsCommand:public UndoCommand
{
  public:
	AddCardsCommand(TableModel * model, const QList < Card > &card);

	virtual void undo();
	virtual void redo();
	virtual int id() const
	{
		return ADDS_COMMAND;
	}
	virtual bool mergeWith(const QUndoCommand * command);
  private:
	void createTitle();
	  QList < Card > cards;
	  QList < int >rows;
};

class DeleteCardCommand:public UndoCommand
{
  public:
	DeleteCardCommand(TableModel * model, int row);
	virtual void undo();
	virtual void redo();
	virtual int id() const
	{
		return DELETE_COMMAND;
	}
  private:
	int row;
	const Card card;
};

class ChangeCardCommand:public UndoCommand
{
	enum FIELDS
	{
		FIELD_NAME = 1,
		FIELD_DATE = 2,
		FIELD_TYPE = 4,
		FIELD_COMMENT = 8,
		FIELD_YEAR = 16
	};

  public:
	  ChangeCardCommand(TableModel * model, int _row,
						TableModel::COLUMNS column, const Card & _newcard);
	virtual void undo();
	virtual void redo();
	virtual int id() const
	{
		return CHANGE_COMMAND;
	}
	virtual bool mergeWith(const QUndoCommand * command);
  private:
	int row;
	int flags;
	Card oldcard;
	Card newcard;


};



#endif // UNDOCOMMAND_H
