QT += core gui
QT += opengl
QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test_opengl
TEMPLATE = app

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TRANSLATIONS += \
    test_opengl_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations
CONFIG += opengl

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_DEBUG += -O0
QMAKE_CXXFLAGS_RELEASE += -O3

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += $$files("../sources/*.cpp", true)
HEADERS += $$files("../sources/*.h", true)
SOURCES += $$files("../app/*.cpp", true)
TRANSLATIONS += $$files("../test_opengl/*.ts", true)

INCLUDEPATH += ../sources
INCLUDEPATH += ../app

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
