#ifndef _H_LISP_PROC_LIST
#define _H_LISP_PROC_LIST
#include "lisp_processor.h"
obj_t fn_car(obj_t* pObj);
obj_t fn_cdr(obj_t * pObj);
obj_t fn_nth(obj_t* pObj);
obj_t fn_cons(obj_t* pObj);
obj_t fn_reverse(obj_t* pObj);
obj_t fn_append(obj_t* pObj);
obj_t fn_length(obj_t* pObj);
obj_t fn_member(obj_t* pObj);
obj_t fn_assoc(obj_t* pObj);
obj_t fn_remove(obj_t* pObj);
obj_t fn_subst(obj_t* pObj);
#define LISP_LIST_FUNC_COUNT 9
#define LISP_LIST_FUNC_NAMES "NTH", "CONS", "REVERSE", "APPEND", "LENGTH", "MEMBER", "ASSOC", "REMOVE", "SUBST"
#define LISP_LIST_FUNCS fn_nth, fn_cons, fn_reverse, fn_append, fn_length, fn_member, fn_assoc, fn_remove, fn_subst
#endif