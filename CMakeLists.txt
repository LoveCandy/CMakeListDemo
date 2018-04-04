# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.11)

PROJECT(libbtcapi)
set(INC_DIR ${PROJECT_SOURCE_DIR}/include)
set(LINK_DIR ${PROJECT_SOURCE_DIR}/lib)
set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

include_directories(${INC_DIR}) # 用${}引用变量

link_directories(${LINK_DIR})

MESSAGE (STATUS "SSS:" ${INC_DIR})

#SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output)

aux_source_directory(${SOURCE_DIR} SOURCE_FILES)
MESSAGE (STATUS "SSS:" ${SOURCE_FILES})


add_library(libbtcapi SHARED ${SOURCE_FILES})

target_link_libraries(libbtcapi  libcurl)

#add_executable(libbtcapi  ${SOURCE_FILES})

#ADD_LIBRARY(btcapi SHARED ${SOURCE_FILES})


