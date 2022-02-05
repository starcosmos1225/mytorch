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
//   ASSERT_TRUE(module = PyModule_Create(&torchmodule));
//   ASSERT_TRUE(THPGenerator_init(module));
//   ASSERT_TRUE(THPException_init(module));
//   THPSize_init(module);
//   THPDtype_init(module);
//   THPDTypeInfo_init(module);
//   THPLayout_init(module);
//   THPMemoryFormat_init(module);
//   THPQScheme_init(module);
//   THPDevice_init(module);
//   THPStream_init(module);
//   ASSERT_TRUE(THPVariable_initModule(module));
//   ASSERT_TRUE(THPFunction_initModule(module));
//   ASSERT_TRUE(THPEngine_initModule(module));
//   // NOTE: We need to be able to access OperatorExportTypes from ONNX for use in
//   // the export side of JIT, so this ONNX init needs to appear before the JIT
//   // init.
//   torch::onnx::initONNXBindings(module);
//   torch::jit::initJITBindings(module);
//   torch::impl::dispatch::initDispatchBindings(module);
//   torch::throughput_benchmark::initThroughputBenchmarkBindings(module);
//   torch::crash_handler::initCrashHandlerBindings(module);
//   torch::autograd::initReturnTypes(module);
//   torch::autograd::initNNFunctions(module);
//   torch::autograd::initFFTFunctions(module);
//   torch::autograd::initLinalgFunctions(module);
//   torch::autograd::initSparseFunctions(module);
//   torch::autograd::initSpecialFunctions(module);
//   torch::autograd::init_legacy_variable(module);
//   torch::python::init_bindings(module);