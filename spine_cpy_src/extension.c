/*
Implements the extension functions required for spine-c

spAtlasPage_createTexture
spAtlasPage_disposeTexture
spUtil_readFile

*/

#define PY_SSIZE_T_CLEAN
#include "Python.h"
#include "spine_cpy_src/extension.h"

#include "spine/Atlas.h"
#include "spine/extension.h"


PyObject *
set_callback(PyObject *dummy, PyObject *args)
{
    #ifdef DEBUG
    printf("C: set_callback\n");
    #endif
    
    unsigned int selection = 0;
    PyObject *py_callback;

    if (PyArg_ParseTuple(args, "IO:set_callback", &selection, &py_callback))
    {
        if (!PyCallable_Check(py_callback))
        {
            PyErr_SetString(PyExc_TypeError, "parameter must be callable");
            return NULL;
        }

        Py_XINCREF(py_callback); /* Add a reference to new callback */

        switch (selection)
        {
        case 0:
            Py_XDECREF(callback_spAtlasPage_createTexture);
            callback_spAtlasPage_createTexture = py_callback;
            break;
        case 1:
            Py_XDECREF(callback_spAtlasPage_disposeTexture);
            callback_spAtlasPage_disposeTexture = py_callback;
            break;
        case 2:
            Py_XDECREF(callback_spUtil_readFile);
            callback_spUtil_readFile = py_callback;
            break;
        default:
            PyErr_SetString(PyExc_TypeError, "The callback selection must be one of: 1-createTexture, 2-disposeTexture, 3-readFile");
            return NULL;
        }
    }

    #ifdef DEBUG
    printf("/\n");
    #endif

    Py_INCREF(Py_None);
    return Py_None;
}

void _spAtlasPage_createTexture(spAtlasPage *self, const char *path)
{
    #ifdef DEBUG
    printf("C: _spAtlasPage_createTexture\n");
    #endif

    if (callback_spAtlasPage_createTexture == NULL)
    {
        PyErr_SetString(PyExc_TypeError, "The callback for spAtlasPage_createTexture is not set.");
        return;
    }
    PyObject *result = PyObject_CallFunction(callback_spAtlasPage_createTexture, "s", path);
    if (!PyTuple_Check(result))
    {
        printf("The return of spAtlasPage_createTexture has to be a tuple!");
        PyErr_SetString(PyExc_TypeError, "The return of spUtil_readFile has to be a tuple!");
        return;
    }
    PyArg_ParseTuple(result, "Oii", &self->rendererObject, &self->width, &self->height);
    Py_XINCREF(self->rendererObject);

    #ifdef DEBUG
    printf("/\n");
    #endif
}

void _spAtlasPage_disposeTexture(spAtlasPage *self)
{
    #ifdef DEBUG
    printf("C: _spAtlasPage_disposeTexture\n");
    #endif

    if (callback_spAtlasPage_disposeTexture == NULL)
    {
        PyErr_SetString(PyExc_TypeError, "The callback for spAtlasPage_disposeTexture is not set.");
        return;
    }
    PyObject *result = PyObject_CallFunction(callback_spAtlasPage_disposeTexture, "O", self->rendererObject);
    Py_XDECREF(self->rendererObject);

    #ifdef DEBUG
    printf("/\n");
    #endif
}

char *_spUtil_readFile(const char *path, int *length)
{
    /*
    The data read will later on be freed by spine itself
    */
    #ifdef DEBUG
    printf("C: _spUtil_readFile\n");
    #endif

    if (callback_spUtil_readFile == NULL)
    {
        printf("The callback for spUtil_readFile is not set.\n");
        PyErr_SetString(PyExc_TypeError, "The callback for spUtil_readFile is not set.\n");
        return NULL;
    }

    PyObject *result = PyObject_CallFunction(callback_spUtil_readFile, "s", path);

    if (!PyObject_CheckBuffer(result))
    {
        printf("The return of spUtil_readFile has to support the buffer protocol (e.g. bytes, bytearray, np.array)!\n");
        PyErr_SetString(PyExc_TypeError, "The return of spUtil_readFile has to support the buffer protocol (e.g. bytes, bytearray, np.array)!\n");
        return NULL;
    }
    Py_buffer view;
    if (PyObject_GetBuffer(result, &view, PyBUF_SIMPLE) < 0)
    {
        printf("Failed to get a view!\n");
        PyErr_SetString(PyExc_TypeError, "Failed to get a view!\n");
        return NULL;
    }

    *length = view.len;
    char *data = malloc(view.len);
    memcpy(data, view.buf, view.len);
    PyBuffer_Release(&view);

    #ifdef DEBUG
    printf("/\n");
    #endif
    
    return data;
}