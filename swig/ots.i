/* example.i */
%module ots
#undef __cplusplus
#define __cplusplus 201103
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

