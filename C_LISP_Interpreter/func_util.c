#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "parsedef.h"
#include "lisp_processor_utils.h"

int getDigitCount(int n) {
	int digits = 1;
	while (n > 10) {
		n /= 10;
		digits++;
	}
	return digits;
}

obj_t makeNIL() {
	obj_t obj;
	obj.type = NIL;
	return obj;
}

obj_t makeNumber(double number) {
	obj_t result;
	result.type = NUMBER;
	result.number.type = NUMBER;
	result.number.value = number;
	result.number.literal = (string_t)malloc(sizeof(char) * (getDigitCount(result.number.value) + 30));
	if (result.number.literal == NULL)
		return create_error();
	sprintf(result.number.literal, "%lf", result.number.value);
	string_t realloced = (string_t)realloc(result.number.literal, sizeof(char) * (strlen(result.number.literal) + 1));
	if (realloced == NULL)
		return create_error();
	result.number.literal = realloced;

	return result;
}

obj_t* makeListWithValue(obj_t* value) {
	obj_t* newList = malloc(sizeof(obj_t));
	if (newList == NULL)
		return NULL;
	newList->type = LIST;
	newList->list.type = LIST;
	newList->list.next = NULL;
	newList->list.value = value;
	return newList;
}


obj_t* prependList(obj_t* list, obj_t* value) {
	obj_t* nextList = list;
	obj_t* newList = (obj_t*)malloc(sizeof(obj_t));
	if (newList == NULL) { return NULL; }
	newList->type = LIST;
	newList->list.type = LIST;
	newList->list.next = nextList;
	newList->list.value = value;
	return newList;
}


obj_t* appendList(obj_t* list, obj_t* value) {
	obj_t* nextList = (obj_t*)malloc(sizeof(obj_t));
	if (nextList == NULL) { return NULL; }
	while (list->list.next != NULL)
		list = list->list.next;
	nextList->type = LIST;
	nextList->list.type = LIST;
	nextList->list.value = value;
	nextList->list.next = NULL;
	list->list.next = nextList;
	return nextList;
}

obj_t create_nil()
{
	obj_t result;
	result.type = NIL;
	return result;
}

bool obj_equals(obj_t* a, obj_t* b) {
	if (a->type != b->type)
		return false;
	switch (a->type) {
	case NUMBER:
		return a->number.value == b->number.value;
	case LITSYMBOL:
	case SYMBOL:
		return strcmp(a->symbol.symbol, b->symbol.symbol) == 0;
	case CODE:
	case LIST:
		if (obj_equals(a->list.value, b->list.value)) {
			if (a->list.next != NULL && b->list.next != NULL) {
				return obj_equals(a->list.next, b->list.next);
			}
			else if (a->list.next == NULL && b->list.next == NULL) {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	case NIL:
		return true;
	}
	// ??
	exit(1);
	return false;
}