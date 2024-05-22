## Description 

This is just a plain plotter made for real-time plotting with Qt, using OpenGL

## Requirements 

C++11

Qt, version higher than 4

Hardware support for OpenGL version 2.0 or higher

## Usage 

Check if you have FreeType on Linux

```
pkg-config --cflags freetype2
```

If you do have the library, you will get path to it.
If you don't have it, install FreeType

```
sudo apt-get install libfreetype-dev libfreetype6 libfreetype6-dev
```

Check if you have supported OpenGL version

```
glxinfo | grep OpenGL
```

If you don't have it installed, install OpenGL dev files and tools

```
sudo apt-get install libgl1-mesa-dev mesa-utils 
```

When you use qmake, you have to link FreeType and OpenGL manually, for example

```QMake
QT += opengl
CONFIG += opengl
unix: INCLUDEPATH += /usr/include/freetype2
LIBS += -lfreetype
win32: LIBS += -lopengl32
```

You can find example .pro file in folder qmake

When you use cmake (which is recommended), add directory with this library, for example,

```CMake
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/sources/openglplot)
```

Then, you have link this library to your project

```CMake
find_library(OPENGLPLOT openglplot PATH_SUFFIXES openglplot)
target_link_libraries(${PROJECT_NAME} PRIVATE openglplot)
```

And, please, copy font file to your build directory, for example

```CMake
get_filename_component(PARENT_PATH
                       "${CMAKE_CURRENT_LIST_DIR}"
                       ABSOLUTE
)
file(COPY "${PARENT_PATH}/sources/openglplot/DejaVuMathTeXGyre.ttf" DESTINATION ${CMAKE_BINARY_DIR})
```

## TODO 

- Add layouts
	- Add axis
	- Add grid
- Add the ability to put two graphs in one plot
- Add text render
- Improve line smoothing perfomance


