/* Interfaz SWIG: expone la clase C++ Receptor como el módulo Python "parqueaderolib" */
%module parqueaderolib

%include "std_string.i"

%{
#include "Receptor.h"
%}

%include "Receptor.h"
