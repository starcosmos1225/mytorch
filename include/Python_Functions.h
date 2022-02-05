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
    bool require_grad=false;
    if (kwargs)
    {
        PyObject* python_require_grad = nullptr;
        python_require_grad = PyDict_GetItemString(kwargs,"requires_grad");
        if (python_require_grad  &&
         PyObject_IsTrue(python_require_grad))
        {
            require_grad = true;
        }
    }
    Dispatcher dispatcher = Dispatcher::singleton();
    KernelFunctorPtr kernel = dispatcher.getFunctor("randn");
    if (!kernel)
    {
        Py_RETURN_NONE;
    }
    Tensor* tensor = kernel->call<Tensor*>(shapes,require_grad);
    
    return packTensor(tensor,getTensorType());
}

static PyObject *THPVariable_sum(PyObject* self, PyObject* args, PyObject* kwargs)
{
    assert((args ? PyTuple_GET_SIZE(args) : 0)==1);
    auto python_tensor = PyTuple_GET_ITEM(args,0);
    Tensor *tensor = unpackTensor(python_tensor);
    Dispatcher dispatcher = Dispatcher::singleton();
    KernelFunctorPtr kernel = dispatcher.getFunctor("sum");
    if (!kernel)
    {
        Py_RETURN_NONE;
    }
    Tensor* out = kernel->call<Tensor*>(tensor);
    if (out->require_grad())
    {
        out->insertEdge(new SumBackward(tensor->getGradMeta(),
                                        out->getGradMeta()));
    }
    return packTensor(out,getTensorType());
}