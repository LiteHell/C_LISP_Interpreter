#ifndef _H_LISP_PROC_UTILS
#define _H_LISP_PROC_UTILS
#include <stdbool.h>
#include "parsedef.h"
bool obj_equals(obj_t* a, obj_t* b);
obj_t makeNIL();
obj_t makeNumber(double number);
obj_t* makeListWithValue(obj_t* value);
obj_t* prependList(obj_t* list, obj_t* value);
obj_t* appendList(obj_t* list, obj_t* value);
#endif