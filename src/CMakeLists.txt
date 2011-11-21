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
include(${QT_USE_FILE})
include(CMakeListsFiles.txt)

QT4_ADD_RESOURCES(annual_RES_CPP ${annual_RES_SRC})
QT4_WRAP_CPP(annual_MOC_CPP ${annual_MOC_H})

include(${QT_USE_FILE})

file (GLOB TRANSLATIONS_FILES translations/*.ts)
QT4_ADD_TRANSLATION(QM_FILES ${TRANSLATIONS_FILES})
add_custom_target (translations DEPENDS ${QM_FILES})
add_custom_target (annual_RES_CPP DEPENDS ${QM_FILES})
# Translation-QRC
# thanks to
# http://stackoverflow.com/questions/3125327/how-to-integrate-qt4-qm-files-into-binary-using-cmake-and-qrc
SET( trans_file translations_cmake )
SET( trans_srcfile ${CMAKE_CURRENT_SOURCE_DIR}/${trans_file}.qrc)
SET( trans_infile ${CMAKE_CURRENT_BINARY_DIR}/${trans_file}.qrc)
SET( trans_outfile ${CMAKE_CURRENT_BINARY_DIR}/qrc_${trans_file}.cxx)
# Copy the QRC file to the output directory
ADD_CUSTOM_COMMAND(
    OUTPUT ${trans_infile}
	COMMAND ${CMAKE_COMMAND} -E copy ${trans_srcfile} ${trans_infile}
	MAIN_DEPENDENCY ${trans_srcfile}
)
# Run the resource compiler (rcc_options should already be set)
ADD_CUSTOM_COMMAND(
	OUTPUT ${trans_outfile}
	COMMAND ${QT_RCC_EXECUTABLE}
	ARGS ${rcc_options} -name ${trans_file} -o ${trans_outfile}
	${trans_infile}
	MAIN_DEPENDENCY ${trans_infile}
	DEPENDS ${QM_FILES}
)



add_executable(annual ${annual_SRCS} ${annual_MOC_CPP} ${annual_RES_CPP} ${trans_outfile})
target_link_libraries(annual ${QT_LIBRARIES})

install(TARGETS annual DESTINATION bin)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config.h.in" "${CMAKE_CURRENT_BINARY_DIR}/config.h")



