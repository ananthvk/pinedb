cmake_minimum_required(VERSION 3.14...3.22)

# ---- Project ----

# Note: update this to your new project's name and version
project(
  PineDB
  VERSION 1.0
  LANGUAGES CXX
)

# ---- Include guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

# ---- Add dependencies via CPM ----
# see https://github.com/TheLartians/CPM.cmake for more info

include(cmake/CPM.cmake)

# PackageProject.cmake will be used to make our target installable
CPMAddPackage("gh:TheLartians/PackageProject.cmake@1.8.0")

CPMAddPackage(
  NAME fmt
  GIT_TAG 10.2.1
  GITHUB_REPOSITORY fmtlib/fmt
  OPTIONS "FMT_INSTALL YES" # create an installable target
)

CPMAddPackage(
  GITHUB_REPOSITORY gabime/spdlog
  VERSION 1.14.1
  OPTIONS "SPDLOG_FMT_EXTERNAL 1"
  OPTIONS "SPDLOG_INSTALL YES"
)

# ---- Add source files ----
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Note: globbing sources is considered bad practice as CMake's generators may not detect new files
# automatically. Keep that in mind when changing files, or explicitly mention them here.
set(headers
    ${CMAKE_CURRENT_SOURCE_DIR}/include/pinedb/bufferpool.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/pinedb/cachereplacer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/pinedb/common.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/pinedb/config.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/pinedb/filehandle.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/pinedb/pinedb.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/pinedb/storage.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/pinedb/page.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/pinedb/btree.h
    "${CMAKE_CURRENT_SOURCE_DIR}/include/pinedb/command.h"
    ${CMAKE_CURRENT_SOURCE_DIR}/include/pinedb/command_registry.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/pinedb/interpreter.h
)
set(sources
    ${CMAKE_CURRENT_SOURCE_DIR}/source/bufferpool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/pinedb.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/storage.cpp
    "${CMAKE_CURRENT_SOURCE_DIR}/source/command.cpp"
    ${CMAKE_CURRENT_SOURCE_DIR}/source/command_registry.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/interpreter.cpp
)

# ---- Create library ----

# Note: for header-only libraries change all PUBLIC flags to INTERFACE and create an interface
# target: add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME} ${headers} ${sources})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

# being a cross-platform target, we enforce standards conformance on MSVC
if(MSVC)
  add_compile_options(/W4)
  target_compile_options(${PROJECT_NAME} PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->")
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Link dependencies
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)

target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                         $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
)

# ---- Create an installable target ----
# this allows users to install and find the library via `find_package()`.

# the location where the project's version header will be placed should match the project's regular
# header paths
string(TOLOWER ${PROJECT_NAME}/version.h VERSION_HEADER_LOCATION)

packageProject(
  NAME ${PROJECT_NAME}
  VERSION ${PROJECT_VERSION}
  NAMESPACE ${PROJECT_NAME}
  BINARY_DIR ${PROJECT_BINARY_DIR}
  INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include
  INCLUDE_DESTINATION include/${PROJECT_NAME}-${PROJECT_VERSION}
  VERSION_HEADER "${VERSION_HEADER_LOCATION}"
  COMPATIBILITY SameMajorVersion
  DEPENDENCIES "fmt 9.1.0;spdlog 1.14.1"
)
