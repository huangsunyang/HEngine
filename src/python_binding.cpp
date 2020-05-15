#include <pybind11/embed.h>
#include "LogManager.hpp"
#include <string>

namespace py = pybind11;

void test_binding_python() {
    py::scoped_interpreter guard{};
    py::module sys = py::module::import("sys");
    py::print(sys.attr("path"));
}