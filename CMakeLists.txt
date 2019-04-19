cmake_minimum_required (VERSION 3.8)

# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)
# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)

set(sources src/CutiePy.hpp
            src/CutiePy.cpp
   )

add_executable (CutiePy ${sources})
target_link_libraries(CutiePy PUBLIC Qt5::Widgets)
