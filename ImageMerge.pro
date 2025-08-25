QT += core gui widgets multimedia

CONFIG += c++17

INCLUDEPATH += \
    util

SOURCES += \
    imagemerge.cpp \
    main.cpp \
    mainwindow.cpp \
    util/aspectratiopixmaplabel.cpp \
    util/filedialog.cpp \
    util/globalsetting.cpp

HEADERS += \
    imagemerge.h \
    mainwindow.h \
    util/aspectratiopixmaplabel.h \
    util/filedialog.h \
    util/globalsetting.h

FORMS += \
    mainwindow.ui \
    util/filedialog.ui
