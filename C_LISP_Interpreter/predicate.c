#include "lisp_processor_functions_predicate.h"
#include "lisp_processor_utils.h"

obj_t fn_atom(obj_t* pObj)
{
	if (argument_count(pObj) != 1)
		return create_error();
	// list is the only non-atom type; nil (non-empty list) is also atom
	return evaluateObject(pObj->list.value).type != LIST ? create_t() : makeNIL();
}

obj_t fn_null(obj_t* pObj)
{
	if (argument_count(pObj) != 1)
		return create_error();
	return evaluateObject(pObj->list.value).type == NIL ? create_t() : makeNIL();
}

obj_t fn_numberp(obj_t* pObj)
{
	if (argument_count(pObj) != 1)
		return create_error();
	return evaluateObject(pObj->list.value).type == NUMBER ? create_t() : makeNIL();
}

obj_t fn_zerop(obj_t* pObj)
{
	if (argument_count(pObj) != 1)
		return create_error();
	obj_t a = evaluateObject(pObj->list.value);
	if (a.type != NUMBER)
		return create_error();
	return a.number.value == 0 ? create_t() : makeNIL();
}

obj_t fn_minusp(obj_t* pObj)
{
	if (argument_count(pObj) != 1)
		return create_error();
	obj_t a = evaluateObject(pObj->list.value);
	if (a.type != NUMBER)
		return create_error();
	return a.number.value < 0 ? create_t() : makeNIL();
}

obj_t fn_equal(obj_t* pObj)
{
	if (argument_count(pObj) != 2)
		return create_error();
	obj_t a = evaluateObject(pObj->list.value);
	obj_t b = evaluateObject(pObj->list.next->list.value);
	return obj_equals(&a, &b) ? create_t() : makeNIL();
}

obj_t fn_lessthan(obj_t* pObj)
{
	if (pObj == NULL)
		return create_error();
	obj_t last = evaluateObject(pObj->list.value);
	if (last.type != NUMBER)
		return create_error();
	pObj = pObj->list.next;
	while (pObj) {
		obj_t now = evaluateObject(pObj->list.value);
		if (now.type != NUMBER)
			return create_error();
		if (last.number.value >= now.number.value)
			return makeNIL();
		last = now;
		pObj = pObj->list.next;
	}
	return create_t();
}

obj_t fn_greaterthan(obj_t* pObj)
{
	if (pObj == NULL)
		return create_error();
	obj_t last = evaluateObject(pObj->list.value);
	if (last.type != NUMBER)
		return create_error();
	pObj = pObj->list.next;
	while (pObj) {
		obj_t now = evaluateObject(pObj->list.value);
		if (now.type != NUMBER)
			return create_error();
		if (last.number.value <= now.number.value)
			return makeNIL();
		last = now;
		pObj = pObj->list.next;
	}
	return create_t();
}

obj_t fn_greaterOrEqualThan(obj_t* pObj)
{
	if (pObj == NULL)
		return create_error();
	obj_t last = evaluateObject(pObj->list.value);
	if (last.type != NUMBER)
		return create_error();
	pObj = pObj->list.next;
	while (pObj) {
		obj_t now = evaluateObject(pObj->list.value);
		if (now.type != NUMBER)
			return create_error();
		if (last.number.value < now.number.value)
			return makeNIL();
		last = now;
		pObj = pObj->list.next;
	}
	return create_t();
}

obj_t fn_lessOrEqualThan(obj_t* pObj)
{
	if (pObj == NULL)
		return create_error();
	obj_t last = evaluateObject(pObj->list.value);
	if (last.type != NUMBER)
		return create_error();
	pObj = pObj->list.next;
	while (pObj) {
		obj_t now = evaluateObject(pObj->list.value);
		if (now.type != NUMBER)
			return create_error();
		if (last.number.value > now.number.value)
			return makeNIL();
		last = now;
		pObj = pObj->list.next;
	}
	return create_t();
}

obj_t fn_equalOp(obj_t* pObj)
{
	if (pObj == NULL)
		return create_error();
	obj_t first = evaluateObject(pObj->list.value);
	if (first.type != NUMBER)
		return create_error();
	pObj = pObj->list.next;
	while (pObj) {
		obj_t now = evaluateObject(pObj->list.value);
		if (now.type != NUMBER)
			return create_error();
		if (first.number.value != now.number.value)
			return makeNIL();
		pObj = pObj->list.next;
	}
	return create_t();
}

obj_t fn_stringp(obj_t* pObj)
{
	if (argument_count(pObj) != 1)
		return create_error();
	return evaluateObject(pObj->list.value).type == STRING ? create_t() : makeNIL();
}
