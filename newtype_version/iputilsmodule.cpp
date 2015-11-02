#include "Python.h"
#include "structmember.h"

#include <fcntl.h>
#include <string>

#include "../ip_table.h"

typedef struct {
    PyObject_HEAD
    IPLib* ip_lib;
} IP2Location;

static void
IP2Location_dealloc(IP2Location* self) {
    delete self->ip_lib;
    self->ob_type->tp_free((PyObject*)self);
}

static PyObject*
IP2Location_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    IP2Location* self;
    self = (IP2Location *)type->tp_alloc(type, 0);
    self->ip_lib = new IPLib();
    return (PyObject*) self;
}

static int
IP2Location_init(IP2Location* self, PyObject* args, PyObject* kwds) {
    const char* dict_path = NULL;
    PyObject* callback = NULL;

    static char* kwlist[] = {"dict_path", "callback", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "s|O", kwlist, &dict_path, &callback)) {
        return -1;
    }

    // dict path not exist
    if (access(dict_path, F_OK) != 0) {
        PyErr_SetString(PyExc_ValueError,
                        "The dict_path must exist");
        return -1;
    }

    if (callback) {
        if (!PyCallable_Check(callback)) {
            PyErr_SetString(PyExc_TypeError,
                            "The callback must be a function");
            return -1;
        }
        FILE* f = fopen(dict_path, "r");
        char buf[1024];
        PyObject* arglist;
        PyObject* result;

        const char* start;
        const char* end;
        const char* country;
        while (fgets(buf, sizeof(buf), f)) {
            arglist = Py_BuildValue("(s)", buf);
            result = PyObject_CallObject(callback, arglist);
            Py_DECREF(arglist);
            if (PyArg_ParseTuple(result, "sss", &start, &end, &country)) {
                self->ip_lib->PushItem(start, end, country);
            }
            Py_DECREF(result);
        }
    } else {
        self->ip_lib->LoadDict(dict_path);
    }

    return 0;
}

static PyMemberDef IP2Location_members[] = {
    {NULL}  /* Sentinel */
};

static PyGetSetDef IP2Location_getseters[] = {
    {NULL} /* Sentinel */
};

static PyObject *
IP2Location_get_country(IP2Location* self, PyObject* args)
{
    const char* ip;

    if (!PyArg_ParseTuple(args, "s", &ip)) {
        return NULL;
    }

    std::string country;
    self->ip_lib->GetCountry(ip, country);

    if (country.empty()) {
        Py_INCREF(Py_None);
        return Py_None;
    } else {
        return Py_BuildValue("s", country.c_str());
    }
}

static PyObject *
IP2Location_cleanup(IP2Location* self)
{
    self->ip_lib->CleanUp();
    Py_INCREF(Py_True);
    return Py_True;
}

static PyMethodDef IP2Location_methods[] = {
    {"get_country", (PyCFunction)IP2Location_get_country, METH_VARARGS, "get country by ip"},
    {"cleanup", (PyCFunction)IP2Location_cleanup, METH_NOARGS, "cleanup"},
    {NULL}  /* Sentinel */
};

static PyTypeObject IP2LocationType = {
    PyObject_HEAD_INIT(NULL)
    0,                                        /*ob_size*/
    "_iputils.IP2Location",                   /*tp_name*/
    sizeof(IP2Location),                      /*tp_basicsize*/
    0,                                        /*tp_itemsize*/
    (destructor)IP2Location_dealloc,          /*tp_dealloc*/
    0,                                        /*tp_print*/
    0,                                        /*tp_getattr*/
    0,                                        /*tp_setattr*/
    0,                                        /*tp_compare*/
    0,                                        /*tp_repr*/
    0,                                        /*tp_as_number*/
    0,                                        /*tp_as_sequence*/
    0,                                        /*tp_as_mapping*/
    0,                                        /*tp_hash */
    0,                                        /*tp_call*/
    0,                                        /*tp_str*/
    0,                                        /*tp_getattro*/
    0,                                        /*tp_setattro*/
    0,                                        /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "IP2Location objects",                    /* tp_doc */
    0,                                        /* tp_traverse */
    0,                                        /* tp_clear */
    0,                                        /* tp_richcompare */
    0,                                        /* tp_weaklistoffset */
    0,                                        /* tp_iter */
    0,                                        /* tp_iternext */
    IP2Location_methods,                      /* tp_methods */
    IP2Location_members,                      /* tp_members */
    IP2Location_getseters,                    /* tp_getset */
    0,                                        /* tp_base */
    0,                                        /* tp_dict */
    0,                                        /* tp_descr_get */
    0,                                        /* tp_descr_set */
    0,                                        /* tp_dictoffset */
    (initproc)IP2Location_init,               /* tp_init */
    0,                                        /* tp_alloc */
    IP2Location_new,                          /* tp_new */
};

static PyMethodDef module_methods[] = {
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC init_iputils(void) {
    PyObject* m;
    if (PyType_Ready(&IP2LocationType) < 0) {
        return;
    }

    m = Py_InitModule3("_iputils", module_methods, "ip utils");
    if (m == NULL) {
        return;
    }
    PyModule_AddObject(m, "IP2Location", (PyObject*)&IP2LocationType);
}
