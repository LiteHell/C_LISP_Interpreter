#define _CRT_SECURE_NO_WARNINGS
#include "lisp_processor.h"
#include <string.h>
#include <stdbool.h>

int getDigitCount(int n) {
	int digits = 1;
	while (n > 10) {
		n /= 10;
		digits++;
	}
	return digits;
}

obj_t fn_add(obj_t* pObj) {
	obj_t result;
	const obj_t* list = pObj;
	result.type = NUMBER;
	result.number.type = NUMBER;
	result.number.value = 0;
	while (list) {
		obj_t evaluated = evaluateObject(list->list.value);
		if (evaluated.type != NUMBER) {
			printf("Something's wrong");
			exit(1);
		}
		result.number.value += evaluated.number.value;
		list = list->list.next;
	}
	result.number.literal = (string_t)malloc(sizeof(char) * (getDigitCount(result.number.value) + 1));
	_itoa(result.number.value, result.number.literal, 10);
	return result;
}

obj_t fn_multiply(obj_t* pObj)
{
	obj_t result;
	const obj_t* list = pObj;
	result.type = NUMBER;
	result.number.type = NUMBER;
	result.number.value = 0;
	bool firstVal = true;
	while (list) {
		obj_t evaluated = evaluateObject(list->list.value);
		if (evaluated.type != NUMBER) {
			printf("Something's wrong");
			exit(1);
		}
		if (firstVal) {
			result.number.value = evaluated.number.value;
			firstVal = false;
		}
		else
			result.number.value *= evaluated.number.value;
		list = list->list.next;
	}
	result.number.literal = (string_t)malloc(sizeof(char) * (getDigitCount(result.number.value) + 1));
	_itoa(result.number.value, result.number.literal, 10);
	return result;
}

obj_t fn_subtract(obj_t* pObj)
{
	obj_t result;
	const obj_t* list = pObj;
	result.type = NUMBER;
	result.number.type = NUMBER;
	result.number.value = 0;
	bool firstVal = true;
	while (list) {
		obj_t evaluated = evaluateObject(list->list.value);
		if (evaluated.type != NUMBER) {
			printf("Something's wrong");
			exit(1);
		}
		if (firstVal) {
			result.number.value = evaluated.number.value;
			firstVal = false;
		}
		else
			result.number.value -= evaluated.number.value;
		list = list->list.next;
	}
	result.number.literal = (string_t)malloc(sizeof(char) * (getDigitCount(result.number.value) + 1));
	_itoa(result.number.value, result.number.literal, 10);
	return result;
}

obj_t fn_divide(obj_t* pObj)
{
	obj_t result;
	const obj_t* list = pObj;
	result.type = NUMBER;
	result.number.type = NUMBER;
	result.number.value = 0;
	bool firstVal = true;
	while (list) {
		obj_t evaluated = evaluateObject(list->list.value);
		if (evaluated.type != NUMBER) {
			printf("Something's wrong");
			exit(1);
		}
		if (firstVal) {
			result.number.value = evaluated.number.value;
			firstVal = false;
		}
		else
			result.number.value /= evaluated.number.value;
		list = list->list.next;
	}
	result.number.literal = (string_t)malloc(sizeof(char) * (getDigitCount(result.number.value) + 1));
	_itoa(result.number.value, result.number.literal, 10);
	return result;
}
