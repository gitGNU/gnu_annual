# -------------------------------------------------
# Project created by QtCreator 2011-06-06T10:56:16
# -------------------------------------------------
TARGET = annual
TEMPLATE = app
#CONFIG += qt release warn_on

SOURCES += \
aboutdialog.cpp anniv.cpp card.cpp cardinserter.cpp dockwidget.cpp listdelegate.cpp listmodel.cpp main_cli.cpp main.cpp mainwindow.cpp settingsdialog.cpp tabledelegate.cpp tablemodel.cpp tableview.cpp tabwidget.cpp undocommand.cpp wizardpages.cpp

HEADERS +=  \
aboutdialog.h cardinserter.h dockwidget.h listmodel.h mainwindow.h settingsdialog.h tablemodel.h tabwidget.h wizardpages.h
FORMS += 
RESOURCES += annual.qrc translations.qrc

TRANSLATIONS += translations/annual_en_US.ts translations/annual_de_DE.ts

QMAKE_CXXFLAGS = -ggdb -O0 -Wextra -Wall
