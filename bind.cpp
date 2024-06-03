#include "pybind11/pybind11.h"
#include "intervalTree.h"

namespace py = pybind11;

PYBIND11_MODULE(intervalTree, m) {
    py::class_<IntervalTree>(m, "IntervalTree")
    .def(py::init<double,double,double>())
    .def("extend_DD",static_cast<void (IntervalTree::*)(double)>(&IntervalTree::extend))
    .def("extend_DDD",static_cast<void (IntervalTree::*)(double,double)>(&IntervalTree::extend))
    .def("addValue",&IntervalTree::addValue)
    .def("getValue",&IntervalTree::getValue)
    .def("intoNextWindows",&IntervalTree::intoNextWindows)
    .def("AllocatedArea_DD",&IntervalTree::AllocatedArea_DD)
    .def("AllocatedArea_DDD",&IntervalTree::AllocatedArea_DDD)
    .def("viewList",&IntervalTree::viewList);
    // py::register_exception<CppExp>(module, "PyExp");
}