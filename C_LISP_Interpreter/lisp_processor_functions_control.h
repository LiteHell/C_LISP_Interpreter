#ifndef _H_LISP_PROC_FUNCS
#define _H_LISP_PROC_FUNCS
#include "lisp_processor.h"
obj_t fn_if(obj_t* pObj);
obj_t fn_cond(obj_t* pObj);
#define LISP_ARITHMETIC_FUNC_COUNT 2
#define LISP_ARITHMETIC_FUNC_NAMES "IF", "COND"
#define LISP_ARITHMETIC_FUNCS fn_if, fn_cond
#endif