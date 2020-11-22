#include "c_dictionary.h"
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>

// make a new list-based dictionary
C_DICTIONARY* dict_create()
{
	C_DICTIONARY* dict = malloc(sizeof(C_DICTIONARY));
	dict->count = 0;
	dict->firstNode = NULL;
	return dict;
}

// free a dictionary
void dict_free(C_DICTIONARY* dict)
{
	if (dict->count != 0) {
		C_DICTIONARY_NODE* node = dict->firstNode;
		while (node != NULL) {
			C_DICTIONARY_NODE* nextNode = node->nextNode;
			free(node->key);
			node = nextNode;
		}
	}
	free(dict);
}

// append a key-value pair at the end
void dict_insert(C_DICTIONARY* dict, char* key, obj_t* obj)
{
	C_DICTIONARY_NODE* curNode = malloc(sizeof(C_DICTIONARY_NODE));
	curNode->object = *obj;
	curNode->nextNode = NULL;
	curNode->key = malloc(sizeof(char) * (strlen(key) + 1));
	strcpy(curNode->key, key);

	if (dict->count == 0) {
		dict->firstNode = curNode;
	}
	else {
		C_DICTIONARY_NODE* node = dict->firstNode;
		while (node->nextNode != NULL) {
			if (strcmp(curNode->key, key) == 0) {
				node->object = curNode->object;
				free(curNode->key);
				free(curNode);
				return;
			}
			node = node->nextNode;
		}
		node->nextNode = curNode;
	}
	dict->count++;
}

// checks if dictionary has given key
bool dict_has(C_DICTIONARY* dict, char* key)
{
	if (dict->count == 0)
		return false;
	C_DICTIONARY_NODE* node = dict->firstNode;
	while (node != NULL) {
		if (strcmp(node->key, key) == 0)
			return true;
		node = node->nextNode;
	}
	return false;
}

// retrieves a value corresponding to given key
obj_t dict_get(C_DICTIONARY* dict, char* key)
{
	if (dict->count == 0) {
		perror("Empty dictionary");
		exit(1);
		return;
	}
	C_DICTIONARY_NODE* node = dict->firstNode;
	while (node != NULL) {
		if (strcmp(node->key, key) == 0)
			return node->object;
		node = node->nextNode;
	}
	perror("Not found on dictionary");
	exit(1);
	return;

}
