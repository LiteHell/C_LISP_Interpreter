#ifndef _H_LISP_PROC_COND
#define _H_LISP_PROC_COND
#include "lisp_processor.h"
bool is_t(obj_t* obj);
bool is_error(obj_t* obj);
obj_t create_t();
obj_t create_error();
#endif