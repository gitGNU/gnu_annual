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

sudo apt-get install qtbase5-dev qttools5-dev-tools qttools5-dev debhelper liblog-log4perl-perl libxml-xpath-perl debhelper
cd `dirname $0` &&
cd ../..
cmake . &&
cp -a dist/debian . &&
fakeroot dh binary
