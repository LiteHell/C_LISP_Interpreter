#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include "lisp_processor_functions_conditional.h"
bool is_t(obj_t* obj) {
	return (obj->type == SYMBOL || obj->type == LITSYMBOL) && !strcmp(obj->symbol.symbol, "T");
}
bool is_error(obj_t* obj) {
	return (obj->type == SYMBOL || obj->type == LITSYMBOL) && !strcmp(obj->symbol.symbol, "ERROR");
}
obj_t create_t() {
	obj_t t;
	string_t tstr = malloc(2 * sizeof(char));
	strcpy(tstr, "T");
	t.type = LITSYMBOL;
	t.symbol.symbol = tstr;
	return t;
}
obj_t create_error() {
	obj_t err;
	string_t errstr = malloc(6 * sizeof(char));
	strcpy(errstr, "ERROR");
	err.type = LITSYMBOL;
	err.symbol.symbol = errstr;
	return err;
}