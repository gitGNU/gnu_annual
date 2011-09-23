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
#include "mainwindow.h"
#include "config.h"

#include <QSettings>
#include <QWizard>
#include <QLabel>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QUndoStack>
#include <QUndoView>
#include <QAction>
#include <QSortFilterProxyModel>
#include <QDockWidget>
#include <QListView>
#include <QCoreApplication>
#include <QToolBar>
#include <QCloseEvent>

#include "undocommand.h"
#include "aboutdialog.h"
#include "wizardpages.h"
#include "listmodel.h"
#include "listdelegate.h"
#include "tablemodel.h"
#include "tabledelegate.h"
#include "card.h"
#include "cardinserter.h"
#include "settingsdialog.h"
#include "tableview.h"

static bool useWizard(QWidget* parent, QSettings * settings)
{
	QWizard *wiz = new QWizard(parent);
	wiz->addPage(new IntroductionPage());
	wiz->addPage(new LicensePage());


	WizardFilePage *filepage = new WizardFilePage;
	filepage->setFilename(settings->value("file").toString());

	wiz->addPage(filepage);
	if (wiz->exec() == QDialog::Rejected)
		return false;
	if (filepage->filename().isNull())
		return false;
	settings->setValue("file", filepage->filename());
	return true;
}

#include <QStringListModel>
#include <QMenu>
#include <QMenuBar>
MainWindow::MainWindow(QWidget * parent):
QMainWindow(parent), settings(new QSettings(this))
{
	setWindowTitle( APPLICATION_NAME " [*]");
	if (!settings->value("file").isNull())
	{
		QFile f(settings->value("file").toString(), this);
		if (!f.exists() || !f.open(QIODevice::ReadOnly | QIODevice::Text)
			|| !f.isReadable())
		{
			QMessageBox::critical(this, tr("Database not found"), 
					tr("The database `%1` cannot be opened. Please proceed the steps of the wizard again.").arg(settings->value("file").toString()));
			if (!useWizard(this, settings))
			{
				QMessageBox::critical(this, tr("Wizard cancelled"), tr("You have not completed the wizard. The program will exit now."));
				throw(1);
			}
		}
	}
	else
	{
		// Set default settings
		settings->setValue("criticalDays", 3);
		settings->setValue("warningDays", 7);
		settings->setValue("criticalColor", "#7f0000");
		settings->setValue("warningColor", "#7f7f00");
		settings->setValue("showInserter", true);

		if (!useWizard(this,settings))
		{
			QMessageBox::critical(0, QObject::tr("Wizard cancelled"),
								  QObject::tr("You have not completed the wizard. The program will exit now."));
			throw(1);
		}
	}
	undostack = new QUndoStack(this);
	tableview = new TableView(this);

	try
	{
		model = new TableModel(settings->value("file").toString());
	}
	catch(const IOException & e)
	{
		QMessageBox::critical(this, tr("Database error"), e);
		useWizard(this,settings);
		throw(1);
	}
	ListModel *listmodel = new ListModel(model, this);


	QStringList arguments = QCoreApplication::arguments();
	if (arguments.indexOf("-q") != -1)
	{
		if (listmodel->rowCount() == 0)
			exit(0);
		int warningDays = settings->value("warningDays").toInt();
		if (listmodel->daysLeft(listmodel->index(0, 0)) > warningDays)
			exit(0);
	}



	proxyModel = new QSortFilterProxyModel(this);
	proxyModel->setSourceModel(model);




	TableDelegate *d = new TableDelegate(proxyModel, tableview);
	tableview->setItemDelegate(d);
	tableview->setModel(proxyModel);
	tableview->setSortingEnabled(true);

	cardinserterDockWidget = new QDockWidget(tr("New Entry"), this);
	CardInserter *cardinserter = new CardInserter(this);
	cardinserterDockWidget->setWidget(cardinserter);
	cardinserterDockWidget->hide();
	addDockWidget(Qt::RightDockWidgetArea, cardinserterDockWidget);

	undoviewDockWidget = new QDockWidget(tr("Undo List"), this);
	QUndoView *undoview = new QUndoView(undostack);
	undoviewDockWidget->setWidget(undoview);
	undoviewDockWidget->hide();
	addDockWidget(Qt::RightDockWidgetArea, undoviewDockWidget);


	connect(cardinserter, SIGNAL(submit(Card)), this, SLOT(push(Card)));
	connect(model, SIGNAL(commandInvoked(UndoCommand *)), this,
			SLOT(push(UndoCommand *)));

	QAction *print = new QAction(QIcon::fromTheme("document-print"), tr("Print"), this);
	connect(print, SIGNAL(triggered()), this, SLOT(print()));




	QListView *listview = new QListView;
	ListDelegate *listdelegate = new ListDelegate(settings, listview);
	listview->setItemDelegate(listdelegate);
	listview->setSelectionMode(QAbstractItemView::NoSelection);

	listview->setModel(listmodel);

	tabwidget = new QTabWidget(this);
	tabwidget->addTab(listview, QIcon::fromTheme("view-fullscreen"), tr("View"));
	tabwidget->addTab(tableview, QIcon::fromTheme("accessories-text-editor"), tr("Edit"));
	setCentralWidget(tabwidget);
	connect(tabwidget, SIGNAL(currentChanged(int)), this, SLOT(OnCurrentChanged(int)));



	QAction *save = new QAction(QIcon::fromTheme("document-save", style()->standardIcon(QStyle::SP_DriveFDIcon)), tr("Save"), this);
	connect(save, SIGNAL(triggered()), this, SLOT(save()));
	save->setShortcut(tr("Ctrl+S"));
	save->setStatusTip(tr("Save database"));
	save->setWhatsThis(tr("Save changes to database file."));



	QAction *config = new QAction(QIcon::fromTheme("preferences-system", style()->standardIcon(QStyle::SP_ComputerIcon)), tr("Config"), this);
	connect(config, SIGNAL(triggered()), this, SLOT(launchConfig()));
	config->setShortcut(tr("Ctrl+C"));
	config->setStatusTip(tr("Edit Configuration"));
	config->setWhatsThis(tr("Opens a configuration dialog to edit preferences."));


	QAction *remove = new QAction(QIcon::fromTheme("edit-delete", style()->standardIcon(QStyle::SP_TrashIcon)), tr("Remove"), this);
	connect(remove, SIGNAL(triggered()), this, SLOT(removeSelected()));
	remove->setShortcut(tr("Ctrl+X"));
	remove->setStatusTip(tr("Remove Entries"));
	remove->setWhatsThis(tr("Removes the selected rows from the table."));

	QAction *undo = undostack->createUndoAction(this);
	undo->setIcon(QIcon::fromTheme("edit-undo", style()->standardIcon(QStyle::SP_ArrowBack)));
	undo->setShortcut(tr("Ctrl+Z"));
	undo->setStatusTip(tr("Undo change"));
	undo->setWhatsThis(tr("Undo the last change of the table."));

	QAction *redo = undostack->createRedoAction(this);
	redo->setIcon(QIcon::fromTheme("edit-redo", style()->standardIcon(QStyle::SP_ArrowForward)));
	redo->setShortcut(tr("Ctrl+Y"));
	redo->setStatusTip(tr("Redo change"));
	redo->setWhatsThis(tr("Redo the last undone change of the table."));

	QAction *close = new QAction(style()->standardIcon(QStyle::SP_TitleBarCloseButton), tr("E&xit"), this);
	connect(close, SIGNAL(triggered()), this, SLOT(close()));
	close->setShortcut(tr("Ctrl+Q"));
	close->setStatusTip(tr("Close Application"));
	close->setWhatsThis(tr("Closes this window."));




	QAction *showCardInserter = new QAction(QIcon::fromTheme("insert-text"), tr("Item Creator"), this);
	connect(showCardInserter, SIGNAL(triggered()), this, SLOT(OnShowCardInserter()));
	showCardInserter->setStatusTip(tr("Show Insertion-Box"));
	showCardInserter->setWhatsThis(tr("Pops-up a docked widget providing help for creating new entries."));
	QAction *showUndoView = new QAction(tr("Undo List"), this);
	connect(showUndoView, SIGNAL(triggered()), this, SLOT(OnShowUndoView()));
	showUndoView->setStatusTip(tr("Show Undo-List"));
	showUndoView->setWhatsThis(tr("Pops-up a docked widget providing a list of changes done to the table."));

	QAction *aboutAction = new QAction(QIcon::fromTheme("help-about"), tr("About"), this);
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(OnAboutDialog()));
	aboutAction->setStatusTip(tr("Shows the About-Dialog"));
	aboutAction->setWhatsThis(tr("Information about the version and the contributors of this program."));
	QAction *aboutQtAction = new QAction(QIcon::fromTheme("help-about"), tr("About Qt"), this);
	connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	aboutQtAction->setStatusTip(tr("About the Qt-framework"));
	aboutQtAction->setWhatsThis(tr("Get information regarding the programming framework Qt which is used by this application."));

	QMenuBar *menu = this->menuBar();

	QMenu *fileMenu = new QMenu(tr("&File"), this);
	fileMenu->addAction(print);
	fileMenu->addAction(save);
	fileMenu->addAction(close);

	QMenu *editMenu = new QMenu(tr("&Edit"), this);
	editMenu->addAction(undo);
	editMenu->addAction(redo);
	editMenu->addAction(remove);

	QMenu *optionMenu = new QMenu(tr("&Options"), this);
	optionMenu->addAction(config);

	QMenu *windowMenu = new QMenu(tr("&Window"), this);
	windowMenu->addAction(showCardInserter);
	windowMenu->addAction(showUndoView);

	QMenu *helpMenu = new QMenu(tr("&Help"), this);
	helpMenu->addAction(aboutAction);
	helpMenu->addAction(aboutQtAction);

	menu->addMenu(fileMenu);
	menu->addMenu(editMenu);
	menu->addMenu(optionMenu);
	menu->addMenu(windowMenu);
	menu->addMenu(helpMenu);

	this->statusBar();

	/* QAction* undo = QAction* redo = */
	toolbar = this->addToolBar(tr("Options"));
	toolbar->addAction(save);
	toolbar->addAction(remove);
	toolbar->addAction(undo);
	toolbar->addAction(redo);
	toolbar->addAction(print);
	toolbar->addAction(config);
	toolbar->hide();

	// CONFIG
	QVariant geometry = settings->value("mainwidget/geometry");
	if(geometry.isNull())
		showMaximized();
	else
		restoreGeometry(geometry.toByteArray());

	
}

