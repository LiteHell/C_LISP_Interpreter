#define _CRT_SECURE_NO_WARNINGS
#include "lisp_processor_functions_list.h"
#include "lisp_processor_utils.h"
#include "lisp_processor.h"

obj_t makeNIL() {
	obj_t obj;
	obj.type = NIL;
	return obj;
}

obj_t fn_car(obj_t* pObj)
{
	obj_t evaluated = evaluateObject(pObj);
	if (evaluated.type != LIST) {
		exit(1); return;
	}
	return *evaluated.list.value;
}

obj_t fn_cdr(obj_t* pObj)
{
	obj_t evaluated = evaluateObject(pObj);
	if (evaluated.type != LIST) {
		exit(1); return;
	}
	return *evaluated.list.next;
}

obj_t fn_nth(obj_t* pObj)
{
	obj_t evaluatedCount = evaluateObject(pObj->list.value);
	obj_t evaluatedList = evaluateObject(pObj->list.next->list.value);
	if (evaluatedCount.type != NUMBER || evaluatedList.type != LIST) {
		exit(1); return;
	}
	int count = evaluatedCount.number.value;
	obj_t* list = &evaluatedList;
	for (int i = 1; i <= count; i++) {
		list = list->list.next;
		if (list == NULL) {
			return makeNIL();
		}
	}
	return *list->list.value;
}

obj_t fn_cons(obj_t* pObj)
{
	obj_t evaluatedItem = evaluateObject(pObj->list.value);
	obj_t evaluatedList = evaluateObject(pObj->list.next->list.value);
	if (evaluatedList.type != LIST) {
		exit(1); return;
	}

	obj_t newList;
	newList.type = LIST;
	newList.list.type = LIST;
	newList.list.next = (obj_t*)malloc(sizeof(obj_t));
	newList.list.value = (obj_t*)malloc(sizeof(obj_t));
	if (newList.list.next == NULL || newList.list.value == NULL)
	{
		exit(1); return;
	}
	*newList.list.value = evaluatedItem;
	*newList.list.next = evaluatedList;
	return newList;
}

obj_t fn_reverse(obj_t* pObj)
{
	obj_t evaluatedList = evaluateObject(pObj->list.value);
	if (evaluatedList.type != LIST) {
		exit(1); return;
	}

	obj_t* curList = &evaluatedList;
	obj_t* newList = NULL;

	while (curList != NULL) {
		if (newList == NULL) {
			newList = (obj_t*)malloc(sizeof(obj_t));
			if (newList == NULL) { exit(1); return; }
			newList->type = LIST;
			newList->list.type = LIST;
			newList->list.next = NULL;
			newList->list.value = curList->list.value;
		}
		else {
			obj_t* nextList = newList;
			newList = (obj_t*)malloc(sizeof(obj_t));
			if (newList == NULL) { exit(1); return; }
			newList->type = LIST;
			newList->list.type = LIST;
			newList->list.next = nextList;
			newList->list.value = curList->list.value;
		}
		curList = curList->list.next;
	}

	return *newList;
}

obj_t fn_append(obj_t* pObj)
{
	bool firstElement = true;
	obj_t* listNow = pObj;
	obj_t* newList = (obj_t*)malloc(sizeof(obj_t));
	obj_t* newListTail = newList;
	while (listNow != NULL) {
		obj_t list = evaluateObject(listNow->list.value);
		obj_t* listNowEtc = &list;
		if (list.type != LIST)
		{
			exit(1); return;
		}
		while (listNowEtc != NULL) {
			if (firstElement) {
				firstElement = false;
			}
			else {
				obj_t* newListNode = (obj_t*)malloc(sizeof(obj_t));
				newListTail->list.next = newListNode;
				newListTail = newListNode;
			}
			newListTail->type = LIST;
			newListTail->list.value = listNowEtc->list.value;
			newListTail->list.next = NULL;


			listNowEtc = listNowEtc->list.next;
		}

		listNow = listNow->list.next;
	}
	return *newList;
}

obj_t fn_length(obj_t* pObj)
{
	obj_t evaluatedList = evaluateObject(pObj->list.value);
	if (evaluatedList.type != LIST)
	{
		exit(1); return;
	}
	int count = 0;
	obj_t* now = &evaluatedList;
	while (now != NULL) {
		count++;
		now = now->list.next;
	}
	obj_t result;
	result.type = NUMBER;
	result.number.type = NUMBER;
	result.number.value = count;
	result.number.literal = malloc(100);
	sprintf(result.number.literal, "%lf", result.number.value);
	result.number.literal = realloc(result.number.literal, strlen(result.number.literal) + 1);
	return result;
}

obj_t fn_member(obj_t* pObj)
{
	obj_t target = evaluateObject(pObj->list.value);
	obj_t evaluatedList = evaluateObject(pObj->list.next->list.value);
	if (evaluatedList.type != LIST)
	{
		exit(1); return;
	}
	obj_t* now = &evaluatedList;
	while (now != NULL) {
		if (obj_equals(now->list.value, &target)) {
			return *now;
		}
		now = now->list.next;
	}
	return makeNIL();

}

obj_t fn_assoc(obj_t* pObj)
{
	obj_t key = evaluateObject(pObj->list.value);
	obj_t evaluatedList = evaluateObject(pObj->list.next->list.value);
	if (evaluatedList.type != LIST)
	{
		exit(1); return;
	}
	obj_t* now = &evaluatedList;
	while (now != NULL) {
		obj_t* entry = now->list.value;
		obj_t* entryKey = entry->list.value;
		obj_t* entryValue = entry->list.next->list.value;
		if (obj_equals(entryKey, &key)) {
			return *entry;
		}
		now = now->list.next;
	}
	return makeNIL();
}

obj_t fn_remove(obj_t* pObj)
{
	obj_t target = evaluateObject(pObj->list.value);
	obj_t evaluatedList = evaluateObject(pObj->list.next->list.value);
	if (evaluatedList.type != LIST)
	{
		exit(1); return;
	}
	obj_t* head = &evaluatedList, * now = head, * prev = NULL;
	while (now != NULL && head != NULL) {
		if (obj_equals(now->list.value, &target)) {
			// Remove it
			if (prev == NULL) {
				head = now->list.next;
			}
			else {
				prev->list.next = now->list.next;
				now = prev;
			}
		}
		prev = now;
		now = now->list.next;
	}
	if (head == NULL)
		return makeNIL();
	else
		return *head;
}

obj_t fn_subst(obj_t* pObj)
{
	obj_t newValue = evaluateObject(pObj->list.value);
	obj_t oldValue = evaluateObject(pObj->list.next->list.value);
	obj_t evaluatedList = evaluateObject(pObj->list.next->list.next->list.value);
	if (evaluatedList.type != LIST)
	{
		exit(1); return;
	}
	obj_t *now = &evaluatedList;
	while (now != NULL) {
		if (obj_equals(now->list.value, &oldValue)) {
			*now->list.value = newValue;
		}
		now = now->list.next;
	}
	return evaluatedList;

}
