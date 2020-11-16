#ifndef _H_LISP_PROC_FUNCS
#define _H_LISP_PROC_FUNCS
#include "lisp_processor.h"
obj_t fn_add(obj_t* pObj);
obj_t fn_multiply(obj_t* pObj);
obj_t fn_subtract(obj_t* pObj);
obj_t fn_divide(obj_t* pObj);
#define LISP_ARITHMETIC_FUNC_COUNT 4
#define LISP_ARITHMETIC_FUNC_NAMES "+", "*", "-", "/"
#define LISP_ARITHMETIC_FUNCS fn_add, fn_multiply, fn_subtract, fn_divide
#endif