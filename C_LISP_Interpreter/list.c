#include "lisp_processor_functions_list.h"
#include "lisp_processor.h"

obj_t fn_car(obj_t* pObj)
{
	obj_t evaluated = evaluateObject(pObj);
	if (evaluated.type != LIST) {
		exit(1); return;
	}
	return *evaluated.list.value;
}

obj_t fn_cdr(obj_t* pObj)
{
	obj_t evaluated = evaluateObject(pObj);
	if (evaluated.type != LIST) {
		exit(1); return;
	}
	return *evaluated.list.next;
}

obj_t fn_nth(obj_t* pObj)
{

}

obj_t fn_cons(obj_t* pObj)
{

}

obj_t fn_reverse(obj_t* pObj)
{

}

obj_t fn_append(obj_t* pObj)
{

}

obj_t fn_length(obj_t* pObj)
{

}

obj_t fn_member(obj_t* pObj)
{

}

obj_t fn_assoc(obj_t* pObj)
{

}

obj_t fn_remove(obj_t* pObj)
{

}

obj_t fn_subst(obj_t* pObj)
{

}
