#pragma once
#include "Python_headers.h"
#include <vector>

void gatherTorchFunctions(std::vector<PyMethodDef>& functions);
void initFunctions(PyObject *module);

int TensorMetaType_init(PyObject *cls, PyObject *args, PyObject *kwargs);


void initTensor(PyObject *module);

PyTypeObject* getTensorType();

