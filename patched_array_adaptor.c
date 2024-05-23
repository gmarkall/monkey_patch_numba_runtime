#include "Python.h"

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include "numpy/ndarrayobject.h"

typedef struct {
    void     *meminfo;
    PyObject *parent;
    npy_intp nitems;
    npy_intp itemsize;
    void *data;

    npy_intp shape_and_strides[];
} arystruct_t;

// Declaration - should be in symbol table of process
void* NRT_meminfo_new_from_pyobject(void *data, PyObject *ownerobj);

// This may need calling during module initialization to set PyArray_API
void* init_patched()
{
  import_array();
}

// Patched Numba function
int patched_NRT_adapt_ndarray_from_python(PyObject *obj, arystruct_t* arystruct)
{
    PyArrayObject *ndary;
    int i, ndim;
    npy_intp *p;
    void *data;

    if (!PyArray_Check(obj)) {
      obj = PyObject_CallMethod(obj, "__array__", NULL);
      if (!obj) {
        return -1;
      }
    }

    ndary = (PyArrayObject*)obj;
    ndim = PyArray_NDIM(ndary);
    data = PyArray_DATA(ndary);

    arystruct->meminfo = NRT_meminfo_new_from_pyobject((void*)data, obj);
    arystruct->data = data;
    arystruct->nitems = PyArray_SIZE(ndary);
    arystruct->itemsize = PyArray_ITEMSIZE(ndary);
    arystruct->parent = obj;
    p = arystruct->shape_and_strides;
    for (i = 0; i < ndim; i++, p++) {
        *p = PyArray_DIM(ndary, i);
    }
    for (i = 0; i < ndim; i++, p++) {
        *p = PyArray_STRIDE(ndary, i);
    }

    return 0;
}

// Add code to make this into a CPython extension module here
