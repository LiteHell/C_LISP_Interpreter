#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "parsedef.h"
#include "lisp_processor_utils.h"

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