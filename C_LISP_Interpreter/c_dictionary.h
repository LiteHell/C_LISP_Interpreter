#ifndef _H_C_DICT
#define _H_C_DICT
#include "parsedef.h"
#include <stdbool.h>

typedef struct C_DICTIONARY_NODE* P_DICTIONARY_NODE;
typedef struct {
	obj_t object;
	char* key;
	P_DICTIONARY_NODE nextNode;
} C_DICTIONARY_NODE;

typedef struct {
	C_DICTIONARY_NODE* firstNode;

} C_DICTIONARY;

C_DICTIONARY* dict_create();
void dict_free(C_DICTIONARY* dict);
void dict_insert(C_DICTIONARY* dict, char* key, obj_t* obj);
bool dict_has(C_DICTIONARY* dict, char* key);
obj_t dict_get(C_DICTIONARY* dict, char* key);
#endif