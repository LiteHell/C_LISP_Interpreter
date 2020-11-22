#include "lisp_processor_functions_symbol.h"
#include "lisp_processor.h"
#include "lisp_processor_utils.h"

obj_t fn_setq(obj_t* pObj)
{
	int argc = argument_count(pObj);
	if (argc == 0)
		return makeNIL();

	while (1) { // allow multiple definition
		if (argc == 1)
			return create_error();

		const obj_t* symbol = pObj->list.value;
		const obj_t* value = pObj->list.next->list.value;
		if (symbol->type != SYMBOL) // LITSYMBOL is not allowed
			return create_error();

		obj_t* evaluated = malloc(sizeof(obj_t));
		if (evaluated == NULL)
			return create_error();
		*evaluated = evaluateObject(value);
		dict_insert(symbolsDict, symbol->symbol.symbol, evaluated);

		if(argc == 2)
			return *evaluated;
		pObj = pObj->list.next->list.next;
		argc -= 2;
	}
}

obj_t fn_list(obj_t* pObj)
{
	if (pObj == NULL)
		return makeNIL();

	obj_t result = *pObj;
	result.type = LIST;
	result = evaluateObject(&result);
	return result;
}
