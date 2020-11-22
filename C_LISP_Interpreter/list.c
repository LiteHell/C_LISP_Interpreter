#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include "lisp_processor_functions_list.h"
#include "lisp_processor_utils.h"
#include "lisp_processor.h"

// LISP �Լ����� �Ű������� CODE�� CDR�� ���޹޴´�. ���ø� ��� (APPEND '(A B) '(C D) '(E F))�� �����Ѵٸ�, fn_append()�� �Ű������� ('(A B) '(C D) '(E F))�� ���޵ȴ�.
// �ٸ� ���������� car, cdr �Լ��� �ٸ� �Լ��� �޸� CODE�� CADR�� ���޹޴´�. ���ø� ��� (CAR '(A B C))�� �����Ѵٸ�, fn_car()�� �Ű������� ('(A B C))�� �ƴ� '(A B C)�� ���޵ȴ�.

// CAR : ����Ʈ�� ù��° ��Ҹ� ��ȯ�Ѵ�.
obj_t fn_car(obj_t* pObj)
{
	// ���޹��� �Ű������� �򰡰��� ���Ѵ�.
	obj_t evaluated = evaluateObject(pObj);
	if (evaluated.type == NIL)
		return makeNIL();
	// �򰡰��� LIST ������ �ƴ϶�� ���� ��ȯ
	if (evaluated.type != LIST)
		return create_error();
	
	// �򰡰��� ù��° ��Ҹ� ��ȯ�Ѵ�.
	return *evaluated.list.value;
}

// CDR : ����Ʈ�� ù��°�� �ƴ� ������ ��ҵ�� �̷���� ���ο� ����Ʈ�� �����Ѵ�.
obj_t fn_cdr(obj_t* pObj)
{
	// ���޹��� �Ű������� �򰡰��� ���Ѵ�.
	obj_t evaluated = evaluateObject(pObj);
	if (evaluated.type == NIL)
		return makeNIL();
	// �򰡰��� LIST ������ �ƴ϶�� ���� ��ȯ
	if (evaluated.type != LIST)
		return create_error();

	// �򰡰��� ������ ��Ҹ� ��ȯ�Ѵ�.
	return evaluated.list.next == NULL ? makeNIL() : *evaluated.list.next;
}

// NTH : �ι�° �Ű������� ����Ʈ���� (ù��° �Ű�����)��°�� ��Ҹ� ��ȯ�Ѵ�. �ε����� 0���� ���� ���� ����
// (NTH I X)�� X�� CDR�� I�� �� ���� CAR���̶� �����ϴٴ� ���̵��� �����ߴ�.
obj_t fn_nth(obj_t* pObj)
{
	if (argument_count(pObj) != 2)
		return create_error();
	// ù��° �Ű������� ���Ѵ�.
	obj_t evaluatedIndex = evaluateObject(pObj->list.value);

	// �ι�° �Ű������� ���Ѵ�.
	obj_t evaluatedList = evaluateObject(pObj->list.next->list.value);
	// ù��° �Ű������� ������ �ƴϰų� �ι�° �Ű������� ����Ʈ�� �ƴ϶�� ������ ��ȯ�Ѵ�.
	double countd = evaluatedIndex.number.value;
	if (evaluatedIndex.type != NUMBER || floor(countd) != countd || countd < 0 || (evaluatedList.type != LIST && evaluatedList.type != NIL))
		return create_error();
	if (evaluatedList.type == NIL)
		return makeNIL();

	// ù��° �Ű������� ���� �����´�.
	int count = countd;
	// ������ ����� ����Ʈ, �ι�° �Ű������� �����Ѵ�.
	obj_t* list = &evaluatedList;
	for (int i = 0; i < count; i++) {
		list = list->list.next; // list = (list�� CDR)�� count��ŭ �ݺ��ϰ� �ȴ�.
		if (list == NULL)
			return makeNIL();
	}
	// list�� CAR�� ��ȯ�Ѵ�.
	return *list->list.value;
}

