#include "Python_variable.h"
#include "Python_Functions.h"
#include "Exceptions.h"
#include <iostream>
#include <vector>
#include "PyTensor.h"
inline PyCFunction castPyCFunctionWithKeywords(PyCFunctionWithKeywords func) {
  // NOLINTNEXTLINE(modernize-redundant-void-arg)
  return (PyCFunction)(void(*)(void))func;
}

PyObject* TensorTypeModule = nullptr;
PyObject* THPVariableFunctionsModule = nullptr;

static PyMethodDef torch_functions[] = {
      {"randn", castPyCFunctionWithKeywords(THPVariable_randn), METH_VARARGS | METH_KEYWORDS | METH_STATIC, NULL}
  };

void gatherTorchFunctions(std::vector<PyMethodDef>& functions)
{
    constexpr size_t num_functions = sizeof(torch_functions) / sizeof(torch_functions[0]);
    functions.insert(
    functions.end(),
    torch_functions,
    torch_functions + num_functions);
}
static PyTypeObject THPVariableFunctions = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  "torch._C._VariableFunctionsClass",    /* tp_name */
  0,                                     /* tp_basicsize */
  0,                                     /* tp_itemsize */
  nullptr,                               /* tp_dealloc */
  0,                                     /* tp_vectorcall_offset */
  nullptr,                               /* tp_getattr */
  nullptr,                               /* tp_setattr */
  nullptr,                               /* tp_reserved */
  nullptr,                               /* tp_repr */
  nullptr,                               /* tp_as_number */
  nullptr,                               /* tp_as_sequence */
  nullptr,                               /* tp_as_mapping */
  nullptr,                               /* tp_hash  */
  nullptr,                               /* tp_call */
  nullptr,                               /* tp_str */
  nullptr,                               /* tp_getattro */
  nullptr,                               /* tp_setattro */
  nullptr,                               /* tp_as_buffer */
  Py_TPFLAGS_DEFAULT,                    /* tp_flags */
  nullptr,                               /* tp_doc */
  nullptr,                               /* tp_traverse */
  nullptr,                               /* tp_clear */
  nullptr,                               /* tp_richcompare */
  0,                                     /* tp_weaklistoffset */
  nullptr,                               /* tp_iter */
  nullptr,                               /* tp_iternext */
  nullptr,                               /* tp_methods */
  nullptr,                               /* tp_members */
  nullptr,                               /* tp_getset */
  nullptr,                               /* tp_base */
  nullptr,                               /* tp_dict */
  nullptr,                               /* tp_descr_get */
  nullptr,                               /* tp_descr_set */
  0,                                     /* tp_dictoffset */
  nullptr,                               /* tp_init */
  nullptr,                               /* tp_alloc */
  nullptr                                /* tp_new */
};
void initFunctions(PyObject *module) {
    static std::vector<PyMethodDef> torch_functions;
    gatherTorchFunctions(torch_functions);
    torch_functions.push_back({nullptr});
    torch_functions.shrink_to_fit();

    THPVariableFunctions.tp_methods = torch_functions.data();
    // std::cerr<<"1"<<::std::endl;
//   THPVariableFunctions.tp_methods = torch_functions;

  if (PyType_Ready(&THPVariableFunctions) < 0) {
    //   std::cerr<<"2"<<::std::endl;
    throw python_error();
  }
//   std::cerr<<"3"<<::std::endl;
  Py_INCREF(&THPVariableFunctions);

  // Steals
  Py_INCREF(&THPVariableFunctions);
  if (PyModule_AddObject(module, "_VariableFunctionsClass",
                         reinterpret_cast<PyObject*>(&THPVariableFunctions)) < 0) {
    throw python_error();
  }
  // PyType_GenericNew returns a new reference
  THPVariableFunctionsModule = PyType_GenericNew(&THPVariableFunctions, Py_None, Py_None);
  // PyModule_AddObject steals a reference
  if (PyModule_AddObject(module, "_VariableFunctions", THPVariableFunctionsModule) < 0) {
    throw python_error();
  }
}

