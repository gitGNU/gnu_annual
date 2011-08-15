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

class Anniv
{
  public:

	enum Type
	{
		ANNIVERSARY = 0,
		BIRTHDAY,
		DEATHDAY,
		EVENT,
		HOLIDAY,
		MEMORIAL,
		WEDDING,
		NAMESDAY,
		ANNIV_INVALID
	};

  private:
	Anniv();
	QIcon icons[ANNIV_INVALID];
	QString types[ANNIV_INVALID];
	
	QString longformat, shortformat;

  public:

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
	const QString& getString(Type type) const
	{
		if (type >= ANNIV_INVALID)
			return types[ANNIV_INVALID];
		return types[type];
	}


	static Type getType(QString str);
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

