# -------------------------------------------------
# Project created by QtCreator 2011-06-06T10:56:16
# -------------------------------------------------
TARGET = annual
TEMPLATE = app
CONFIG += qt release warn_on

SOURCES += \
aboutdialog.cpp  card.cpp          listdelegate.cpp  main_cli.cpp  mainwindow.cpp      tabledelegate.cpp  tableview.cpp    wizardpages.cpp \
anniv.cpp        cardinserter.cpp  listmodel.cpp     main.cpp      settingsdialog.cpp  tablemodel.cpp     undocommand.cpp

HEADERS +=  \
aboutdialog.h  card.h          config.h       listdelegate.h  mainwindow.h      tabledelegate.h  tableview.h    wizardpages.h \
anniv.h        cardinserter.h  ioexception.h  listmodel.h     settingsdialog.h  tablemodel.h     undocommand.h
FORMS += 
RESOURCES += annual.qrc

TRANSLATIONS += translations/annual_en.ts

QMAKE_CXXFLAGS = -ggdb -O0 -Wextra -Wall
