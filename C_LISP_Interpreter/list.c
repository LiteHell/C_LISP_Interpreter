#define _CRT_SECURE_NO_WARNINGS
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
	// �򰡰��� LIST ������ �ƴ϶�� ���� ��ȯ
	if (evaluated.type != LIST)
		return create_error();

	// �򰡰��� ù��° ��Ҹ� ��ȯ�Ѵ�.
	return *evaluated.list.next;
}

// NTH : �ι�° �Ű������� ����Ʈ���� (ù��° �Ű�����)��°�� ��Ҹ� ��ȯ�Ѵ�. �ε����� 0���� ���� ���� ����
// (NTH I X)�� X�� CDR�� (I - 1)��ŭ �� ���� CAR���̶� �����ϴٴ� ���̵��� �����ߴ�.
obj_t fn_nth(obj_t* pObj)
{
	// ù��° �Ű������� ���Ѵ�.
	obj_t evaluatedIndex = evaluateObject(pObj->list.value);
	// �ι�° �Ű������� ���ٸ� ������ ��ȯ�Ѵ�.
	if (pObj->list.next == NULL)
		return create_error();

	// �ι�° �Ű������� ���Ѵ�.
	obj_t evaluatedList = evaluateObject(pObj->list.next->list.value);
	// ù��° �Ű������� ���ڰ� �ƴϰ� �ι�° �Ű������� ����Ʈ�� �ƴ϶�� ������ ��ȯ�Ѵ�.
	if (evaluatedIndex.type != NUMBER || evaluatedList.type != LIST)
		return create_error();

	// ù��° �Ű������� ���� �����´�.
	int count = evaluatedIndex.number.value;
	// ������ ����� ����Ʈ, �ι�° �Ű������� �����Ѵ�.
	obj_t* list = &evaluatedList;
	for (int i = 1; i <= count; i++) {
		list = list->list.next; // list = (list�� CDR)�� (count - 1)��ŭ �ݺ��ϰ� �ȴ�.
		if (list == NULL)
			return makeNIL();
	}
	// list�� CAR�� ��ȯ�Ѵ�.
	return *list->list.value;
}

// CONS : �ι�° �Ű�����(����Ʈ ����)�� ù��° �Ű������� ù��° ��ҷ� �߰��� ����Ʈ�� ��ȯ�Ѵ�.
obj_t fn_cons(obj_t* pObj)
{
	// ù��° �Ű������� ���Ѵ�.
	obj_t evaluatedItem = evaluateObject(pObj->list.value);
	// �ι�° �Ű������� ���ٸ� ������ȯ
	if (pObj->list.next == NULL)
		return create_error();
	
	// �ι�° �Ű������� ���Ѵ�.
	obj_t evaluatedList = evaluateObject(pObj->list.next->list.value);
	// �ι�° �Ű������� ����Ʈ�� �ƴ϶�� ���� ��ȯ
	if (evaluatedList.type != LIST)
		return create_error();

	// ���ο� ����Ʈ�� �����Ѵ�.
	obj_t newList;
	newList.type = LIST;
	newList.list.type = LIST;
	newList.list.next = (obj_t*)malloc(sizeof(obj_t));
	newList.list.value = (obj_t*)malloc(sizeof(obj_t));
	// �޸� �Ҵ� ���н� ���� ��ȯ
	if (newList.list.next == NULL || newList.list.value == NULL)
		return create_error();
	// ����Ʈ�� CAR�� ù��° �Ű������� �ϰ�
	*newList.list.value = evaluatedItem;
	// ����Ʈ�� CDR�� �ι�° �Ű������� �Ѵ�.
	*newList.list.next = evaluatedList;
	// ���� ù��° �Ű������� �� ó���� ���Ե� ����Ʈ�� ��������� �ȴ�.

	return newList;
}

// REVERSE : �־��� ����Ʈ�� ������ ���ο� ����Ʈ�� ��ȯ�Ѵ�.
obj_t fn_reverse(obj_t* pObj)
{
	// ù��° �Ű������� ���ϰ�, �򰡰��� ����Ʈ�� �ƴ϶�� ����
	obj_t evaluatedList = evaluateObject(pObj->list.value);
	if (evaluatedList.type != LIST)
		return create_error();

	// ������ ���� ����Ʈ
	obj_t* curList = &evaluatedList;
	// ������ ���� ����Ʈ
	obj_t* newList = NULL;

	while (curList != NULL) {
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
		// curList = (curList�� cdr)
		curList = curList->list.next;
	}

	return *newList;
}

// APPEND : �Ű��������� ���޹��� ����Ʈ���� ���� �����Ͽ� ���� ���ο� ����Ʈ�� ��ȯ�Ѵ�.
obj_t fn_append(obj_t* pObj)
{
	bool firstElement = true;
	obj_t* listNow = pObj;
	obj_t* newList = (obj_t*)malloc(sizeof(obj_t));
	obj_t* newListTail = newList;
	while (listNow != NULL) {
		obj_t list = evaluateObject(listNow->list.value);
		if (list.type != LIST)
		{
			return create_error();
		}
		obj_t* innerListNow = &list;
		while (innerListNow != NULL) {
			if (firstElement) {
				firstElement = false;
			}
			else {
				obj_t* newListNode = (obj_t*)malloc(sizeof(obj_t));
				newListTail->list.next = newListNode;
				newListTail = newListNode;
			}
			newListTail->type = LIST;
			newListTail->list.value = innerListNow->list.value;
			newListTail->list.next = NULL;


			innerListNow = innerListNow->list.next;
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
		return create_error();
	}
	int count = 0;
	obj_t* now = &evaluatedList;
	while (now != NULL) {
		count++;
		now = now->list.next;
	}
	return makeNumber(count);
}

obj_t fn_member(obj_t* pObj)
{
	obj_t target = evaluateObject(pObj->list.value);
	obj_t evaluatedList = evaluateObject(pObj->list.next->list.value);
	if (evaluatedList.type != LIST)
	{
		return create_error();
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
		return create_error();
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
		return create_error();
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
		return create_error();
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
