#include "LogManager.hpp"
#include "Logger.hpp"
#include <pybind11/embed.h>
#include <cstdarg>


namespace LOG {

map<string, ILogger *> LogManager::m_loggers = map<string, ILogger *>();

void LogManager::init() {
    if (PyType_Ready(&PyLogManagerType) < 0)
	{
		return;
	}
	PyLogManager *logoutput = PyObject_NEW_VAR(PyLogManager, &PyLogManagerType, 0);
	PySys_SetObject((char*)"stdout", (PyObject*)logoutput);
	PySys_SetObject((char*)"stderr", (PyObject*)logoutput);
	Py_DECREF(logoutput);
	
    LogManager::registerLogger(new ConsoleLogger);
    LogManager::registerLogger(new FileLogger("log/log.txt"));
}

void LogManager::log(LogLevel level, const char * format, ...) {
    va_list args, args1;
    va_start(args, format);
    va_copy(args1, args);

    size_t len = _vscprintf(format, args) + 1;
    char * log_string = new char[len];
    vsprintf(log_string, format, args1);
    for(auto iter: LogManager::m_loggers) {
        iter.second->write(log_string);
    }
    delete log_string;

    va_end(args);
}

void LogManager::registerLogger(ILogger * logger) {
    LogManager::m_loggers[logger->GetName()] = logger;
}

void LogManager::unregisterLogger(ILogger * logger) {
    LogManager::unregisterLogger(logger->GetName());
}

void LogManager::unregisterLogger(std::string name) {
    LogManager::m_loggers.erase(name);
}

void LogManager::showAllLogger() {
    for (auto iter: LogManager::m_loggers) {
        LogManager::log(LogLevel::INFO, "%s\n", iter.first.c_str());
    }
}


/* start of PyLogManager */

static PyObject* PyLogManager_Write(PyObject *self, PyObject *args)
{
	char *data;
	if (!PyArg_ParseTuple(args, "s", &data))
	{
		return 0;
	}
    LogManager::log(LogLevel::INFO, data);
	Py_RETURN_NONE;
}

static PyObject* PyLogManager_Flush(PyObject *self, PyObject *args)
{
	Py_RETURN_NONE;
}

static PyMethodDef PyLogManager_methods[] = {
	{"write", PyLogManager_Write, METH_VARARGS, "write"},
	{"flush", PyLogManager_Flush , METH_VARARGS, "flush"},
	{NULL, NULL} /* Sentinel */
};

PyTypeObject PyLogManagerType = {
	PyObject_HEAD_INIT(NULL)
	0,								// ob_size
	"PyLogManager",			        // tp_name
	sizeof(PyLogManager),			// tp_basicsize
	0,								// tp_itemsize
	0,	                            // tp_dealloc
	0,								// tp_print
	0,								// tp_getattr
	0,								// tp_setattr
	0,								// tp_compare
	0,								// tp_repr
	0,								// tp_as_number
	0,								// tp_as_sequence
	0,								// tp_as_mapping
	0,								// tp_hash
	0,								// tp_call
	0,								// tp_str
	0,								// tp_getattro
	0,								// tp_setattro
	0,								// tp_as_buffer
	Py_TPFLAGS_DEFAULT,				// tp_flags
	"PyLogManager",				    // tp_doc
	0,								// tp_traverse
	0,								// tp_clear
	0,								// tp_richcompare
	0,								// tp_weaklistoffset
	0,								// tp_iter
	0,								// tp_iternext
	PyLogManager_methods,			// tp_methods
	0,								// tp_members
	0,			                    // tp_getset
	0,								// tp_base
	0,								// tp_dict
	0,								// tp_descr_get
	0,								// tp_descr_set
	0,								// tp_dictoffset
	0,		                        // tp_init
	0,								// tp_alloc
	PyType_GenericNew,				// tp_new
};

}