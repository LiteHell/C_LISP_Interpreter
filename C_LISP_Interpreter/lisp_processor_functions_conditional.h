#ifndef _H_LISP_PROC_COND
#define _H_LISP_PROC_COND
#include "lisp_processor.h"
bool is_t(obj_t* obj);
bool is_error(obj_t* obj);
obj_t create_t();
obj_t create_error();
#define LISP_ARITHMETIC_FUNC_COUNT 0
#define LISP_ARITHMETIC_FUNC_NAMES
#define LISP_ARITHMETIC_FUNCS
#endif