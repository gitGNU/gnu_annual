project(annual) # the name of your project
set (APPLICATION_NAME Annual)
set (APPLICATION_VERSION_MAJOR 1)
set (APPLICATION_VERSION_MINOR 0)
set (APPLICATION_VERSION_PATCH 0)
set (APPLICATION_VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_PATCH}")
set (PROJECT_SOURCE_DIR src)
set (HOMEPAGE "www.devwork.org/annual")
set (EMAIL "dominik@devwork.org")

IF(WIN32 AND NOT UNIX)
	set (EXECUTABLE "annual.exe")
ELSE(WIN32 AND NOT UNIX)
	set (EXECUTABLE "annual")
ENDIF(WIN32 AND NOT UNIX)


ENABLE_TESTING()

cmake_minimum_required(VERSION 2.4.0)

find_package(Qt4 REQUIRED) # find and setup Qt4 for this project

# tell cmake to process CMakeLists.txt in that subdirectory
add_subdirectory(${PROJECT_SOURCE_DIR})
add_subdirectory(perl)


#cpack
include (InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE COPYING)
set(CPACK_PACKAGE_NAME ${APPLICATION_NAME})
set(CPACK_PACKAGE_VERSION_MAJOR ${APPLICATION_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${APPLICATION_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${APPLICATION_VERSION_PATCH})
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY
"Keeps track of pending events and reminds you hopefully in right time.
Provided by a list of events stored in a database file, this program sorts all events out,
which will not come up in the next days. If there are remaining days, they
will be displayed either in command-line or in a Qt-Window. 
Moreover, the graphical window supports the edition of the events.
Additionally, you can import various file formats with the perl-script `2annual`.
")
set(CPACK_PACKAGE_VENDOR "Dominik Köppl")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "README")
SET(CPACK_RESOURCE_FILE_LICENSE "COPYING")
set(CPACK_PACKAGE_CONTACT "${CPACK_PACKAGE_VENDOR} <${EMAIL}>")
SET(CPACK_PACKAGE_ICON "${PROJECT_SOURCE_DIR}/icons/program.png")

SET(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")
IF(WIN32 AND NOT UNIX)
  # There is a bug in NSI that does not handle full unix paths properly. Make
    # sure there is at least one set of four (4) backlasshes.
	SET(CPACK_NSIS_INSTALLED_ICON_NAME "${PROJECT_SOURCE_DIR}/${EXECUTABLE}")
	SET(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} ${APPLICATION_NAME}")
	SET(CPACK_NSIS_HELP_LINK "http:\\\\\\\\${HOMEPAGE}")
	SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\${HOMEPAGE}")
	SET(CPACK_NSIS_CONTACT ${EMAIL})
	SET(CPACK_NSIS_MODIFY_PATH ON)
ELSE(WIN32 AND NOT UNIX)
	SET(CPACK_STRIP_FILES "${PROJECT_SOURCE_DIR}/${EXECUTABLE}")
	SET(CPACK_SOURCE_STRIP_FILES "")
ENDIF(WIN32 AND NOT UNIX)
	SET(CPACK_PACKAGE_EXECUTABLES ${EXECUTABLE})

#debian
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt4, perl, libxml-xpath-perl, liblog-log4perl-perl")
set(CPACK_DEBIAN_PACKAGE_SECTION "editors")
set(CPACK_DEBIAN_PACKAGE_RECOMMENDS "")
set(CPACK_DEBIAN_PACKAGE_SUGGESTS "")
set(DEBIAN_PACKAGE_BUILDS_DEPENDS "debhelper (>= 7), g++, libqt4-dev")

#rpm
set(CPACK_RPM_PACKAGE_LICENSE "GPL")
set(CPACK_RPM_PACKAGE_GROUP "Applications/Databases")
set(CPACK_RPM_PACKAGE_REQUIRES "qt")


#osx
set(CPACK_BUNDLE_ICON ${CPACK_PACKAGE_ICON})

include (CPack)
