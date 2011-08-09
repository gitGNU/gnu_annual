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
#include "cardinserter.h"
#include <QDateEdit>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDateEdit>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include "card.h"

CardInserter::CardInserter(QWidget * parent)
	: QWidget(parent), date(new QDateEdit(this)), name(new QLineEdit(this)), type(new QComboBox(this)), ok(new QPushButton(tr("Insert"), this))
{
	for (int i = 0; i < Anniv::ANNIV_INVALID; ++i)
		type->addItem(Anniv::instance().
					  getIcon(static_cast < Anniv::Type > (i)),
					  Anniv::instance().getString(static_cast < Anniv::Type >
												  (i)));
	date->setDisplayFormat("dd.MM.yyyy");

	QVBoxLayout *vb = new QVBoxLayout;
	QFormLayout *layout = new QFormLayout;
	layout->addRow(tr("Type"), type);
	layout->addRow(tr("Date"), date);
	layout->addRow(tr("Name"), name);
	vb->addLayout(layout);
	vb->addWidget(ok);
	vb->addStretch(2);
	setLayout(vb);
	connect(ok, SIGNAL(clicked()), this, SLOT(clicked()));
}

void CardInserter::clicked()
{
	Card c(type->currentIndex(), date->date(), name->text());
	emit submit(c);
}
