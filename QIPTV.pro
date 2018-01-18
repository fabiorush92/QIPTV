QT += core gui widgets network

TARGET = QIPTV
VERSION = 1.0.0

TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS QT_MESSAGELOGCONTEXT

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    panelmanager.cpp \
    common/qmydialog.cpp \
    panelform.cpp \
    panelrepository.cpp

HEADERS += \
    mainwindow.h \
    panelmanager.h \
    common/syslogmessagehandler.h \
    common/qmydialog.h \
    panelform.h \
    panelrepository.h \
    paneltypes.h

FORMS += \
    mainwindow.ui \
    panelform.ui
