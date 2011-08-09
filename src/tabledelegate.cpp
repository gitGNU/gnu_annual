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
#include "tabledelegate.h"
#include <QAbstractItemModel>
#include <QComboBox>
#include <QDateEdit>
#include "tablemodel.h"


TableDelegate::TableDelegate(QAbstractItemModel * _model, QObject * parent)
		: QItemDelegate(parent), model(_model)
{}

QWidget *TableDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	switch (index.column())
	{
	case TableModel::COLUMN_TYPE:
		{
			QComboBox *editor = new QComboBox(parent);
			for (int i = 0; i < Anniv::ANNIV_INVALID; ++i)
				editor->addItem(Anniv::instance().  getIcon(static_cast < Anniv::Type > (i)), Anniv::instance().getString(static_cast < Anniv::Type > (i)));
			return editor;
		}
	case TableModel::COLUMN_DATE:
		{
			QDateEdit *editor = new QDateEdit(parent);

			if (model->
				data(model->index(index.row(), TableModel::COLUMN_YEARENABLED),
					 Qt::CheckStateRole).toInt() == Qt::Checked)
				editor->setDisplayFormat(Anniv::instance().longDateFormat());
			else
				editor->setDisplayFormat(Anniv::instance().shortDateFormat());
			return editor;
		}
	default:
		return QItemDelegate::createEditor(parent, option, index);
	}
}

void TableDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
	switch (index.column())
	{
	case TableModel::COLUMN_TYPE:
		{
			int value = index.model()->data(index, Qt::EditRole).toInt();
			QComboBox *box = static_cast < QComboBox * >(editor);
			box->setCurrentIndex(value);
			return;
		}
	case TableModel::COLUMN_DATE:
		{
			QDateEdit *edit = static_cast < QDateEdit * >(editor);
			if (model->
				data(model->index(index.row(), TableModel::COLUMN_YEARENABLED),
					 Qt::CheckStateRole).toInt() == Qt::Checked)
			{
				QDate value =
					index.model()->data(index, Qt::EditRole).toDate();
				edit->setDate(value);
			}
			else
			{
				QDate value =
					QDate::fromString(index.model()->data(index, Qt::EditRole).
									  toString(),
									  Anniv::instance().shortDateFormat());
				edit->setDate(value);
			}
			return;
		}
	default:
		return QItemDelegate::setEditorData(editor, index);
	}
}

void TableDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const 
{
	switch (index.column())
	{
	case TableModel::COLUMN_TYPE:
		{
			QComboBox *box = static_cast < QComboBox * >(editor);
			int value = box->currentIndex();
			model->setData(index, value, Qt::EditRole);
			return;
		}
	case TableModel::COLUMN_DATE:
		{
			QDateEdit *edit = static_cast < QDateEdit * >(editor);
			QDate value = edit->date();
			model->setData(index, value, Qt::EditRole);
			return;
		}
	default:
		return QItemDelegate::setModelData(editor, model, index);
	}
}

