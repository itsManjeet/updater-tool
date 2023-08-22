cmake_minimum_required(VERSION 3.10)
project(updater-tool
    VERSION "0.0.1"
    DESCRIPTION "OStree based updater tool"
    LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(INSERT CMAKE_MODULE_PATH 0 ${PROJECT_SOURCE_DIR}/cmake)

find_package(OSTree)
find_package(DBus)
find_package(GLib)

add_subdirectory(src)