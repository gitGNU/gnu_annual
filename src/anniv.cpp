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
	icons.push_back(QIcon(":/icons/anniversary.png"));
	icons.push_back(QIcon(":/icons/birthday.png"));
	icons.push_back(QIcon(":/icons/deathday.png"));
	icons.push_back(QIcon(":/icons/event.png"));
	icons.push_back(QIcon(":/icons/holiday.png"));
	icons.push_back(QIcon(":/icons/memorial.png"));
	icons.push_back(QIcon(":/icons/wedding.png"));
	icons.push_back(QIcon(":/icons/namesday.png"));
	icons.push_back(QIcon());

	types.push_back(QObject::tr("anniversary"));
	types.push_back(QObject::tr("birthday"));
	types.push_back(QObject::tr("death"));
	types.push_back(QObject::tr("event"));
	types.push_back(QObject::tr("holiday"));
	types.push_back(QObject::tr("memorial"));
	types.push_back(QObject::tr("wedding"));
	types.push_back(QObject::tr("namesday"));
	types.push_back(QObject::tr("invalid"));

	longformat = QLocale::system().dateFormat(QLocale::ShortFormat);
	if (longformat.count('y') == 2)
		longformat.replace("yy", "yyyy");
	shortformat = longformat;
	shortformat.remove(QRegExp("[/\\.]?y+[/\\.]?"));
}
