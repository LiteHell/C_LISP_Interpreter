typedef enum {
	NUMBER,
	SYMBOL,
	CODE,
	LIST,
	NIL
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
	union obj_t *value, *next;
} list_t;

typedef union obj_t {
	type_t type;
	number_t number;
	symbol_t symbol;
	list_t list;
	union {
		type_t type;
		number_t number;
		symbol_t symbol;
		list_t list;
	} any;
} obj_t;