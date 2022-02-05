#pragma once
#include "Tensor.h"
#include "Python_headers.h"
#include "Python_variable.h"
#include "Autograd.h"
#include <iostream>
#include <sstream>
// extern PyTypeObject* TensorType;
struct PyTensor
{
    PyObject_HEAD
    Tensor* cdata=nullptr;
};
struct TensorMeta {
  PyHeapTypeObject base;
};
static Tensor* unpackTensor(PyObject* obj)
{
    return ((PyTensor*)obj)->cdata;
}
static PyObject* packTensor(Tensor* tensor,PyTypeObject *type)
{
    PyObject* obj = type->tp_alloc(type, 0);
    
    if (obj) {
        auto v = (PyTensor*) obj;
        v->cdata = tensor;
    }
    return obj;
}
static PyObject *tensor_new(PyTypeObject *type, PyObject *args, PyObject *kwargs)
{
    // ::std::cerr<<"call tensor new 1"<<::std::endl;
    std::vector<double> data(15,1.0);
    std::vector<int> shapes{3,5};
    // std::cout<<"shapre size:"<<shapes.size()<<std::endl;
    Tensor* tensor= new Tensor(shapes,std::move(data));
    // std::cout<<"return new:"<<tensor->size()<<std::endl;;
    return packTensor(tensor, type);
}
inline PyObject *ReturnTensor(Tensor *tensor, PyTypeObject *type){
    return packTensor(tensor,type);
}
static PyObject * tensor_add(PyObject* self, PyObject* args,PyObject* unused)
{
    Tensor* tensor_self = unpackTensor(self);
    assert((args ? PyTuple_GET_SIZE(args) : 0)==1);
    auto other = PyTuple_GET_ITEM(args,0);
    Tensor* tensor_other = unpackTensor(other);
    // std::cout<<tensor_str(self)
    assert(tensor_self->size()==tensor_other->size());
    int size = tensor_self->size();
    auto& self_data = tensor_self->get();
    auto& other_data = tensor_other->get();
    std::vector<double> data(size,0);
    for (int i=0;i<size;i++)
        data[i] = self_data[i]+other_data[i];
    bool require_grad = tensor_self->require_grad()||tensor_other->require_grad();
    Tensor* res = new Tensor(tensor_self->shapes(),std::move(data),require_grad);
    if (require_grad)
    {
        res->insertEdge(new AddBackward(tensor_self->getGradMeta(),tensor_other->getGradMeta()));
    }
    return ReturnTensor(res, self->ob_type);
}

static void tensor_destruct(PyObject* obj) {
    // ::std::cerr<<"call destruct"<<::std::endl;
    // PyObject* obj = type->tp_alloc(type, 0);
    // if (obj)
    Tensor* tensor = unpackTensor(obj);
    delete(tensor);
    
    Py_TYPE(obj)->tp_free(obj);
    // ::std::cerr<<"end call deconstruct"<<::std::endl;
    // return 1;
}

static int tensor_clear(PyTensor* self) {
    ::std::cerr<<"call clear"<<::std::endl;
    delete (self->cdata);

    return 1;
}

static PyObject *tensor_str(PyObject *obj)
{
    Tensor *tensor = unpackTensor(obj);
    std::stringstream ss;
    ss << tensor->toString();
    // auto& data = tensor->get();
    // std::cout<<"tensor.size:"<<tensor->size()<<std::endl;
    // for (int i=0;i<tensor->size();i++)
    //     ss<<data[i]<<" ";
    // ss << tensor->get();
    return Py_BuildValue("s", ss.str().c_str());
}

static PyObject *tensor_backward(PyObject *obj)
{
    std::cout<<"begin backward"<<std::endl;
    Tensor *tensor = unpackTensor(obj);
    backward(tensor->getGradMeta());
    Py_RETURN_NONE;
}