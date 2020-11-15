#include "lisp_processor_functions_symbol.h"
#include "lisp_processor.h"

obj_t fn_setq(obj_t* pObj)
{
	const obj_t* symbol = pObj->list.value;
	const obj_t* value = pObj->list.next->list.value;
	if (!symbol || !value) {
		exit(1);
		return;
	}
	else if (symbol->type != SYMBOL) {
		exit(1);
		return;
	}
	obj_t evaluated = evaluateObject(value);
	dict_insert(symbolsDict, symbol->symbol.symbol, &evaluated);
	return evaluated;
}