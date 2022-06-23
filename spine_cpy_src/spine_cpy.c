#define PY_SSIZE_T_CLEAN
#include "Python.h"

#include "spine_cpy_src/extension.h"
#include "spine_cpy_src/test.h"

// Exported methods are collected in a table
static struct PyMethodDef method_table[] = {
    {"set_callback",
     (PyCFunction)set_callback,
     METH_VARARGS,
     ""},
    {"test_file_load",
     (PyCFunction)test_file_load,
     METH_VARARGS,
     ""},
    {NULL,
     NULL,
     0,
     NULL} // Sentinel value ending the table
};

// A struct contains the definition of a module
static PyModuleDef spine_module = {
    PyModuleDef_HEAD_INIT,
    "spine", // Module name
    "",
    -1, // Optional size of the module state memory
    method_table,
    NULL, // Optional slot definitions
    NULL, // Optional traversal function
    NULL, // Optional clear function
    NULL  // Optional module deallocation function
};

PyMODINIT_FUNC PyInit_spine(void)
{
    PyObject *m = PyModule_Create(&spine_module);
// TODO - replace with PyBool_True/False
#ifdef SKELETON_BINARY_SUPPORT
    PyModule_AddObject(m, "BINARY_SUPPORT", PyBool_FromLong(1));
#else
    PyModule_AddObject(m, "BINARY_SUPPORT", PyBool_FromLong(0));
#endif

    return m;
}