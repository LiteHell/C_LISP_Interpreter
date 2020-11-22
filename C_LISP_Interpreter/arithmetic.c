#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdbool.h>
#include "lisp_processor_functions_arithmetic.h"
#include "lisp_processor_utils.h"

// add all arguments; returns 0 with no arguments
obj_t fn_add(obj_t* pObj)
{
	double value = 0;
	while (pObj) {
		obj_t evaluated = evaluateObject(pObj->list.value);
		if (evaluated.type != NUMBER)
			return create_error();
		value += evaluated.number.value;
		pObj = pObj->list.next;
	}
	return makeNumber(value);
}

// multiply all arguments; returns 1 with no arguments
obj_t fn_multiply(obj_t* pObj)
{
	double value = 1;
	while (pObj) {
		obj_t evaluated = evaluateObject(pObj->list.value);
		if (evaluated.type != NUMBER)
			return create_error();
		value *= evaluated.number.value;
		pObj = pObj->list.next;
	}
	return makeNumber(value);
}

// 1 argument: returns -(argument)
// 2+ arguments: subtracts the other arguments from the first
obj_t fn_subtract(obj_t* pObj)
{
	if (pObj == NULL)
		return create_error();
	double value = pObj->list.value->number.value;
	if (pObj->list.next == NULL) // 1 argument
		value = -value;
	else
		value -= fn_add(pObj->list.next).number.value;
	return makeNumber(value);
}

// 1 argument: returns 1/(argument)
// 2+ arguments: divides the first argument by the other arguments
// division by zero results in 'error
obj_t fn_divide(obj_t* pObj)
{
	if (pObj == NULL)
		return create_error();
	double value = pObj->list.value->number.value, num, denom;
	if (pObj->list.next == NULL) { // 1 argument
		num = 1;
		denom = value;
	}
	else {
		num = value;
		denom = fn_multiply(pObj->list.next).number.value;
	}
	return denom == 0 ? create_error() : makeNumber(num / denom);
}
