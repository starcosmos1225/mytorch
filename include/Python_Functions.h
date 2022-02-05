#pragma once
#include "Python_headers.h"
#include <iostream>
#include "Dispatcher.h"
#include "Tensor.h"
#include "PyTensor.h"
#include "KernelFunctor.h"
#include "Python_variable.h"
#include <exception>
static PyObject* THPVariable_randn(PyObject* self, PyObject* args, PyObject* kwargs)
{
    unsigned int nargs = args ? PyTuple_GET_SIZE(args) : 0;
    std::cout<<"nargs:"<<nargs<<std::endl;
    assert((args ? PyTuple_GET_SIZE(args) : 0)==1);
    auto python_shapes = PyTuple_GET_ITEM(args,0);
    assert(PyTuple_Check(python_shapes) || PyList_Check(python_shapes));
    auto tuple = PyTuple_Check(python_shapes);
    const auto size = tuple ? PyTuple_GET_SIZE(python_shapes) : PyList_GET_SIZE(python_shapes);
    std::vector<int> shapes(size);
    for(int i=0;i<size;i++) {
        PyObject* obj = tuple ? PyTuple_GET_ITEM(python_shapes, i) : PyList_GET_ITEM(python_shapes, i);
        if (PyLong_Check(obj))
        {
            auto shape = PyLong_AsLong(obj);
            shapes[i] = shape;
        }
    }
    std::cout<<"here"<<nargs<<std::endl;
    bool require_grad=false;
    if (kwargs)
    {
        PyObject* python_require_grad = nullptr;
        python_require_grad = PyDict_GetItemString(kwargs,"require_grad");
        if (python_require_grad  &&
         PyObject_IsTrue(python_require_grad))
        {
            require_grad = true;
        }
    }
    std::cout<<"dispatcher"<<nargs<<std::endl;
    Dispatcher dispatcher = Dispatcher::singleton();
    std::cout<<"dispatcher1"<<nargs<<std::endl;
    KernelFunctorPtr kernel = dispatcher.getFunctor("randn");
    std::cout<<"dispatcher2"<<nargs<<std::endl;
    if (!kernel)
    {
        Py_RETURN_NONE;
    }
    std::cout<<"dispatcher3"<<nargs<<std::endl;
    Tensor* tensor = kernel->call<Tensor*>(shapes,require_grad);
    
    std::cout<<"tensor->size:"<<tensor->size()<<std::endl;
    return packTensor(tensor,getTensorType());
}