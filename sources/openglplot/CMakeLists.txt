CMAKE_MINIMUM_REQUIRED(VERSION 3.18.0)

project(openglplot LANGUAGES C CXX)


set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



if(UNIX)
    find_package(Freetype REQUIRED)
endif()
find_package(OpenGL REQUIRED)


find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Widgets OpenGL OpenGLWidgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Gui REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS OpenGL REQUIRED)
if(${QT_VERSION_MAJOR} MATCHES "6")
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS OpenGLWidgets REQUIRED)
endif()


add_library(openglplot
    "${CMAKE_CURRENT_LIST_DIR}/openglplot.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/openglplot.h"
)


# specify the C++ standard
add_library(${PROJECT_NAME}_compiler_flags INTERFACE)
add_library(${PROJECT_NAME}_compiler_flags_c INTERFACE)
add_library(${PROJECT_NAME}_compiler_flags_cxx INTERFACE)

target_link_libraries(${PROJECT_NAME}_compiler_flags INTERFACE ${PROJECT_NAME}_compiler_flags_c)
target_link_libraries(${PROJECT_NAME}_compiler_flags INTERFACE ${PROJECT_NAME}_compiler_flags_cxx)
target_compile_features(${PROJECT_NAME}_compiler_flags INTERFACE cxx_std_11)


if(WIN32)
	if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
                set(FREETYPE_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/freetype-2.13.2/include")
                set(FREETYPE_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/freetype-2.13.2/win64/freetype.lib")
	elseif(${CMAKE_SIZEOF_VOID_P} EQUAL 4)
		set(FREETYPE_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/freetype-2.13.2/include")
		set(FREETYPE_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/freetype-2.13.2/win32/freetype.lib")
	endif()
endif()

#if(WIN32)
#    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/freetype-2.13.2")
#    find_library(Freetype freetype PATH_SUFFIXES freetype)
#endif()


target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${FREETYPE_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
)


target_link_libraries(${PROJECT_NAME} PUBLIC
    openglplot_compiler_flags
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::OpenGL
    ${OPENGL_LIBRARIES}
    ${FREETYPE_LIBRARIES}
)

#if(WIN32)
#    target_link_libraries(${PROJECT_NAME} PUBLIC
#        freetype)
#endif()

if(${QT_VERSION_MAJOR} MATCHES "6")
target_link_libraries(${PROJECT_NAME} PUBLIC
        Qt${QT_VERSION_MAJOR}::OpenGLWidgets
        )
endif()


set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
target_compile_options(${PROJECT_NAME}_compiler_flags_cxx INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wattributes;-Wshadow;-Wno-system-headers;-Wno-deprecated;-Woverloaded-virtual;-Wwrite-strings;-Wunused;-Wunused-variable;-Wunused-parameter;-Wunused-function;-Wcast-align;-Wold-style-cast;-Wpedantic;-Wuninitialized;-ffunction-sections;-fdata-sections;>>"

  # Set build parameters:
  #         Debug
  #         Release
  #         MinSizeRel
  #         RelWithDebInfo
  "$<$<CONFIG:Debug>:$<${gcc_like_cxx}:$<BUILD_INTERFACE:-O0;-DDEBUG;>>>"
  "$<$<CONFIG:Release>:$<${gcc_like_cxx}:$<BUILD_INTERFACE:-O2;-DNDEBUG;>>>"
  "$<$<CONFIG:MinSizeRel>:$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Os;-DNDEBUG;>>>"
  "$<$<CONFIG:RelWithDebInfo>:$<${gcc_like_cxx}:$<BUILD_INTERFACE:-O2;-DNDEBUG;>>>"
)
