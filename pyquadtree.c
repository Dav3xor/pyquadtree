#include "quadtree.h"
#include "Python.h"

#include <Python.h>

typedef struct {
  PyObject_HEAD
  pqt_QuadTree qt;
  /* Type-specific fields go here. */
} QuadTreeObject;



static int QuadTree_init(QuadTreeObject *self, PyObject *args)
{
  float xmin, ymin, xmax, ymax;

  if (! PyArg_ParseTuple(args, "ffff", 
                         &xmin, &ymin,
                         &xmax, &ymax)) {
    return -1;
  }

  // TODO: Raise ValueError here (instead of return -1)

  newtree(&self->qt,5,5,(struct pqt_Extent){xmin, ymin, xmax, ymax});

  return 0;
}

static PyObject *
QuadTree_addpoint(QuadTreeObject *self, PyObject *args)
{
  float x, y;
  PyObject *data;
  if ( PyArg_ParseTuple(args, "ffO", 
                         &x, &y,
                         &data)) {
    Py_INCREF(data);

    addpoint(&self->qt,
             x, y,
             data);
  }
  
  // TODO: Raise ValueError here
  Py_RETURN_NONE;
}

static PyObject *
QuadTree_movepoint(QuadTreeObject *self, PyObject *args)
{
  float oldx, oldy;
  float newx, newy;

  PyObject *data;
  if ( PyArg_ParseTuple(args, "ffffO", 
                         &oldx, &oldy,
                         &newx, &newy,
                         &data)) {
    movepoint(&self->qt, 
              oldx, oldy, 
              newx, newy, 
              data);
  }
  
  // TODO: Raise ValueError here
  Py_RETURN_NONE;
}

static PyObject *
QuadTree_deletepoint(QuadTreeObject *self, PyObject *args)
{
  float x, y;
  PyObject *data;
  if ( PyArg_ParseTuple(args, "ffO", 
                         &x, &y,
                         &data)) {

    if(deletepoint(&self->qt,x,y,data)){
      Py_XDECREF(data);
    }
  }
  
  // TODO: Raise ValueError here
  Py_RETURN_NONE;
}

static PyObject *
QuadTree_listpoints(QuadTreeObject *self)
{
  listpoints(&self->qt);
    
  Py_RETURN_NONE;
}

static PyObject *
QuadTree_findnearby(QuadTreeObject *self, PyObject *args)
{

  float x, y, radius;
  if ( PyArg_ParseTuple(args, "fff", 
                         &x, &y, &radius)) {

  
    findnearby(&self->qt,x,y,radius);
  }
    
  // TODO: Raise ValueError here
  Py_RETURN_NONE;
}

void maptoleaf(pqt_LeafData *data, void *arg)
{
  PyObject *pyfunc = (PyObject *)arg;
  PyObject *pyarg = (PyObject *)data->data;
  
  if(PyCallable_Check(pyfunc)) {
    PyObject_CallFunctionObjArgs(pyfunc,pyarg,NULL);
  }
}

void listleaf(pqt_LeafData *data, void *arg)
{
  PyObject *pylist = (PyObject *)arg;
  PyObject *pydata = (PyObject *)data->data;
  
  Py_XINCREF(pydata);
  PyList_Append(pylist, pydata);
}

void pydeleteleaf(pqt_LeafData *data, void *arg)
{
  PyObject *pydata = (PyObject *)data->data;
  Py_XDECREF(pydata);
}

static PyObject *QuadTree_maptonearby(QuadTreeObject *self, PyObject *args)
{
  PyObject *func;
  float x, y, radius;
  if ( PyArg_ParseTuple(args, "Offf", 
                         &func, &x, &y, &radius)) {
    maptonearby(&self->qt,&maptoleaf,func,x,y,radius);
  }
  // TODO: Raise ValueError here  
  Py_RETURN_NONE;
}
static PyObject *QuadTree_listnearby(QuadTreeObject *self, PyObject *args)
{
  float x, y, radius;
  PyObject *list = PyList_New(0);
  if ( PyArg_ParseTuple(args, "fff", 
                        &x, &y, &radius)) {
    if (list) {
      maptonearby(&self->qt,&listleaf,list,x,y,radius);
      return list;
    }
  }
    
  // TODO: Raise ValueError here  
  return list;
}
static void QuadTree_dealloc(QuadTreeObject *self)
{ 
  deletetree(&self->qt, pydeleteleaf);   

  self->ob_type->tp_free((PyObject*)self);
}