void MainWindow::OnCurrentChanged(int index)
{
	if(index < 0) return;
	if(index == 0) 
	{
		cardinserterDockWidget->hide();
	}
	else
	{
		if(settings->value("showInserter", false).toBool())
			cardinserterDockWidget->show();
		toolbar->show();
	}
}

void MainWindow::OnAboutDialog()
{
	AboutDialog dlg;
	dlg.exec();
}

void MainWindow::OnShowUndoView()
{
	undoviewDockWidget->show();
}

void MainWindow::OnShowCardInserter()
{
	cardinserterDockWidget->show();
}


void MainWindow::launchConfig()
{
	SettingsDialog *dlg = new SettingsDialog(settings);
	dlg->exec();
}


void MainWindow::closeEvent(QCloseEvent * event)
{
	if (!undostack->isClean())
	{
		QMessageBox::StandardButton ret =
			QMessageBox::question(this, tr("The document has been modified."), tr("There are unsaved changes. Do you want to save your changes?"),
								  QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Save);
		switch (ret)
		{
		case QMessageBox::Ok:
			save();
			event->accept();
			break;
		case QMessageBox::Cancel:
			event->ignore();
			break;
		case QMessageBox::No:
			event->accept();
			break;
		default:
			event->accept();
			break;
		}
	}
	else
		event->accept();
	settings->setValue("mainwidget/geometry", saveGeometry());
	if(tabwidget->currentIndex() != 0)
		settings->setValue("showInserter", cardinserterDockWidget->isVisible());
}

