CMAKE_MINIMUM_REQUIRED(VERSION 3.18.0)

project(test_opengl_copy LANGUAGES C CXX)


set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set to OFF if using something else
set(USING_QT ON)

#set to ON if using GLFW
set(USING_GLFW OFF)


find_package(Freetype REQUIRED)
find_package(OpenGL REQUIRED)


if(USING_QT)
        find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Widgets OpenGL OpenGLWidgets REQUIRED)
	find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
        find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Gui REQUIRED)
	find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
        find_package(Qt${QT_VERSION_MAJOR} COMPONENTS OpenGL REQUIRED)
        find_package(Qt${QT_VERSION_MAJOR} COMPONENTS OpenGLWidgets REQUIRED)
endif()


# specify the C++ standard
add_library(${PROJECT_NAME}_compiler_flags INTERFACE)
add_library(${PROJECT_NAME}_compiler_flags_c INTERFACE)
add_library(${PROJECT_NAME}_compiler_flags_cxx INTERFACE)

target_link_libraries(${PROJECT_NAME}_compiler_flags INTERFACE ${PROJECT_NAME}_compiler_flags_c)
target_link_libraries(${PROJECT_NAME}_compiler_flags INTERFACE ${PROJECT_NAME}_compiler_flags_cxx)
target_compile_features(${PROJECT_NAME}_compiler_flags INTERFACE cxx_std_11)


get_filename_component(PARENT_PATH
                       "${CMAKE_CURRENT_LIST_DIR}"
                       ABSOLUTE
)


set(CMAKE_BINARY_DIR "${CMAKE_BINARY_DIR}/build")


add_library(openglplot
    "${CMAKE_CURRENT_LIST_DIR}/sources/openglplot/openglplot.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/sources/openglplot/openglplot.h"
)


target_include_directories(openglplot PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/sources/openglplot"
    ${FREETYPE_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS})


target_link_libraries(openglplot PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::OpenGL
    Qt${QT_VERSION_MAJOR}::OpenGLWidgets
    ${OPENGL_LIBRARIES}
    ${FREETYPE_LIBRARIES}
)


#insert your files and directories names here
set(SOURCES
	"${PARENT_PATH}/sources/mainwindow.cpp"
        "${PARENT_PATH}/app/main.cpp"
)

set(HEADER
    "${PARENT_PATH}/sources/mainwindow.h"
) 
    
set(INCLUDE_PATH
    "${PARENT_PATH}/sources"
    "${PARENT_PATH}/app"
)


set(EXTRA_APP_PARAM)
if(WIN32)
  set(EXTRA_APP_PARAM WIN32)
endif()


add_executable(${PROJECT_NAME} ${EXTRA_APP_PARAM} ${SOURCES} ${HEADER})


target_include_directories(${PROJECT_NAME} PUBLIC "${PARENT_PATH}/sources" "${PARENT_PATH}/app" ${FREETYPE_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS})


if(NOT USING_QT)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${PROJECT_NAME}_compiler_flags
        openglplot
    )
endif()


if(USING_QT)
  target_link_libraries(${PROJECT_NAME} PRIVATE
        ${PROJECT_NAME}_compiler_flags
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::OpenGL
        Qt${QT_VERSION_MAJOR}::OpenGLWidgets
        openglplot
        ${OPENGL_LIBRARIES}
        ${FREETYPE_LIBRARIES}
  )
endif()

