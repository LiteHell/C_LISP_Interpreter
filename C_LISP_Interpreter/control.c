#include "lisp_processor_functions_control.h"
#include "lisp_processor_utils.h"

obj_t fn_if(obj_t* pObj) {
	obj_t condition = evaluateObject(pObj->list.value);
	if (pObj->list.next == NULL)
		return create_error();

	if (is_t(&condition)) {
		return evaluateObject(pObj->list.next->list.value);
	}
	else if (pObj->list.next->list.next != NULL) {
		return evaluateObject(pObj->list.next->list.next->list.value);
	}
	else {
		return create_nil();
	}
}
obj_t fn_cond(obj_t* pObj) {
	obj_t* list = pObj;
	while (list != NULL) {
		obj_t caseList = evaluateObject(list->list.value);
		if (caseList.type != LIST)
			return create_error();
		obj_t condition = evaluateObject(caseList.list.value);
		if (is_t(&condition)) {
			if (caseList.list.next == NULL)
				return create_error();
			return evaluateObject(caseList.list.next->list.value);
		}

		list = list->list.next;
	}
	return create_nil();
}