bool sortIntReverse(int a, int b)
{
	return a > b;
}

void MainWindow::removeSelected()
{
	QModelIndexList indices = tableview->selectedIndexes();
	QSet < int >rows;
	for (QModelIndexList::const_iterator it = indices.begin();
		 it != indices.end(); ++it)
	{
		int row = it->row();
		if (rows.find(row) == rows.end())
			rows.insert(row);
	}
	QList < int >rowList = rows.toList();
	qSort(rowList.begin(), rowList.end(), sortIntReverse);

	for (QList < int >::const_iterator it = rowList.begin();
		 it != rowList.end(); ++it)
	{
		int row = proxyModel->mapToSource(proxyModel->index(*it, 0)).row();
		if (model->rowCount() <= row + 1)
			continue;
		undostack->push(new DeleteCardCommand(model, row));
	}
}



#include <QPrintDialog>
#include <QPrinter>
#include <QTextEdit>
void MainWindow::print()
{
	QPrintDialog printDialog(this);
	if (printDialog.exec() == QDialog::Rejected)
		return;

	QTextEdit *edit = new QTextEdit;

	QString text = "<table>";
	for (int row = 0; row < model->rowCount(); ++row)
	{
		text += "<tr>";
		for (int col = 0; col < model->columnCount(); ++col)
		{
			text +=
				"<td>" + model->data(model->index(row, col)).toString() +
				"</td>";
		}
		text += "</tr>";
	}
	text += "</table>";
	edit->setHtml(text);
	edit->render(printDialog.printer());
	delete edit;
}

void MainWindow::save()
{
	model->save();
	undostack->clear();
	setWindowModified(false);
}
void MainWindow::push(UndoCommand * cmd)
{
	setWindowModified(true);
	undostack->push((QUndoCommand *) cmd);
}
void MainWindow::push(const Card & card)
{
	setWindowModified(true);
	undostack->push(new AddCardCommand(model, card));
}
