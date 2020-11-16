#ifndef _H_LISP_PROC_SYMBOL
#define _H_LISP_PROC_SYMBOL
#include "lisp_processor.h"
obj_t fn_setq(obj_t* pObj);
obj_t fn_list(obj_t* pObj);
#define LISP_SYMBOL_FUNC_COUNT 2
#define LISP_SYMBOL_FUNC_NAMES "SETQ", "LIST"
#define LISP_SYMBOL_FUNCS fn_setq, fn_list
#endif