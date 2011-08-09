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
#ifndef ANNIV_H
#define ANNIV_H

#include <QIcon>
#include <QList>
class QStringList;

class Anniv
{

  private:
	Anniv();
	QList < QIcon > icons;
	QStringList types;
	QString longformat, shortformat;

  public:

	enum Type
	{
		ANNIVERSARY = 0,
		BIRTHDAY,
		WEDDING,
		EVENT,
		MEMORIAL,
		HOLIDAY,
		DEATHDAY,
		NAMESDAY,
		ANNIV_INVALID
	};

	static const Anniv & instance()
	{
		static Anniv anniv;
		  return anniv;
	}
	const QIcon & getIcon(Type type) const
	{
		if (type >= ANNIV_INVALID)
			return icons[ANNIV_INVALID];
		return icons[type];

	}
	const QString & getString(Type type) const
	{
		if (type >= ANNIV_INVALID)
			return types[ANNIV_INVALID];
		return types[type];
	}


	static Type getType(QString str)
	{
		if (str.length() == 0)
			return ANNIV_INVALID;
		switch (str.toUpper().at(0).unicode())
		{
		case 'A':
			return ANNIVERSARY;
		case 'B':
			return BIRTHDAY;
		case 'D':
			return DEATHDAY;
		case 'E':
			return EVENT;
		case 'H':
			return HOLIDAY;
		case 'M':
			return MEMORIAL;
		case 'W':
			return WEDDING;
		case 'N':
			return NAMESDAY;
		default:
			return ANNIV_INVALID;
		}
	}
	const QString & shortDateFormat() const
	{
		return shortformat;
	}
	const QString & longDateFormat() const
	{
		return longformat;
	}

};
#endif // ANNIV_H

