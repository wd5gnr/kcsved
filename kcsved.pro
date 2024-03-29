#-------------------------------------------------
#
# Project created by QtCreator 2012-01-25T19:21:48
#
#-------------------------------------------------

QT       += core gui webkit sql

TARGET = kcsved
TEMPLATE = app


SOURCES += main.cpp\
    editrow.cpp \
    csvmodel.cpp \
    editwindow.cpp \
    qcsvlineedit.cpp \
    qcsvpushbutton.cpp \
    kcsvedmain.cpp \
    selector.cpp \
    optdialog.cpp \
    browseview.cpp \
    recentfilemgr.cpp

HEADERS  += \
    editrow.h \
    csvmodel.h \
    editwindow.h \
    qcsvlineedit.h \
    qcsvpushbutton.h \
    kcsvedmain.h \
    selector.h \
    optdialog.h \
    browseview.h \
    recentfilemgr.h \
    dbdoc.h

FORMS    += \
    editwindow.ui \
    kcsvedmain.ui \
    selector.ui \
    optdialog.ui \
    browseview.ui


RESOURCES += \
    csved.qrc

OTHER_FILES += \
    todo.txt
