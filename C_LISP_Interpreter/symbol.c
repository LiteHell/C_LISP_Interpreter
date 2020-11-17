#include "lisp_processor_functions_symbol.h"
#include "lisp_processor.h"
#include "lisp_processor_utils.h"

obj_t fn_setq(obj_t* pObj)
{
	const obj_t* symbol = pObj->list.value;
	const obj_t* value = pObj->list.next->list.value;
	if (!symbol || !value)
		return create_error();
	else if (symbol->type != SYMBOL)
		return create_error();
	obj_t evaluated = evaluateObject(value);
	dict_insert(symbolsDict, symbol->symbol.symbol, &evaluated);
	return evaluated;
}

obj_t fn_list(obj_t* pObj)
{
	obj_t result = *pObj;
	result.type = LIST;
	result = evaluateObject(&result);
	return result;
}