static PyMethodDef QuadTree_methods[] = {
  {"addpoint", (PyCFunction)QuadTree_addpoint, METH_VARARGS,
   "Add a point object to the Quad Tree"
  },
  {"movepoint", (PyCFunction)QuadTree_movepoint, METH_VARARGS,
   "Move a point object in the Quad Tree"
  },
  {"deletepoint", (PyCFunction)QuadTree_deletepoint, METH_VARARGS,
   "Delete a point from the Quad Tree"
  },
  {"listpoints", (PyCFunction)QuadTree_listpoints, METH_NOARGS,
   "list all the points in the Quad Tree"
  },
  {"findnearby", (PyCFunction)QuadTree_findnearby, METH_VARARGS,
   "print a list of nearby nodes to the console"
  },
  {"listnearby", (PyCFunction)QuadTree_listnearby, METH_VARARGS,
   "make a list of points near x,y within radius"
  },
  {"maptonearby", (PyCFunction)QuadTree_maptonearby, METH_VARARGS,
   "map a function to nearby objects"
  },
  {NULL}  /* Sentinel */
};


static PyTypeObject QuadTreeType = {
  PyObject_HEAD_INIT(NULL)
  0,                         /*ob_size*/
  "quadtree.QuadTree",             /*tp_name*/
  sizeof(QuadTreeObject), /*tp_basicsize*/
  0,                         /*tp_itemsize*/
  (destructor)QuadTree_dealloc, /*tp_dealloc*/
  0,                         /*tp_print*/
  0,                         /*tp_getattr*/
  0,                         /*tp_setattr*/
  0,                         /*tp_compare*/
  0,                         /*tp_repr*/
  0,                         /*tp_as_number*/
  0,                         /*tp_as_sequence*/
  0,                         /*tp_as_mapping*/
  0,                         /*tp_hash */
  0,                         /*tp_call*/
  0,                         /*tp_str*/
  0,                         /*tp_getattro*/
  0,                         /*tp_setattro*/
  0,                         /*tp_as_buffer*/
  Py_TPFLAGS_DEFAULT,        /*tp_flags*/
  "QuadTree is a Python C extension library that implements a data structure\n"
  "useful for doing spatial indexing.  If you have thousands of objects, and\n"
  "need to quickly find out what objects are close to each other, this class\n"
  "is very useful.\n",
  0,                   /* tp_traverse */
  0,                   /* tp_clear */
  0,                   /* tp_richcompare */
  0,                   /* tp_weaklistoffset */
  0,                   /* tp_iter */
  0,                   /* tp_iternext */
  QuadTree_methods,          /* tp_methods */
  0,                         /* tp_members */
  0,                         /* tp_getset */
  0,                         /* tp_base */
  0,                         /* tp_dict */
  0,                         /* tp_descr_get */
  0,                         /* tp_descr_set */
  0,                         /* tp_dictoffset */
  (initproc)QuadTree_init,                         /* tp_init */
  0,                         /* tp_alloc */
  0,              /* tp_new */
};


#ifndef PyMODINIT_FUNC  /* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif
PyMODINIT_FUNC
initquadtree(void) 
{
  PyObject* m;

  QuadTreeType.tp_new = PyType_GenericNew;
  if (PyType_Ready(&QuadTreeType) < 0)
    return;

  m = Py_InitModule3("quadtree", QuadTree_methods,
                     "A quad tree (spatial index) implemented as a C extension.");

  Py_INCREF(&QuadTreeType);
  PyModule_AddObject(m, "QuadTree", (PyObject *)&QuadTreeType);
}
