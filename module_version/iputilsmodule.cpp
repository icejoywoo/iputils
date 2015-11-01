#include "Python.h"

#include <string>
#include "../ip_table.h"

static IPLib ip_lib;

static PyObject* load_dict_function(PyObject *self, PyObject *args) {
    PyObject* dict_path = NULL;
    if (!PyArg_ParseTuple(args, "O", &dict_path)) {
        return NULL;
    }

    if (!PyString_Check(dict_path)) {
        PyErr_SetString(PyExc_TypeError,
                        "The dict path must be a string");
        return NULL;
    }

    char* _dict_path = PyString_AsString(dict_path);
    ip_lib.LoadDict(_dict_path);

    Py_INCREF(Py_True);
    return Py_True;
}

static PyObject* get_country_function(PyObject *self, PyObject *args) {
    PyObject* ip = NULL;
    if (!PyArg_ParseTuple(args, "O", &ip)) {
        return NULL;
    }

    if (!PyString_Check(ip)) {
        PyErr_SetString(PyExc_TypeError,
                        "The ip must be a string");
        return NULL;
    }

    std::string country;
    const char* _ip = PyString_AsString(ip);
    ip_lib.GetCountry(_ip, country);

    if (country.empty()) {
        Py_INCREF(Py_None);
        return Py_None;
    } else {
        return Py_BuildValue("s", country.c_str());
    }
}

static PyObject* cleanup_function(PyObject *self) {
    ip_lib.CleanUp();
    Py_INCREF(Py_True);
    return Py_True;
}

static PyMethodDef module_methods[] = {
    {"load_dict", (PyCFunction)load_dict_function, METH_VARARGS, "load dict to memory"},
    {"get_country", (PyCFunction)get_country_function, METH_VARARGS, "get country by ip"},
    {"cleanup", (PyCFunction)cleanup_function, METH_NOARGS, "cleanup memory"},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC init_iputils(void) {
    Py_InitModule3("_iputils", module_methods, "ip utils");
}

