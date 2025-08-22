QT += core gui widgets multimedia

CONFIG += c++17

INCLUDEPATH += \
    util

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    util/aspectratiopixmaplabel.cpp \
    util/filedialog.cpp \
    util/globalsetting.cpp

HEADERS += \
    mainwindow.h \
    util/aspectratiopixmaplabel.h \
    util/filedialog.h \
    util/globalsetting.h

FORMS += \
    mainwindow.ui \
    util/filedialog.ui
