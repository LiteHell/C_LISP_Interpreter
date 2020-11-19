#ifndef _H_PARSEDEF
#define _H_PARSEDEF

typedef enum {
	NUMBER,
	SYMBOL,
	LITSYMBOL,
	STRING,
	CODE,
	LIST,
	NIL,
	_EXPLICIT_LITERAL_LIST
} type_t;

union obj_t;

typedef char *string_t;
typedef struct {
	type_t type;
	string_t literal;
	double value;
} number_t;
typedef struct {
	type_t type;
	string_t symbol;
} symbol_t;
typedef struct {
	type_t type;
	string_t string;
} strobj_t;
typedef struct {
	type_t type;
	union obj_t *value, *next;
} list_t;

typedef union obj_t {
	type_t type;
	number_t number;
	symbol_t symbol;
	strobj_t string;
	list_t list;
	union {
		type_t type;
		number_t number;
		symbol_t symbol;
		strobj_t string;
		list_t list;
	} any;
} obj_t;
#endif