#include "lisp_processor.h"
#include "lisp_processor_functions.h"
#include "c_dictionary.h"
#include <string.h>
#include <ctype.h>

typedef obj_t (*processorFunction)(obj_t* pObj);

#define FUNC_COUNT 5
const char* funcNames[FUNC_COUNT] = { "+", "-", "/", "*", "setq" };
processorFunction funcs[FUNC_COUNT] = { fn_add, fn_subtract, fn_divide, fn_multiply, fn_setq };
C_DICTIONARY* symbolsDict;

void initializeProcessor()
{
	symbolsDict = dict_create();
}

void freeProcessor()
{
	dict_free(symbolsDict);
}

bool lowercased_strcmp(char* a, char* b) {
	char ca = 'a', cb = 'a';
	if (strlen(a) != strlen(b))
		return false;
	for (; ca != NULL; a++, b++) {
		ca = *a;
		cb = *b;
		if (tolower(ca) != tolower(cb))
			return false;
	}
	return true;
}

obj_t evaluateObject(const obj_t* pObj) {
	obj_t result = *pObj;
	switch (pObj->type) {
	case CODE: {
		string_t funcName = pObj->list.value->symbol.symbol;
		for (int i = 0; i < FUNC_COUNT; i++) {
			if (lowercased_strcmp(funcName, funcNames[i])) {
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
	case SYMBOL: {
		if (dict_has(symbolsDict, result.symbol.symbol))
			return dict_get(symbolsDict, result.symbol.symbol);
		break;
	}
	case NUMBER:
	case LITSYMBOL:
	case NIL:
	default:
		// Do nothing
		break;
	}
	return result;
}

