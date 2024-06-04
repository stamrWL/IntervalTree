#include "pybind11/pybind11.h"
#include "intervalTree.h"

namespace py = pybind11;

PYBIND11_MODULE(intervalTree, m) {
    py::class_<IntervalTree>(m, "IntervalTree")
    .def(py::init<double,double,double>())
    .def("addValue",&IntervalTree::addValue)
    .def("getValue",&IntervalTree::getValue)
    .def("releaseLeft",&IntervalTree::releaseLeft)
    .def("intoNextWindows",static_cast<void (IntervalTree::*)()>(&IntervalTree::intoNextWindows))
    .def("intoNextWindows_D",static_cast<void (IntervalTree::*)(double)>(&IntervalTree::intoNextWindows))
    .def("changeDefualtValue",&IntervalTree::changeDefualtValue)
    .def("AllocatedArea_DD",&IntervalTree::AllocatedArea_DD)
    .def("AllocatedArea_DDD",&IntervalTree::AllocatedArea_DDD)
    .def("viewList",&IntervalTree::viewList);
    // py::register_exception<CppExp>(module, "PyExp");
}