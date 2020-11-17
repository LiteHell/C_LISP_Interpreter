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
	obj_t* newList = NULL;
	// �Ű������� ���޹��� ����Ʈ���� �ݺ�
	for (obj_t* listNow = pObj; listNow != NULL; listNow = listNow->list.next) {
		// �Ű����� ��
		obj_t innerList = evaluateObject(listNow->list.value);
		// ���� �򰡰��� ����Ʈ�� �ƴ϶�� ���� ��ȯ
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
			}
			else {
				// ù ������ �ƴ϶�� ����Ʈ�� �ڿ� �߰�
				appendList(newList, innerListNow->list.value);
			}
		}
	}
	if (newList == NULL)
		return create_error();
	else
		return *newList;
}

obj_t fn_length(obj_t* pObj)
{
	// �Ű����� ��
	obj_t evaluatedList = evaluateObject(pObj->list.value);
	// �Ű������� ����Ʈ ������ �ƴ϶�� ���� ��ȯ
	if (evaluatedList.type != LIST)
	{
		return create_error();
	}
	// ������ ������ ����
	int count = 0;
	
	// ������ ����
	for (obj_t* now = &evaluatedList; now != NULL; now = now->list.next) {
		count++;
	}
	// ��� ��ȯ
	return makeNumber(count);
}

obj_t fn_member(obj_t* pObj)
{
	// ù��° �Ű����� ��, ����Ʈ ������ ã�ƾ��� ���̴�.
	obj_t target = evaluateObject(pObj->list.value);
	// �ι�° �Ű����� ��, Ư�� ���� ã�� ����Ʈ�̴�.
	obj_t evaluatedList = evaluateObject(pObj->list.next->list.value);
	// �ι�° �Ű������� ����Ʈ�� �ƴ϶�� ���� ��ȯ
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
	// Ű�� ����� ù��° �Ű����� ��
	obj_t key = evaluateObject(pObj->list.value);
	// DB�� ����� �ι�° �Ű����� ��, ����Ʈ�� �ƴ϶�� ������ ��ȯ�Ѵ�.
	obj_t evaluatedList = evaluateObject(pObj->list.next->list.value);
	if (evaluatedList.type != LIST)
	{
		return create_error();
	}

	
	for (obj_t* now = &evaluatedList; now != NULL; now = now->list.next) {
		// DB ���
		obj_t* entry = now->list.value;
		// DB ����� Ű
		obj_t* entryKey = entry->list.value;
		// DB ����� ��
		obj_t* entryValue = entry->list.next->list.value;
		
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
	// �Ű����� �� �� ���� ����
	obj_t target = evaluateObject(pObj->list.value);
	obj_t evaluatedList = evaluateObject(pObj->list.next->list.value);
	if (evaluatedList.type != LIST)
	{
		return create_error();
	}

	obj_t* head = &evaluatedList, * now = head, * prev = NULL;
	while (now != NULL && head != NULL) {
		if (obj_equals(now->list.value, &target)) {
			// ��ġ�Ѵٸ� �����Ѵ�.
			if (prev == NULL) { // ù ��Ҹ� �����ϴ� �����, head�� cdr�� �����ϴ� �͸����� ������ ȿ���� ��Ÿ����.
				head = now->list.next;
			}
			else { // ù ��Ұ� �ƴ϶��, ��ũ�帮��Ʈ���� �����ϴ� ��İ� ���� �����Ѵ�.
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
	// �Ű����� �� �� ����
	obj_t newValue = evaluateObject(pObj->list.value);
	obj_t oldValue = evaluateObject(pObj->list.next->list.value);
	obj_t evaluatedList = evaluateObject(pObj->list.next->list.next->list.value);
	if (evaluatedList.type != LIST)
	{
		return create_error();
	}

	
	for (obj_t* now = &evaluatedList; now != NULL; now = now->list.next) {
		if (obj_equals(now->list.value, &oldValue)) {
			// ��ġ�Ѵٸ� value�� �ٲ۴�.
			*now->list.value = newValue;
		}
	}
	return evaluatedList;

}
