#ifndef _H_LISP_PROC
#define _H_LISP_PROC
#include "parsedef.h"
#include "lisp.yy.h"
#include "lisp.tab.h"
#include "c_dictionary.h"

extern C_DICTIONARY* symbolsDict;
void initializeProcessor();
void freeProcessor();
obj_t evaluateObject(const obj_t* pObj);

#endif