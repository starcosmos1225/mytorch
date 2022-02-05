#include "Module.h"
#include "Python_variable.h"
#include <iostream>
static PyObject * test(PyObject* self_, PyObject* args)
{
    std::cout<<"test pass"<<std::endl;
    Py_RETURN_NONE;
}

PyObject* initModule() {
    static PyMethodDef methods[] = {
    {"test",  test,  METH_NOARGS ,       nullptr},
    {NULL}
    };
  static struct PyModuleDef torchmodule = {
     PyModuleDef_HEAD_INIT,
     "torch._C",
     nullptr,
     -1,
     methods
  };
//   auto module = PyModule_Create(&torchmodule);
  return PyModule_Create(&torchmodule);
}

PyMODINIT_FUNC PyInit__C(void)
{
    PyObject *m;
    // std::cout<<"call init"<<std::endl;
    m = initModule();
    initFunctions(m);
    initTensor(m);
    return m;
}