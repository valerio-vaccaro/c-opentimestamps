/* ots.i */
%module ots
#undef __cplusplus
#define __cplusplus 201103
#define SWIG_FILE_WITH_INIT

%include "stdint.i"
%include "std_string.i"
%include "cpointer.i"
%pointer_class(int, intp);
%pointer_class(unsigned char, ucharp);

%include typemaps.i
%apply unsigned char *OUTPUT {unsigned char *hash};

%include "carrays.i"
//%include "cdata.i"
%array_class(int, intArray);
%array_class(double, doubleArray);
%array_class(unsigned char, ucharArray);

%apply int { size_t };
typedef int size_t;

%include <pybuffer.i>
%pybuffer_mutable_binary(const unsigned char *data, const size_t len);
%pybuffer_mutable_binary(const unsigned char *msg, const size_t len);

%{
/* Put header files here or function declarations like below */
#include "../src/Common.h"
#include "../src/Context.h"
#include "../src/Op.h"
#include "../src/Attestation.h"
#include "../src/Timestamp.h"
#include "../src/DetachedFile.h"
%}

%include "../src/Common.h"
%include "../src/Context.h"
%include "../src/Op.h"
%include "../src/Attestation.h"
%include "../src/Timestamp.h"
%include "../src/DetachedFile.h"

