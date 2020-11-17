#ifndef _H_LISP_PROC_CONTROL
#define _H_LISP_PROC_CONTROL
#include "lisp_processor.h"
obj_t fn_if(obj_t* pObj);
obj_t fn_cond(obj_t* pObj);
#define LISP_CONTROL_FUNC_COUNT 2
#define LISP_CONTROL_FUNC_NAMES "IF", "COND"
#define LISP_CONTROL_FUNCS fn_if, fn_cond
#endif