// CONS : �ι�° �Ű�����(����Ʈ ����)�� ù��° �Ű������� ù��° ��ҷ� �߰��� ����Ʈ�� ��ȯ�Ѵ�.
obj_t fn_cons(obj_t* pObj)
{
	if (argument_count(pObj) != 2)
		return create_error();

	obj_t* itemPtr, * listPtr;
	// ù��° �Ű������� ���Ѵ�.
	obj_t evaluatedItem = evaluateObject(pObj->list.value);
	// �ι�° �Ű������� ���Ѵ�.
	obj_t evaluatedList = evaluateObject(pObj->list.next->list.value);
	// �ι�° �Ű������� ����Ʈ�� �ƴ϶�� ���� ��ȯ
	if (evaluatedList.type == NIL) {
		itemPtr = malloc(sizeof(obj_t));
		if (itemPtr == NULL)
			return create_error();
		*itemPtr = evaluatedItem;
		return *makeListWithValue(itemPtr);
	}
	if (evaluatedList.type != LIST)
		return create_error();

	itemPtr = malloc(sizeof(obj_t));
	if (itemPtr == NULL)
		return create_error();
	listPtr = malloc(sizeof(obj_t));
	if (listPtr == NULL)
		return create_error();
	*itemPtr = evaluatedItem;
	*listPtr = evaluatedList;
	obj_t* newList = prependList(listPtr, itemPtr);
	return newList == NULL ? create_error() : *newList;
}

// REVERSE : �־��� ����Ʈ�� ������ ���ο� ����Ʈ�� ��ȯ�Ѵ�.
obj_t fn_reverse(obj_t* pObj)
{
	if (argument_count(pObj) != 1)
		return create_error();
	// ù��° �Ű������� ���ϰ�, �򰡰��� ����Ʈ�� �ƴ϶�� ����
	obj_t evaluatedList = evaluateObject(pObj->list.value);
	if (evaluatedList.type == NIL)
		return makeNIL();
	if (evaluatedList.type != LIST)
		return create_error();

	// ������ ���� ����Ʈ
	obj_t* newList = NULL;

	// ������ ���� ����Ʈ(curList)������ �ݺ��Ѵ�.
	for (obj_t* curList = &evaluatedList; curList != NULL; curList = curList->list.next) {
		if (newList == NULL) {
			// ù �������, curList�� car���� ���� ����Ʈ�� ������ newList�� �ִ´�.
			newList = makeListWithValue(curList->list.value);
		}
		else {
			// ù ������ �ƴ϶��, newList�� �տ� curList�� car���� �߰��Ѵ�.
			newList = prependList(newList, curList->list.value);
		}
		if (newList == NULL)
			return create_error();
	}
	return *newList;
}

// APPEND : �Ű��������� ���޹��� ����Ʈ���� ���� �����Ͽ� ���� ���ο� ����Ʈ�� ��ȯ�Ѵ�.
obj_t fn_append(obj_t* pObj)
{
	if (argument_count(pObj) == 1 && pObj->list.value->type != LIST) // �Ű������� 1���� ��� ����Ʈ�� �ƴ� �� ���
		return *pObj->list.value;

	obj_t* newList = NULL, * newTail = NULL;
	// �Ű������� ���޹��� ����Ʈ���� �ݺ�
	for (obj_t* listNow = pObj; listNow != NULL; listNow = listNow->list.next) {
		// �Ű����� ��
		obj_t innerList = evaluateObject(listNow->list.value);
		// ���� �򰡰��� ����Ʈ�� �ƴ϶�� ���� ��ȯ
		if (innerList.type == NIL)
			continue;
		if (innerList.type != LIST)
		{
			return create_error();
		}

		// ����Ʈ ���θ� �ݺ�
		for (obj_t* innerListNow = &innerList; innerListNow != NULL; innerListNow = innerListNow->list.next) {
			if (newList == NULL) {
				// ù ������� ����Ʈ ����
				newList = makeListWithValue(innerListNow->list.value);
				// �޸� �Ҵ���н� ���� ��ȯ
				if (newList == NULL)
					return create_error();
				newTail = newList;
			}
			else {
				// ù ������ �ƴ϶�� ����Ʈ�� �ڿ� �߰�
				newTail = appendList(newTail, innerListNow->list.value);
				if(newTail == NULL)
					return create_error();
			}
		}
	}
	return newList == NULL ? makeNIL() : *newList;
}

obj_t fn_length(obj_t* pObj)
{
	if (argument_count(pObj) != 1)
		return create_error();

	// �Ű����� ��
	obj_t evaluated = evaluateObject(pObj->list.value);
	switch (evaluated.type) {
	case NIL:
		return makeNumber(0);
	case LIST: {
		// ������ ������ ����
		int count = 0;

		// ������ ����
		for (obj_t* now = &evaluated; now != NULL; now = now->list.next) {
			count++;
		}
		// ��� ��ȯ
		return makeNumber(count);
	}
	case STRING:
		return makeNumber(strlen(evaluated.string.string));
	default:
		return create_error();
	}
}

