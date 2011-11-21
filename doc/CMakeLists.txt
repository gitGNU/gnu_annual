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
set(SOURCES 2annual.pod annual.pod)

set(manfiles)
foreach(file_pod ${SOURCES})
  string(REPLACE ".pod" ".man" file_man ${file_pod})
  set(file_man "${CMAKE_CURRENT_BINARY_DIR}/${file_man}")
  add_custom_command(
    OUTPUT ${file_man} 
    COMMAND pod2man --date `stat --format=%y ${CMAKE_CURRENT_SOURCE_DIR}/${file_pod} | cut -b-10` --center "${APPLICATION_NAME} Documentation"  --release "${APPLICATION_NAME} v${APPLICATION_VERSION}" --section 1 ${CMAKE_CURRENT_SOURCE_DIR}/${file_pod} -o ${file_man}
    DEPENDS ${file_pod}
  ) 
  list(APPEND manfiles ${file_man})
endforeach()

ADD_CUSTOM_TARGET(doc ALL
  DEPENDS ${manfiles}
)

INSTALL(
 FILES ${manfiles}
 DESTINATION share/man/man1
)
