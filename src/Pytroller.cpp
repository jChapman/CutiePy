#include "Pytroller.hpp"

#include <pybind11/pybind11.h>

#include <iostream>

namespace py = pybind11;

void Something::doSomething(std::string something) {
    std::cout << something;
}

PYBIND11_MODULE(example, m) {
    m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------
        .. currentmodule:: cmake_example
        .. autosummary::
           :toctree: _generate
           add
           subtract
    )pbdoc";

    m.def("subtract", [](int i, int j) { return i - j; }, R"pbdoc(
        Subtract two numbers
        Some other explanation about the subtract function.
    )pbdoc");

    py::class_<Something> something(m, "Something");
    something.def("doSomething", &Something::doSomething);
}
