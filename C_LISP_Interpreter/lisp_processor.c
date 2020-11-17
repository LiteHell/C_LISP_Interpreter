#include "lisp_processor.h"
#include "lisp_processor_functions.h"
#include "c_dictionary.h"
#include <string.h>
#include <ctype.h>

typedef obj_t (*processorFunction)(obj_t* pObj);

const char* funcNames[LISP_FUNC_COUNT] = { LISP_FUNC_NAMES };
processorFunction funcs[LISP_FUNC_COUNT] = { LISP_FUNCS };
C_DICTIONARY* symbolsDict;

void initializeProcessor()
{
	symbolsDict = dict_create();
}

void freeProcessor()
{
	dict_free(symbolsDict);
}

bool is_cadr_string(char* str) {
	if (strlen(str) < 3)
		return false;
	if (str[0] != 'c' && str[0] != 'C')
		return false;

	char c;
	while (c = tolower(*(++str))) {
		if (c == 'r')
			return *(++str) == '\0';
		else if (c != 'a' && c != 'd')
			return false;
	}
	return false;
}

obj_t evaluateObject(const obj_t* pObj) {
	obj_t result = *pObj;
	switch (pObj->type) {
	case CODE: {
		string_t funcName = pObj->list.value->symbol.symbol;
		for (int i = 0; i < LISP_FUNC_COUNT; i++) {
			// Handle car and cdr separately
			if (is_cadr_string(funcName)) {
				char* c = funcName + strlen(funcName) - 1;
				result = *result.list.next->list.value;
				while (tolower(*(--c)) != 'c') {
					if (tolower(*c) == 'a')
						result = fn_car(&result);
					else if (tolower(*c) == 'd')
						result = fn_cdr(&result);
				}
				return result;
			}

			// All symbols are uppercased, so no need of case insensitive strcmp
			if (strcmp(funcName, funcNames[i]) == 0) {
				obj_t parameters = *pObj->list.next;
				parameters.type = LIST;
				return (funcs[i](&parameters));
			}
		}
		obj_t newVal = *pObj;
		newVal.type = LIST;
		return evaluateObject(&newVal);
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

