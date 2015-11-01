#include "Python.h"

#include <string>
#include "../ip_table.h"

static IPLib ip_lib;

static PyObject* load_dict_function(PyObject *self, PyObject *args) {
    const char* dict_path = NULL;
    if (!PyArg_ParseTuple(args, "s", &dict_path)) {
        return NULL;
    }

    ip_lib.LoadDict(dict_path);

    Py_INCREF(Py_True);
    return Py_True;
}

static PyObject* get_country_function(PyObject *self, PyObject *args) {
    const char* ip = NULL;
    if (!PyArg_ParseTuple(args, "s", &ip)) {
        return NULL;
    }

    std::string country;
    ip_lib.GetCountry(ip, country);

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

