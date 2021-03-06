cmake_minimum_required(VERSION 2.8)
project(stromx-zbar)

SET(GCC_DEBUG_FLAGS "-g")

# http://stackoverflow.com/a/11437693
# Add a coverage build type
set(CMAKE_CXX_FLAGS_COVERAGE
    "${GCC_DEBUG_FLAGS} --coverage"
    CACHE STRING "Flags used by the C++ compiler during coverage builds."
    FORCE )
set(CMAKE_C_FLAGS_COVERAGE
    "${GCC_DEBUG_FLAGS} --coverage"
    CACHE STRING "Flags used by the C compiler during coverage builds."
    FORCE )
set(CMAKE_EXE_LINKER_FLAGS_COVERAGE
    ""
    CACHE STRING "Flags used for linking binaries during coverage builds."
    FORCE )
set(CMAKE_SHARED_LINKER_FLAGS_COVERAGE
    ""
    CACHE STRING "Flags used by the shared libraries linker during coverage builds."
    FORCE )
mark_as_advanced(
    CMAKE_CXX_FLAGS_COVERAGE
    CMAKE_C_FLAGS_COVERAGE
    CMAKE_EXE_LINKER_FLAGS_COVERAGE
    CMAKE_SHARED_LINKER_FLAGS_COVERAGE )

set(STROMX_ZBAR_VERSION_MAJOR 0)
set(STROMX_ZBAR_VERSION_MINOR 1)
set(STROMX_ZBAR_VERSION_PATCH 0)

enable_testing()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)
set(LIB_DIR lib${LIB_SUFFIX})

if(CMAKE_COMPILER_IS_GNUCXX)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -std=c++0x -pedantic -Wall -Wextra")
endif(CMAKE_COMPILER_IS_GNUCXX)

if(MSVC)
    add_definitions(/DBOOST_ALL_DYN_LINK)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251 /wd4275 /wd4661")
endif(MSVC)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

find_package(stromx REQUIRED)
find_library(zbar_LIB "zbar")
find_path(zbar_DIR "zbar.h")

if(NOT zbar_DIR)
    message(SEND_ERROR "Can not find ZBar include directory")
endif()

if(NOT zbar_LIB)
    message(SEND_ERROR "Can not find ZBar library")
endif()

find_package(CppUnit)

if(CPPUNIT_FOUND)
    option(BUILD_TESTS "Build unit tests" ON)
endif()

if(WIN32)
    set(LOCALE_SUFFIX "locale")
else(WIN32)
    set(LOCALE_SUFFIX "share/locale")
endif(WIN32)

set(LOCALE_DIR "${CMAKE_INSTALL_PREFIX}/${LOCALE_SUFFIX}")

add_subdirectory(stromx)

