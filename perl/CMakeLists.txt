# annual - Reminder for annual events
# Keeps track of all your anniversaries and hopefully reminds you at the
# right time.
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
#
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/2annual.pl.in" "${CMAKE_CURRENT_BINARY_DIR}/2annual" @ONLY)

ADD_TEST(gmx ${CMAKE_CURRENT_SOURCE_DIR}/test.sh ${CMAKE_CURRENT_BINARY_DIR}/2annual 0)
ADD_TEST(outlook ${CMAKE_CURRENT_SOURCE_DIR}/test.sh ${CMAKE_CURRENT_BINARY_DIR}/2annual 1)
ADD_TEST(vcard ${CMAKE_CURRENT_SOURCE_DIR}/test.sh ${CMAKE_CURRENT_BINARY_DIR}/2annual 2)

ADD_CUSTOM_TARGET(perl ALL
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/2annual
)

INSTALL(
	PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/2annual 
	DESTINATION bin
)

