#include "lisp_processor.h"
#include "lisp_processor_functions.h"
#include "lisp_processor_utils.h"
#include "c_dictionary.h"
#include <string.h>
#include <ctype.h>

typedef obj_t (*processorFunction)(obj_t* pObj);

// LISP function dictionary
const char* funcNames[LISP_FUNC_COUNT] = { LISP_FUNC_NAMES };
processorFunction funcs[LISP_FUNC_COUNT] = { LISP_FUNCS };
// symbol-to-variable dictionary
C_DICTIONARY* symbolsDict;

// prepare variable dictionary
void initializeProcessor()
{
	symbolsDict = dict_create();
}

// discard variable dictionary
void freeProcessor()
{
	dict_free(symbolsDict);
}

// check if str matches /c[ad]*r/
bool is_cadr_string(char* str) {
	if (strlen(str) < 3)
		return false;
	if (str[0] != 'C')
		return false;

	char c;
	while(1)
		switch (*++str) {
		case 'R':
			return *++str == '\0';
		default: // can handle early \0
			return false;
		case 'A': case 'D':
			break;
		}
}

// evaluate given object
obj_t evaluateObject(const obj_t* pObj) {
	obj_t result = *pObj;
	switch (pObj->type) {
	case CODE: {
		if (pObj->list.value == NULL || pObj->list.value->type != SYMBOL)
			return create_error();
		string_t funcName = pObj->list.value->symbol.symbol;

		// Handle car and cdr separately
		if (is_cadr_string(funcName)) {
			char* c = funcName + strlen(funcName) - 1; // points to 'R'
			if (result.list.next == NULL)
				result = create_error();
			result = *result.list.next->list.value;
			while(1)
				switch (*--c) {
				case 'C':
					return result;
				case 'A':
					result = fn_car(&result);
					break;
				case 'D':
					result = fn_cdr(&result);
					break;
				}
		}
		else {
			for (int i = 0; i < LISP_FUNC_COUNT; i++) {
				if (strcmp(funcName, funcNames[i]) == 0) { // function lookup
					return funcs[i](pObj->list.next);
				}
			}
		}

		obj_t newVal = *pObj;
		newVal.type = LIST;
		return evaluateObject(&newVal);
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