int TensorMetaType_init(PyObject *cls, PyObject *args, PyObject *kwargs)
{
    ::std::cerr<<"call meta init"<<::std::endl;
    return 1;
}
static PyTypeObject TensorMetaType = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  "torch._C._TensorMeta",                      /* tp_name */
  sizeof(TensorMeta),                     /* tp_basicsize */
  0,                                           /* tp_itemsize */
  nullptr,                                     /* tp_dealloc */
  0,                                           /* tp_vectorcall_offset */
  nullptr,                                     /* tp_getattr */
  nullptr,                                     /* tp_setattr */
  nullptr,                                     /* tp_reserved */
  nullptr,                                     /* tp_repr */
  nullptr,                                     /* tp_as_number */
  nullptr,                                     /* tp_as_sequence */
  nullptr,                                     /* tp_as_mapping */
  nullptr,                                     /* tp_hash  */
  nullptr,                                     /* tp_call */
  nullptr,                                     /* tp_str */
  nullptr,                                     /* tp_getattro */
  nullptr,                                     /* tp_setattro */
  nullptr,                                     /* tp_as_buffer */
  Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,    /* tp_flags */
  nullptr,                                     /* tp_doc */
  nullptr,                                     /* tp_traverse */
  nullptr,                                     /* tp_clear */
  nullptr,                                     /* tp_richcompare */
  0,                                           /* tp_weaklistoffset */
  nullptr,                                     /* tp_iter */
  nullptr,                                     /* tp_iternext */
  nullptr,                                     /* tp_methods */
  nullptr,                                     /* tp_members */
  nullptr,                                     /* tp_getset */
  nullptr,              /* tp_base */
  nullptr,                                     /* tp_dict */
  nullptr,                                     /* tp_descr_get */
  nullptr,                                     /* tp_descr_set */
  0,                                           /* tp_dictoffset */
  TensorMetaType_init,                    /* tp_init */
  nullptr,                                     /* tp_alloc */
  nullptr,                                     /* tp_new */
};

static PyTypeObject TensorBaseType = {
    PyVarObject_HEAD_INIT(
        &TensorMetaType,
        0) "torch._C._TensorBase", /* tp_name */
    sizeof(PyTensor), /* tp_basicsize */
    0, /* tp_itemsize */
    // This is unspecified, because it is illegal to create a THPVariableType
    // directly.  Subclasses will have their tp_dealloc set appropriately
    // by the metaclass
    (destructor)tensor_destruct, /* tp_dealloc */
    0, /* tp_vectorcall_offset */
    nullptr, /* tp_getattr */
    nullptr, /* tp_setattr */
    nullptr, /* tp_reserved */
    nullptr, /* tp_repr */
    nullptr, /* tp_as_number */
    nullptr, /* tp_as_sequence */
    nullptr, /* tp_as_mapping */
    nullptr, /* tp_hash  */
    nullptr, /* tp_call */
    tensor_str, /* tp_str */
    nullptr, /* tp_getattro */
    nullptr, /* tp_setattro */
    nullptr, /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE |
        Py_TPFLAGS_HAVE_GC, /* tp_flags */
    nullptr, /* tp_doc */
    // Also set by metaclass
    nullptr, /* tp_traverse */
    (inquiry)tensor_clear, /* tp_clear */
    nullptr, /* tp_richcompare */
    0, /* tp_weaklistoffset */
    nullptr, /* tp_iter */
    nullptr, /* tp_iternext */
    nullptr, /* tp_methods */
    nullptr, /* tp_members */
    nullptr, /* tp_getset */
    nullptr, /* tp_base */
    nullptr, /* tp_dict */
    nullptr, /* tp_descr_get */
    nullptr, /* tp_descr_set */
    0, /* tp_dictoffset */
    nullptr, /* tp_init */
    nullptr, /* tp_alloc */
    // Although new is provided here, it is illegal to call this with cls ==
    // THPVariableMeta.  Instead, subclass it first and then construct it
    (newfunc)tensor_new, /* tp_new */
};

static PyMethodDef tensor_methods[] = {
  // These magic methods are all implemented on python object to wrap NotImplementedError
  {"__add__", castPyCFunctionWithKeywords(tensor_add), METH_VARARGS, NULL},
  {"__radd__", castPyCFunctionWithKeywords(tensor_add), METH_VARARGS, NULL},
  {"backward", (PyCFunction)(void(*)(void))(tensor_backward), METH_NOARGS, NULL},
    {nullptr}
};

void initTensor(PyObject *module) {
  TensorMetaType.tp_base = &PyType_Type;
  if (PyType_Ready(&TensorMetaType)<0)
    return;
  Py_INCREF(&TensorMetaType);
  PyModule_AddObject(module, "_TensorMeta",(PyObject *)&TensorMetaType);

  TensorBaseType.tp_methods = tensor_methods;

  if (PyType_Ready(&TensorBaseType) < 0) {
    throw python_error();
  }
  Py_INCREF(&TensorBaseType);

  // Steals
  // Py_INCREF(&TensorType);
  // PyModule_AddObject(module, "_TensorBaseClass",   (PyObject *)&TensorType);
  // return;
  if (PyModule_AddObject(module, "_TensorBase",
                         reinterpret_cast<PyObject*>(&TensorBaseType)) < 0) {
    throw python_error();
  }
  
}

static PyTypeObject* TensorType = nullptr;
PyTypeObject* getTensorType()
{
  if (TensorType==nullptr)
  {
    std::cout<<"nullptr type"<<std::endl;
    PyObject* tensor_module = PyImport_ImportModule("torch._Tensor");
    if (!tensor_module)
    {
        std::cerr<<"tensor_module nullptr"<<std::endl;
        return nullptr;
    }
    TensorType = (PyTypeObject* )PyObject_GetAttrString(tensor_module, "Tensor");
  }
  return TensorType;
}

