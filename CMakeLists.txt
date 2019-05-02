cmake_minimum_required (VERSION 3.8)

include(QtHelpers.cmake)
find_package(Qt5Widgets CONFIG REQUIRED)

add_subdirectory(pybind11)


set(sources src/CutiePy.hpp
            src/CutiePy.cpp
            src/Example.cpp
   )

pybind11_add_module(cmake_example src/Example.cpp)

add_executable (CutiePy ${sources})
target_link_libraries(CutiePy PUBLIC Qt5::Widgets)
target_link_libraries(CutiePy PUBLIC pybind11::embed)

DeployQt(CutiePy)
