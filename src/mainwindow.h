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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tablemodel.h"

class TableModel;
class QUndoStack;
class QSettings;
class QSortFilterProxyModel;
class TableView;

class MainWindow:public QMainWindow
{
  Q_OBJECT public:
	MainWindow(QWidget * parent = 0);
  public slots:
	void push(UndoCommand * cmd);
	void push(const Card & card);
	void print();
	void launchConfig();
	void save();
	void removeSelected();

	void OnShowUndoView();
	void OnShowCardInserter();
	void OnAboutDialog();
  protected:
	virtual void closeEvent(QCloseEvent * event);
  private:
	QUndoStack * undostack;
	TableView *tableview;
	QSortFilterProxyModel *proxyModel;
	TableModel *model;
	QSettings *settings;


	QDockWidget *undoviewDockWidget;
	QDockWidget *cardinserterDockWidget;
};





#endif // MAINWINDOW_H
