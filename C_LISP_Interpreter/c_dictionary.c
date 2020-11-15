#include "c_dictionary.h"
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>

C_DICTIONARY* dict_create()
{
	C_DICTIONARY* dict = (C_DICTIONARY*)malloc(sizeof(C_DICTIONARY));
	dict->count = 0;
	dict->firstNode = NULL;
	return dict;
}

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

void dict_insert(C_DICTIONARY* dict, char* key, obj_t* obj)
{
	C_DICTIONARY_NODE* curNode = malloc(sizeof(C_DICTIONARY_NODE));
	curNode->object = *obj;
	curNode->nextNode = NULL;
	curNode->key = (char*)malloc(sizeof(char) * (strlen(key) + 1));
	strcpy(curNode->key, key);
	if (dict->count == 0) {
		dict->firstNode = curNode;
	}
	else {
		C_DICTIONARY_NODE* node = dict->firstNode;
		while (node->nextNode != NULL)
			node = node->nextNode;
		node->nextNode = curNode;
	}
	dict->count++;
}

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
