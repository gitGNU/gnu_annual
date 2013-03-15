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
#include "card.h"
#include <QDebug>
#include <QIcon>


bool operator==(const Card & a, const Card & b)
{
	return a.name() == b.name() && a.type() == b.type() && a.comment() == b.comment() && a.hasYear() == b.hasYear() && 
		(a.hasYear()? a.date() == b.date() : (a.date().month() == b.date().month() && a.date().day() == b.date().day()));
}

bool operator!=(const Card & a, const Card & b)
{
	return !operator==(a, b);
}

QDebug operator<<(QDebug q, const IOException & e)
{
	return q << e.message();
}

bool operator<(const Card & a, const Card & b)
{
	if (a.date().month() == b.date().month()) return a.date().day() < b.date().day();
	return a.date().month() < b.date().month();
}

QDebug operator<<(QDebug q, const Card & e)
{
	return q << Anniv::instance().getString(e.type()) << ';' << e.date() << ';' << e.name() << ';' << e.comment();
}

QStringList readCSV(QString line)
{
	int pos = 0, lastpos = 0;

	const char sep = (line.count(';') > line.count(',')) ? ';' : ':';

	QStringList strs;
	while (pos < line.length())
	{
		bool quote = false;
		if (line[pos] == '\"')
		{
			quote = true;
			++pos;
		}
		lastpos = pos;
		while (pos < line.length())
		{
			if (quote == true && line[pos] == '\"')
			{
				++pos;
				const QChar& c = line[pos];
				if(c.toLatin1() == sep)
				{
					line.remove(pos - 1, 1);
					break;
				}
				else if(c.toLatin1() == '\"')
				{
					line.remove(pos - 1, 1);
				}
			}
			if (quote == false && line[pos] == sep)
				break;
			++pos;
		}
		if (pos > lastpos + (quote ? 1 : 0))
			strs.append(line.midRef(lastpos, pos - lastpos - (quote ? 1 : 0)).toString());
		++pos;
	}
	return strs;
}

static char getDateSep(QString str)
{
	const static char arr[] = { '-' , '.', '/', '\0' };
	int maxindex = 0;
	for(int i = 1; arr[i] != 0; ++i)
		if(str.count(arr[maxindex]) < str.count(arr[i])) maxindex = i;
	return arr[maxindex];
}

Card *readCard(QString line)
{
	QStringList col = readCSV(line);
	if(col.isEmpty())
		return (Card *) 0;
	if(col.length() < 3)
		throw IOException(QObject::tr("The line `%1` is incomplete").arg(col.join("*")));
	Anniv::Type type = Anniv::getType(col[0]);
	if(type == Anniv::ANNIV_INVALID)
		throw IOException(QObject::tr("Invalid type %1").arg(col[0]));

	const char datumsep = getDateSep(col[1]);
	QStringList datum = col[1].split(datumsep);
	QDate *date;
	if(datum.size() < 2)
		throw IOException(QObject::tr("The date %1 is invalid!").arg(col[1]));
	bool hasYear;
	if (datum.length() == 3)
	{
		if (QDate::isValid(datum[0].toInt(), datum[1].toInt(), datum[2].toInt()))
			date = new QDate(datum[0].toInt(), datum[1].toInt(), datum[2].toInt());
		else if (QDate::isValid(datum[2].toInt(), datum[1].toInt(), datum[0].toInt()))
			date = new QDate(datum[2].toInt(), datum[1].toInt(), datum[0].toInt());
		else
			throw IOException(QObject::tr("The date %1 is invalid!").arg(col[1]));
		hasYear = true;
	}
	else
	{
		if (QDate::isValid(2000, datum[0].toInt(), datum[1].toInt()))
			date = new QDate(2000, datum[0].toInt(), datum[1].toInt());
		else
			throw IOException(QObject::tr("The date %1 is invalid!").arg(col[1]));
		hasYear = false;
	}

	Card *card = new Card(type, *date, col[2], col.length() > 3 ? col[3] : "", hasYear);
	delete date;
	return card;
}

QList < Card > readCards(QIODevice * dev)
{
	Q_ASSERT(dev != 0);
	QList<Card> liste;

	if (!dev->isOpen())
		dev->open(QIODevice::ReadOnly);
	QTextStream stream(dev);
	while (!stream.atEnd())
	{
		QString line = stream.readLine();
		if (line.isEmpty() || line.at(0) == '#')
			continue;
		Card *card = readCard(line);
		if (card)
		{
			liste.push_back(*card);
			delete card;

		}
	}

	return liste;
}

QTextStream & writeCSVEscaped(QTextStream & os, QString str)
{
	static const QChar quot('\"');
	static const QRegExp regex("[\";,]");
	if (str.contains(regex))
	{
		str.replace("\"", "\"\"");
		os << quot << str << quot;
	}
	else
		os << str;
	return os;
}


QTextStream & operator<<(QTextStream & os, const Card & card)
{
	static const QChar sep = QChar(';');

	os << Anniv::instance().getString(card.type())
		<< sep
		<< ((card.hasYear())? card.date().toString("yyyy.MM.dd") : card.date().toString("MM.dd")) << sep;
	writeCSVEscaped(os, card.name());
	os << sep;
	writeCSVEscaped(os, card.comment());
	return os;
}

QTextStream & operator<<(QTextStream & os, const QList < Card > &cards)
{
	for (QList < Card >::const_iterator it = cards.begin(); it != cards.end(); ++it)
	{
		if(!it->name().isEmpty())
			os << *it << endl;
	}
	return os;
}


void writeCards(QIODevice * dev, QList < Card > cards)
{
	if (!dev->isOpen())
		dev->open(QIODevice::WriteOnly);
	QTextStream stream(dev);
	stream << cards;
}
