#include "lisp_processor_functions_predicate.h"
#include "lisp_processor_utils.h"

obj_t fn_atom(obj_t* pObj)
{
	obj_t a = evaluateObject(pObj->list.value);
	if (a.type == SYMBOL)
		return create_t();
	else
		return create_nil();
}

obj_t fn_null(obj_t* pObj)
{
	obj_t a = evaluateObject(pObj->list.value);
	if (a.type == NIL)
		return create_t();
	else
		return create_nil();
}

obj_t fn_numberp(obj_t* pObj)
{
	obj_t a = evaluateObject(pObj->list.value);
	if (a.type == NUMBER)
		return create_t();
	else
		return create_nil();
}

obj_t fn_zerop(obj_t* pObj)
{
	obj_t a = evaluateObject(pObj->list.value);
	if (a.type != NUMBER)
		return create_error();
	else if (a.number.value == 0)
		return create_t();
	else
		return create_nil();
}

obj_t fn_minusp(obj_t* pObj)
{
	obj_t a = evaluateObject(pObj->list.value);
	if (a.type != NUMBER)
		return create_error();
	else if (a.number.value < 0)
		return create_t();
	else
		return create_nil();

}

obj_t fn_equal(obj_t* pObj)
{
	obj_t a = evaluateObject(pObj->list.value);
	if (pObj->list.next == NULL)
		return create_error();

	obj_t b = evaluateObject(pObj->list.next->list.value);
	if (obj_equals(&a, &b))
		return create_t();
	else
		return create_nil();
}

obj_t fn_lessthan(obj_t* pObj)
{
	obj_t a = evaluateObject(pObj->list.value);
	if (pObj->list.next == NULL)
		return create_error();

	obj_t b = evaluateObject(pObj->list.next->list.value);
	if (a.type == NUMBER && b.type == NUMBER && a.number.value < b.number.value)
		return create_t();
	else
		return create_nil();

}

obj_t fn_greaterthan(obj_t* pObj)
{
	obj_t a = evaluateObject(pObj->list.value);
	if (pObj->list.next == NULL)
		return create_error();

	obj_t b = evaluateObject(pObj->list.next->list.value);
	if (a.type == NUMBER && b.type == NUMBER && a.number.value > b.number.value)
		return create_t();
	else
		return create_nil();

}

obj_t fn_greaterOrEqualThan(obj_t* pObj)
{
	obj_t a = evaluateObject(pObj->list.value);
	if (pObj->list.next == NULL)
		return create_error();

	obj_t b = evaluateObject(pObj->list.next->list.value);
	if (a.type == NUMBER && b.type == NUMBER && a.number.value >= b.number.value)
		return create_t();
	else
		return create_nil();
}

obj_t fn_lessOrEqualThan(obj_t* pObj)
{
	obj_t a = evaluateObject(pObj->list.value);
	if (pObj->list.next == NULL)
		return create_error();

	obj_t b = evaluateObject(pObj->list.next->list.value);
	if (a.type == NUMBER && b.type == NUMBER && a.number.value <= b.number.value)
		return create_t();
	else
		return create_nil();

}

obj_t fn_stringp(obj_t* pObj)
{
	obj_t a = evaluateObject(pObj->list.value);
	if (a.type == STRING)
		return create_t();
	else
		return create_nil();
}
