#ifndef _H_LISP_PROC_PREDICATE
#define _H_LISP_PROC_PREDICATE
#include "lisp_processor.h"
obj_t fn_atom(obj_t* pObj);
obj_t fn_null(obj_t* pObj);
obj_t fn_numberp(obj_t* pObj);
obj_t fn_zerop(obj_t* pObj);
obj_t fn_minusp(obj_t* pObj);
obj_t fn_equal(obj_t* pObj);
obj_t fn_lessthan(obj_t* pObj);
obj_t fn_greaterOrEqualThan(obj_t* pObj);
obj_t fn_stringp(obj_t pObj);
#define LISP_PREDICATE_FUNC_COUNT 9
#define LISP_PREDICATE_FUNC_NAMES "ATOM", "NULL", "NUMBERP", "ZEROP", "MINUSP", "EQUAL", "<", ">=", "STRINGP"
#define LISP_PREDICATE_FUNCS fn_atom, fn_null, fn_numberp, fn_zerop, fn_minusp, fn_equal, fn_lessthan, fn_greaterOrEqualThan, fn_stringp
#endif