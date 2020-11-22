#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "parsedef.h"
#include "lisp_processor_utils.h"

// DEPRECATED: calculate int length
int getDigitCount(int n) {
	int digits = 1;
	while (n > 10) {
		n /= 10;
		digits++;
	}
	return digits;
}

// make a nil
obj_t makeNIL() {
	obj_t obj;
	obj.type = NIL;
	return obj;
}

// make a number
obj_t makeNumber(double number) {
	obj_t result;
	result.type = NUMBER;
	result.number.value = number;

	char* temp = malloc(100 * sizeof(char));
	if (temp == NULL)
		return create_error();
	int len = sprintf(temp, "%f", number);
	result.number.literal = realloc(temp, len + 1);
	if (result.number.literal == NULL)
		return create_error();
	
	return result;
}

// make a list node
obj_t* makeListWithValue(obj_t* value) {
	obj_t* newList = malloc(sizeof(obj_t));
	if (newList == NULL)
		return NULL;
	newList->type = LIST;
	newList->list.next = NULL;
	newList->list.value = value;
	return newList;
}

// make a list node that points to `list`
obj_t* prependList(obj_t* list, obj_t* value) {
	obj_t* newList = malloc(sizeof(obj_t));
	if (newList == NULL)
		return NULL;
	newList->type = LIST;
	newList->list.next = list;
	newList->list.value = value;
	return newList;
}

// append a list node at the end of `list`. Returns the new node
obj_t* appendList(obj_t* list, obj_t* value) {
	obj_t* nextList = malloc(sizeof(obj_t));
	if (nextList == NULL)
		return NULL;
	while (list->list.next != NULL)
		list = list->list.next;
	list->list.next = nextList;
	nextList->type = LIST;
	nextList->list.value = value;
	nextList->list.next = NULL;
	return nextList;
}

// DUPLICATE: make a nil
obj_t create_nil()
{
	obj_t result;
	result.type = NIL;
	return result;
}

// deep object comparison
bool obj_equals(obj_t* a, obj_t* b) {
	int typeMap[] = { 0, 1, 1, 2, 3, 3, 4 }; // loose type comparison
	if (a == b) // same object optimization
		return true;
	if (typeMap[a->type] != typeMap[b->type])
		return false;

	switch (a->type) {
	case NUMBER:
		return a->number.value == b->number.value;
	case SYMBOL: case LITSYMBOL:
		return strcmp(a->symbol.symbol, b->symbol.symbol) == 0;
	case STRING:
		return strcmp(a->string.string, b->string.string) == 0;
	case CODE: case LIST:
		if (obj_equals(a->list.value, b->list.value)) {
			if ((a->list.next == NULL) != (b->list.next == NULL))
				return false;
			if (a->list.next != NULL)
				return obj_equals(a->list.next, b->list.next);
			return true;
		}
		else
			return false;
	case NIL:
		return true;
	}
	return false;
}

int argument_count(obj_t* obj) {
	int count = 0;
	while (obj) {
		count++;
		obj = obj->list.next;
	}
	return count;
}

// checks if obj is 't
bool is_t(obj_t* obj) {
	return (obj->type == SYMBOL || obj->type == LITSYMBOL) && !strcmp(obj->symbol.symbol, "T");
}
// checks if obj is 'error
bool is_error(obj_t* obj) {
	return (obj->type == SYMBOL || obj->type == LITSYMBOL) && !strcmp(obj->symbol.symbol, "ERROR");
}
// make a 't
obj_t create_t() {
	obj_t t;
	string_t tstr = malloc(2 * sizeof(char));
	strcpy(tstr, "T");
	t.type = LITSYMBOL;
	t.symbol.symbol = tstr;
	return t;
}
// make an 'error
obj_t create_error() {
	obj_t err;
	string_t errstr = malloc(6 * sizeof(char));
	strcpy(errstr, "ERROR");
	err.type = LITSYMBOL;
	err.symbol.symbol = errstr;
	return err;
}