#define _CRT_SECURE_NO_WARNINGS
#include "lisp_processor_functions_list.h"
#include "lisp_processor_utils.h"
#include "lisp_processor.h"

// LISP 함수들은 매개변수로 CODE의 CDR를 전달받는다. 예시를 들어 (APPEND '(A B) '(C D) '(E F))를 실행한다면, fn_append()에 매개변수로 ('(A B) '(C D) '(E F))가 전달된다.
// 다만 예외적으로 car, cdr 함수는 다른 함수와 달리 CODE의 CADR를 전달받는다. 예시를 들어 (CAR '(A B C))를 실행한다면, fn_car()에 매개변수로 ('(A B C))가 아닌 '(A B C)가 전달된다.

// CAR : 리스트의 첫번째 요소를 반환한다.
obj_t fn_car(obj_t* pObj)
{
	// 전달받은 매개변수의 평가값을 구한다.
	obj_t evaluated = evaluateObject(pObj);
	// 평가값이 LIST 형식이 아니라면 오류 반환
	if (evaluated.type != LIST)
		return create_error();
	
	// 평가값의 첫번째 요소를 반환한다.
	return *evaluated.list.value;
}

// CDR : 리스트의 첫번째가 아닌 나머지 요소들로 이루어진 새로운 리스트를 반한한다.
obj_t fn_cdr(obj_t* pObj)
{
	// 전달받은 매개변수의 평가값을 구한다.
	obj_t evaluated = evaluateObject(pObj);
	// 평가값이 LIST 형식이 아니라면 오류 반환
	if (evaluated.type != LIST)
		return create_error();

	// 평가값의 첫번째 요소를 반환한다.
	return *evaluated.list.next;
}

// NTH : 두번째 매개변수의 리스트에서 (첫번째 매개변수)번째의 요소를 반환한다. 인덱스는 0부터 세는 것을 주의
// (NTH I X)는 X에 CDR을 (I - 1)만큼 한 값의 CAR값이랑 동일하다는 아이디어로 구현했다.
obj_t fn_nth(obj_t* pObj)
{
	// 첫번째 매개변수를 평가한다.
	obj_t evaluatedIndex = evaluateObject(pObj->list.value);
	// 두번째 매개변수가 없다면 오류를 반환한다.
	if (pObj->list.next == NULL)
		return create_error();

	// 두번째 매개변수를 평가한다.
	obj_t evaluatedList = evaluateObject(pObj->list.next->list.value);
	// 첫번째 매개변수가 숫자가 아니고 두번째 매개변수가 리스트가 아니라면 오류를 반환한다.
	if (evaluatedIndex.type != NUMBER || evaluatedList.type != LIST)
		return create_error();

	// 첫번째 매개변수의 값을 가져온다.
	int count = evaluatedIndex.number.value;
	// 루프에 사용할 리스트, 두번째 매개변수를 대입한다.
	obj_t* list = &evaluatedList;
	for (int i = 1; i <= count; i++) {
		list = list->list.next; // list = (list의 CDR)을 (count - 1)만큼 반복하게 된다.
		if (list == NULL)
			return makeNIL();
	}
	// list의 CAR을 반환한다.
	return *list->list.value;
}

// CONS : 두번째 매개변수(리스트 형식)에 첫번째 매개변수를 첫번째 요소로 추가한 리스트를 반환한다.
obj_t fn_cons(obj_t* pObj)
{
	// 첫번째 매개변수를 평가한다.
	obj_t evaluatedItem = evaluateObject(pObj->list.value);
	// 두번째 매개변수가 없다면 오류반환
	if (pObj->list.next == NULL)
		return create_error();
	
	// 두번째 매개변수를 평가한다.
	obj_t evaluatedList = evaluateObject(pObj->list.next->list.value);
	// 두번째 매개변수가 리스트가 아니라면 오류 반환
	if (evaluatedList.type != LIST)
		return create_error();

	// 새로운 리스트를 생성한다.
	obj_t newList;
	newList.type = LIST;
	newList.list.type = LIST;
	newList.list.next = (obj_t*)malloc(sizeof(obj_t));
	newList.list.value = (obj_t*)malloc(sizeof(obj_t));
	// 메모리 할당 실패시 오류 반환
	if (newList.list.next == NULL || newList.list.value == NULL)
		return create_error();
	// 리스트의 CAR을 첫번째 매개변수로 하고
	*newList.list.value = evaluatedItem;
	// 리스트의 CDR을 두번째 매개변수로 한다.
	*newList.list.next = evaluatedList;
	// 따라서 첫번째 매개변수가 맨 처음에 삽입된 리스트가 만들어지게 된다.

	return newList;
}

// REVERSE : 주어진 리스트를 뒤집은 새로운 리스트를 반환한다.
obj_t fn_reverse(obj_t* pObj)
{
	// 첫번째 매개변수를 평가하고, 평가값이 리스트가 아니라면 에러
	obj_t evaluatedList = evaluateObject(pObj->list.value);
	if (evaluatedList.type != LIST)
		return create_error();

	// 뒤집기 전의 리스트
	obj_t* curList = &evaluatedList;
	// 뒤집은 후의 리스트
	obj_t* newList = NULL;

	while (curList != NULL) {
		if (newList == NULL) {
			// 첫 루프라면, curList의 car값을 가진 리스트를 생성해 newList에 넣는다.
			newList = makeListWithValue(curList->list.value);
		}
		else {
			// 첫 루프가 아니라면, newList의 앞에 curList의 car값을 추가한다.
			newList = prependList(newList, curList->list.value);
		}
		if (newList == NULL)
			return create_error();
		// curList = (curList의 cdr)
		curList = curList->list.next;
	}

	return *newList;
}

// APPEND : 매개변수들의 전달받은 리스트들을 서로 연결하여 만든 새로운 리스트를 반환한다.
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
