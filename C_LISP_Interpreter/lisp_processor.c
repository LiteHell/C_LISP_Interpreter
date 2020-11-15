#include "lisp_processor.h"
#include "lisp_processor_functions.h"
#include <string.h>

typedef obj_t (*processorFunction)(obj_t* pObj);

#define FUNC_COUNT 4
const char* funcNames[FUNC_COUNT] = { "+", "-", "/", "*" };
processorFunction funcs[FUNC_COUNT] = { fn_add, fn_subtract, fn_divide, fn_multiply };

obj_t evaluateObject(const obj_t* pObj) {
	obj_t result = *pObj;
	switch (pObj->type) {
	case CODE: {
		string_t funcName = pObj->list.value->symbol.symbol;
		for (int i = 0; i < FUNC_COUNT; i++) {
			if (strcmp(funcName, funcNames[i]) == 0) {
				return (funcs[i](pObj->list.next));
			}
		}
		break;
	}
	case LIST: {
		obj_t* list = &result;
		while (list) {
			obj_t evaluated = evaluateObject(list->list.value);
			list->list.value->any = evaluated.any;
			list->list.value->list = evaluated.list;
			list->list.value->number = evaluated.number;
			list->list.value->symbol = evaluated.symbol;
			list->list.value->type = evaluated.type;

			list = list->list.next;
		}
		break;
	}
	case NUMBER:
	case SYMBOL:
	case NIL:
	default:
		// Do nothing
		break;
	}
	return result;
}

