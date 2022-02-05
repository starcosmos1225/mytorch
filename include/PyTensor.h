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
    assert(tensor_self->size()==tensor_other->size());
    int size = tensor_self->size();
    auto& self_data = tensor_self->data();
    auto& other_data = tensor_other->data();
    std::vector<double> data(size,0);
    for (int i=0;i<size;i++)
        data[i] = self_data[i]+other_data[i];
    bool require_grad = tensor_self->require_grad()||tensor_other->require_grad();
    Tensor* res = new Tensor(tensor_self->shapes(),std::move(data),require_grad);
    if (require_grad)
    {
        res->insertEdge(new AddBackward(tensor_self->getGradMeta(),
                                        tensor_other->getGradMeta(),
                                        res->getGradMeta()));
    }
    return ReturnTensor(res, self->ob_type);
}

static void tensor_destruct(PyObject* obj) {

    Tensor* tensor = unpackTensor(obj);
    delete(tensor);
    
    Py_TYPE(obj)->tp_free(obj);
}

static int tensor_clear(PyTensor* self) {
    delete (self->cdata);

    return 1;
}

static PyObject *tensor_str(PyObject *obj)
{
    Tensor *tensor = unpackTensor(obj);
    std::stringstream ss;
    ss << tensor->toString();
    return Py_BuildValue("s", ss.str().c_str());
}

static PyObject *tensor_backward(PyObject *obj)
{
    Tensor *tensor = unpackTensor(obj);
    AutogradMeta* node = tensor->getGradMeta();
    std::vector<double>& grad = node->grad();
    grad = std::vector<double>(tensor->size(),1.0);
    // std::cout<<"begin call backward"<<std::endl;
    backward(node);
    Py_RETURN_NONE;
}

static PyObject *tensor_sigmoid(PyObject *obj)
{
    // std::cout<<"begin sigmoid"<<std::endl;
    Tensor *tensor = unpackTensor(obj);
    Dispatcher dispatcher = Dispatcher::singleton();
    KernelFunctorPtr kernel = dispatcher.getFunctor("sigmoid");
    if (!kernel)
    {
        Py_RETURN_NONE;
    }
    Tensor* out = kernel->call<Tensor*>(tensor);
    if (out->require_grad())
    {
        out->insertEdge(new SigmoidBackward(tensor->getGradMeta(),
                                        out->getGradMeta()));
    }
    // std::cout<<"end sigmoid"<<std::endl;
    return packTensor(out,getTensorType());
}

static PyObject * tensor_getgrad(PyTensor *self, void *closure)
{
    // std::cout<<"call grad"<<std::endl;
    AutogradMeta* gradMeta = self->cdata->getGradMeta();
    std::string res = gradMeta->toString();
    std::cout<<res<<std::endl;
    // Py_INCREF(self->first);
    return Py_BuildValue("s", res.c_str());
}