obj_t fn_member(obj_t* pObj)
{
	if (argument_count(pObj) != 2)
		return create_error();

	// ù��° �Ű����� ��, ����Ʈ ������ ã�ƾ��� ���̴�.
	obj_t target = evaluateObject(pObj->list.value);
	// �ι�° �Ű����� ��, Ư�� ���� ã�� ����Ʈ�̴�.
	obj_t evaluatedList = evaluateObject(pObj->list.next->list.value);
	// �ι�° �Ű������� ����Ʈ�� �ƴ϶�� ���� ��ȯ
	if (evaluatedList.type == NIL)
		return makeNIL();
	if (evaluatedList.type != LIST)
	{
		return create_error();
	}
	
	for (obj_t* now = &evaluatedList; now != NULL; now = now->list.next) {
		// ã�Ҵٸ� ���� ����Ʈ�� ��ȯ
		if (obj_equals(now->list.value, &target)) {
			return *now;
		}
	}
	// �� ã�Ҵٸ� nil ��ȯ
	return makeNIL();
}

obj_t fn_assoc(obj_t* pObj)
{
	if (argument_count(pObj) != 2)
		return create_error();

	// Ű�� ����� ù��° �Ű����� ��
	obj_t key = evaluateObject(pObj->list.value);
	// DB�� ����� �ι�° �Ű����� ��, ����Ʈ�� �ƴ϶�� ������ ��ȯ�Ѵ�.
	obj_t evaluatedList = evaluateObject(pObj->list.next->list.value);
	if (evaluatedList.type == NIL)
		return makeNIL();
	if (evaluatedList.type != LIST)
	{
		return create_error();
	}

	for (obj_t* now = &evaluatedList; now != NULL; now = now->list.next) {
		// DB ���
		obj_t* entry = now->list.value;
		if (entry->type == NIL)
			continue;
		if (entry->type != LIST)
			return create_error();
		// DB ����� Ű
		obj_t* entryKey = entry->list.value;
		
		// DB ����� Ű�� ���޹��� Ű�� ��ġ�Ѵٸ� �ش� ��Ҹ� ��ȯ�Ѵ�.
		if (obj_equals(entryKey, &key)) {
			return *entry;
		}
	}

	// ã�� ���ߴٸ� NIL�� ��ȯ�Ѵ�.
	return makeNIL();
}

obj_t fn_remove(obj_t* pObj)
{
	if (argument_count(pObj) != 2)
		return create_error();

	// �Ű����� �� �� ���� ����
	obj_t target = evaluateObject(pObj->list.value);
	obj_t evaluatedList = evaluateObject(pObj->list.next->list.value);
	if (evaluatedList.type == NIL)
		return makeNIL();
	if (evaluatedList.type != LIST)
	{
		return create_error();
	}

	// ���� ����Ʈ�� �������� ���ƾ� ��
	obj_t* newList = NULL, * newTail = NULL;
	for (obj_t* now = &evaluatedList; now != NULL; now = now->list.next) {
		if(!obj_equals(now->list.value, &target))
			if (newList == NULL) {
				newList = makeListWithValue(now->list.value);
				if (newList == NULL)
					return create_error();
				newTail = newList;
			}
			else {
				newTail = appendList(newTail, now->list.value);
				if (newTail == NULL)
					return create_error();
			}
	}

	return newList == NULL ? makeNIL() : *newList;
}

obj_t fn_subst(obj_t* pObj)
{
	if (argument_count(pObj) != 3)
		return create_error();

	// �Ű����� �� �� ���� ����
	obj_t newValue = evaluateObject(pObj->list.value);
	obj_t oldValue = evaluateObject(pObj->list.next->list.value);
	obj_t evaluatedList = evaluateObject(pObj->list.next->list.next->list.value);
	if (evaluatedList.type == NIL)
		return makeNIL();
	if (evaluatedList.type != LIST)
	{
		return create_error();
	}

	// ���� ����Ʈ�� �������� ���ƾ� ��
	obj_t* newList = NULL, * newTail = NULL;
	for (obj_t* now = &evaluatedList; now != NULL; now = now->list.next) {
		obj_t* target = now->list.value;
		if (obj_equals(now->list.value, &oldValue))
			target = &newValue;
		if (newList == NULL) {
			newList = makeListWithValue(target);
			if (newList == NULL)
				return create_error();
			newTail = newList;
		}
		else {
			newTail = appendList(newTail, target);
			if (newTail == NULL)
				return create_error();
		}
	}

	return newList == NULL ? makeNIL() : *newList;
}
