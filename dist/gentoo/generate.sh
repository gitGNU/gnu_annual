#!/bin/sh
#
# annual - Reminder for annual events
# Keeps track of all your anniversaries and hopefully reminds you at the right time.
# Copyright (C) 2011 Dominik Köppl
#
# This program is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option)
# any later version.
# 
# This program is distributed in the hope that it will be useful, but WITHOUT 
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
# more details.
# 
# You should have received a copy of the GNU General Public License along
# with this program.  If not, see <http://www.gnu.org/licenses/>.

d=`mktemp -d`
fd="$d/app-office/annual"
ebuild='annual-9999.ebuild'
cd `dirname $0` &&
mkdir -p "$fd" &&
cp * "$fd/" &&
cd "$fd/" &&
ebuild $ebuild manifest &&
ebuild $ebuild merge
