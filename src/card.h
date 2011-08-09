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
#ifndef CARD_H
#define CARD_H
#include <QString>
#include <QStringList>
#include <QDate>
#include "anniv.h"
#include "ioexception.h"

class QTextStream;

class Card
{
  public:
	explicit Card()
		: _type(Anniv::ANNIVERSARY), _date(QDate::currentDate()), _hasYear(true)
	{
		updateDaysleft();
	}
	Card(int type, QDate date, QString name, QString comment = "", bool hasYear = true)
		: _type((type < 0 || type > Anniv::ANNIV_INVALID) ? Anniv::ANNIV_INVALID : (Anniv::Type) type), _date(date), _name(name), _comment(comment), _hasYear(hasYear)
	{
		updateDaysleft();
	}
	Card(Anniv::Type type, QDate date, QString name, QString comment = "", bool hasYear = true)
		: _type((type < 0 || type > Anniv::ANNIV_INVALID) ? Anniv::ANNIV_INVALID : type), _date(date), _name(name), _comment(comment), _hasYear(hasYear)
	{
		updateDaysleft();
	}
	const QDate & date() const
	{
		return _date;
	}
	Anniv::Type type() const
	{
		return _type;
	}
	const QString & name() const
	{
		return _name;
	}
	const QString & comment() const
	{
		return _comment;
	}
	bool hasYear() const
	{
		return _hasYear;
	}

	void setComment(QString comment)
	{
		_comment = comment;
	}
	void setType(QString type)
	{
		_type = Anniv::getType(type);
	}
	void setType(Anniv::Type type)
	{
		_type = type;
	}
	void setType(int type)
	{
		_type = (type < 0 || type > Anniv::ANNIV_INVALID) ? Anniv::ANNIV_INVALID : (Anniv::Type) type;
	}
	void setName(QString name)
	{
		_name = name;
	}
	void enableYear(bool enableYear)
	{
		_hasYear = enableYear;
	}


	void setDate(const QDate & date)
	{
		this->_date = date;
		updateDaysleft();
	}
	int daysLeft() const
	{
		return daysleft;
	}
	operator  QString() const
	{
		return QString("{%1 %2 %3}").arg(_name).arg(_date.toString()).arg(Anniv::instance().getString(_type));
	}
  protected:
	void updateDaysleft()
	{
		int yearadd = 0;
		if (_date.month() < QDate::currentDate().month())
			yearadd = 1;
		else if (_date.month() == QDate::currentDate().month() && _date.day() < QDate::currentDate().day())
			yearadd = 1;

		QDate anniversary(QDate::currentDate().year() + yearadd, _date.month(), _date.day());
		daysleft = QDate::currentDate().daysTo(anniversary);
	}

  private:
	Anniv::Type _type;
	QDate _date;
	QString _name;
	QString _comment;
	bool _hasYear;
	int daysleft;

	// const static int CARD_COLUMNS = 2;
};


bool operator<(const Card & a, const Card & b);
QDebug operator<<(QDebug q, const Card & e);
QDebug operator<<(QDebug q, const IOException & e);
bool operator==(const Card & a, const Card & b);
bool operator!=(const Card & a, const Card & b);
QList < Card > readCards(QIODevice * dev);
Card *readCard(QString line);
void writeCards(QIODevice * dev, QList < Card > cards);
QTextStream & operator<<(QTextStream & os, const Card & card);
QTextStream & operator<<(QTextStream & os, const QList < Card > &card);

#endif // CARD_H

