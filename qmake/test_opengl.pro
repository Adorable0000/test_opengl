QT += core gui
QT += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test_opengl
TEMPLATE = app

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

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

INCLUDEPATH += ../sources
INCLUDEPATH += ../sources/openglplot
INCLUDEPATH += ../app

unix: INCLUDEPATH += /usr/include/freetype2
win32 {
    INCLUDEPATH += /souces/openglplot/freetype-2.13.2/include
    !contains(QMAKE_TARGET.arch, x86_64) {
        ## Windows x86 (32bit) specific build here
        LIBS += ../sources/openglplot/freetype-2.13.2/win32/freetype.lib
    } else {
        ## Windows x64 (64bit) specific build here
        LIBS += ../sources/openglplot/freetype-2.13.2/win64/freetype.lib
    }
}
LIBS += -lfreetype

win32: LIBS += -lopengl32

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
