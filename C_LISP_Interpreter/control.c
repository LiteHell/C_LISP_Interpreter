#include "lisp_processor_functions_control.h"
#include "lisp_processor_utils.h"

// runs 2nd argument if nonnil, 3rd (or nil) otherwise
obj_t fn_if(obj_t* pObj) {
	int argc = argument_count(pObj);
	if (argc != 2 && argc != 3)
		return create_error();

	obj_t condition = evaluateObject(pObj->list.value);
	if (condition.type != NIL) { // truthy
		return evaluateObject(pObj->list.next->list.value);
	}
	else if (argc == 3) { // falsy; else expression is provided
		return evaluateObject(pObj->list.next->list.next->list.value);
	}
	else { // falsy; defaults to nil
		return makeNIL();
	}
}

// runs first matching predicate-expression pair; nil if none matches
obj_t fn_cond(obj_t* pObj) {
	while (pObj) {
		obj_t* sublist = pObj->list.value; // (pred expr ...)
		if (sublist->type != CODE)
			return create_error();

		obj_t pred = evaluateObject(sublist->list.value);
		if (pred.type != NIL) { // predicate is truthy
			obj_t result = makeNIL(); // result defaults to nil
			obj_t* expr = sublist->list.next; // expression list
			while (expr) {
				result = evaluateObject(expr->list.value);
				expr = expr->list.next;
			}
			return result;
		}
		pObj = pObj->list.next;
	}
	return makeNIL();
}
