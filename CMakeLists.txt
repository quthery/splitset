cmake_minimum_required(VERSION 3.16)

project(GtkClock C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)

add_executable(clock main.c)

target_link_libraries(clock
    PRIVATE
        PkgConfig::GTK4
        m
)
