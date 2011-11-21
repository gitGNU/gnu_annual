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
#include <QIcon>
#include <QLocale>
#include "anniv.h"


Anniv::Anniv()
{

	{
		int i=0;
		icons[i++] = QIcon(":/icons/anniversary.png");
		icons[i++] = QIcon(":/icons/birthday.png");
		icons[i++] = QIcon(":/icons/deathday.png");
		icons[i++] = QIcon(":/icons/event.png");
		icons[i++] = QIcon(":/icons/holiday.png");
		icons[i++] = QIcon(":/icons/memorial.png");
		icons[i++] = QIcon(":/icons/wedding.png");
		icons[i++] = QIcon(":/icons/namesday.png");
		icons[i++] = QIcon(":/icons/invalid.png");
	}
	{
		int i=0;
		types[i++] = QObject::tr("anniversary"); 
		types[i++] = QObject::tr("birthday");
		types[i++] = QObject::tr("death");
		types[i++] = QObject::tr("event");
		types[i++] = QObject::tr("holiday");
		types[i++] = QObject::tr("memorial");
		types[i++] = QObject::tr("wedding");
		types[i++] = QObject::tr("namesday");
		types[i++] = QObject::tr("invalid");
	}

	longformat = QLocale::system().dateFormat(QLocale::ShortFormat);
	if (longformat.count('y') == 2)
		longformat.replace("yy", "yyyy");
	shortformat = longformat;
	shortformat.remove(QRegExp("[/\\.]?y+[/\\.]?"));
}

Anniv::Type Anniv::getType(QString str)
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

Anniv::Type Anniv::getTypeTranslated(QString str) const
{
	for(size_t i = 0; i < ANNIV_INVALID; ++i)
	{
		if(str == types[i])
			return static_cast<Anniv::Type>(i);
	}
	return ANNIV_INVALID;
}
