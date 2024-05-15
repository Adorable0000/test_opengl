CMAKE_MINIMUM_REQUIRED(VERSION 3.18.0)

project(test_opengl LANGUAGES C CXX)


set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Gui REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)


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


add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/sources/openglplot)


#set(CMAKE_BINARY_DIR "${CMAKE_BINARY_DIR}/build")


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


target_include_directories(${PROJECT_NAME} PUBLIC
    "${PARENT_PATH}/sources"
    "${PARENT_PATH}/app"
)

find_library(OPENGLPLOT openglplot PATH_SUFFIXES openglplot)
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${PROJECT_NAME}_compiler_flags
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    openglplot
)


file(COPY "${PARENT_PATH}/sources/openglplot/DejaVuMathTeXGyre.ttf" DESTINATION ${CMAKE_BINARY_DIR})
file(COPY "${PARENT_PATH}/sources/openglplot/Microsoft_JhengHei_UI_high_res.bff" DESTINATION ${CMAKE_BINARY_DIR})

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
