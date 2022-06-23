#pragma once
#define PY_SSIZE_T_CLEAN
#include "Python.h"

// define callback pointers
PyObject *callback_spAtlasPage_createTexture;
PyObject *callback_spAtlasPage_disposeTexture;
PyObject *callback_spUtil_readFile;

PyObject *
set_callback(PyObject *dummy, PyObject *args);