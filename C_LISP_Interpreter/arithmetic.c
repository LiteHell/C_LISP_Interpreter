#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdbool.h>
#include "lisp_processor_functions_arithmetic.h"
#include "lisp_processor_utils.h"

obj_t fn_add(obj_t* pObj) {
	const obj_t* list = pObj;
	double value = 0;
	while (list) {
		obj_t evaluated = evaluateObject(list->list.value);
		if (evaluated.type != NUMBER)
			return create_error();
		value += evaluated.number.value;
		list = list->list.next;
	}
	return makeNumber(value);
}

obj_t fn_multiply(obj_t* pObj)
{
	const obj_t* list = pObj;
	double value = 0;
	bool firstVal = true;
	while (list) {
		obj_t evaluated = evaluateObject(list->list.value);
		if (evaluated.type != NUMBER)
			return create_error();
		if (firstVal) {
			value = evaluated.number.value;
			firstVal = false;
		}
		else
			value *= evaluated.number.value;
		list = list->list.next;
	}
	return makeNumber(value);
}

obj_t fn_subtract(obj_t* pObj)
{
	const obj_t* list = pObj;
	double value = 0;
	bool firstVal = true;
	while (list) {
		obj_t evaluated = evaluateObject(list->list.value);
		if (evaluated.type != NUMBER)
			return create_error();
		if (firstVal) {
			value = evaluated.number.value;
			firstVal = false;
		}
		else
			value -= evaluated.number.value;
		list = list->list.next;
	}
	return makeNumber(value);
}

obj_t fn_divide(obj_t* pObj)
{
	const obj_t* list = pObj;
	double value = 0;
	bool firstVal = true;
	while (list) {
		obj_t evaluated = evaluateObject(list->list.value);
		if (evaluated.type != NUMBER)
			return create_error();
		if (firstVal) {
			value = evaluated.number.value;
			firstVal = false;
		}
		else
			value /= evaluated.number.value;
		list = list->list.next;
	}
	return makeNumber(value);
}
