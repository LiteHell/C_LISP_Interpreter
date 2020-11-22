#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
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
	if (evaluated.type == NIL)
		return makeNIL();
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
	if (evaluated.type == NIL)
		return makeNIL();
	// 평가값이 LIST 형식이 아니라면 오류 반환
	if (evaluated.type != LIST)
		return create_error();

	// 평가값의 나머지 요소를 반환한다.
	return evaluated.list.next == NULL ? makeNIL() : *evaluated.list.next;
}

// NTH : 두번째 매개변수의 리스트에서 (첫번째 매개변수)번째의 요소를 반환한다. 인덱스는 0부터 세는 것을 주의
// (NTH I X)는 X에 CDR을 I번 한 값의 CAR값이랑 동일하다는 아이디어로 구현했다.
obj_t fn_nth(obj_t* pObj)
{
	if (argument_count(pObj) != 2)
		return create_error();
	// 첫번째 매개변수를 평가한다.
	obj_t evaluatedIndex = evaluateObject(pObj->list.value);

	// 두번째 매개변수를 평가한다.
	obj_t evaluatedList = evaluateObject(pObj->list.next->list.value);
	// 첫번째 매개변수가 정수가 아니거나 두번째 매개변수가 리스트가 아니라면 오류를 반환한다.
	double countd = evaluatedIndex.number.value;
	if (evaluatedIndex.type != NUMBER || floor(countd) != countd || countd < 0 || (evaluatedList.type != LIST && evaluatedList.type != NIL))
		return create_error();
	if (evaluatedList.type == NIL)
		return makeNIL();

	// 첫번째 매개변수의 값을 가져온다.
	int count = countd;
	// 루프에 사용할 리스트, 두번째 매개변수를 대입한다.
	obj_t* list = &evaluatedList;
	for (int i = 0; i < count; i++) {
		list = list->list.next; // list = (list의 CDR)을 count만큼 반복하게 된다.
		if (list == NULL)
			return makeNIL();
	}
	// list의 CAR을 반환한다.
	return *list->list.value;
}

// CONS : 두번째 매개변수(리스트 형식)에 첫번째 매개변수를 첫번째 요소로 추가한 리스트를 반환한다.
obj_t fn_cons(obj_t* pObj)
{
	if (argument_count(pObj) != 2)
		return create_error();

	obj_t* itemPtr, * listPtr;
	// 첫번째 매개변수를 평가한다.
	obj_t evaluatedItem = evaluateObject(pObj->list.value);
	// 두번째 매개변수를 평가한다.
	obj_t evaluatedList = evaluateObject(pObj->list.next->list.value);
	// 두번째 매개변수가 리스트가 아니라면 오류 반환
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

// REVERSE : 주어진 리스트를 뒤집은 새로운 리스트를 반환한다.
obj_t fn_reverse(obj_t* pObj)
{
	if (argument_count(pObj) != 1)
		return create_error();
	// 첫번째 매개변수를 평가하고, 평가값이 리스트가 아니라면 에러
	obj_t evaluatedList = evaluateObject(pObj->list.value);
	if (evaluatedList.type == NIL)
		return makeNIL();
	if (evaluatedList.type != LIST)
		return create_error();

	// 뒤집은 후의 리스트
	obj_t* newList = NULL;

	// 뒤집기 전의 리스트(curList)내에서 반복한다.
	for (obj_t* curList = &evaluatedList; curList != NULL; curList = curList->list.next) {
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
	}
	return *newList;
}

// APPEND : 매개변수들의 전달받은 리스트들을 서로 연결하여 만든 새로운 리스트를 반환한다.
obj_t fn_append(obj_t* pObj)
{
	if (argument_count(pObj) == 1 && pObj->list.value->type != LIST) // 매개변수가 1개일 경우 리스트가 아닌 값 허용
		return *pObj->list.value;

	obj_t* newList = NULL, * newTail = NULL;
	// 매개변수로 전달받은 리스트들을 반복
	for (obj_t* listNow = pObj; listNow != NULL; listNow = listNow->list.next) {
		// 매개변수 평가
		obj_t innerList = evaluateObject(listNow->list.value);
		// 만약 평가값이 리스트가 아니라면 오류 반환
		if (innerList.type == NIL)
			continue;
		if (innerList.type != LIST)
		{
			return create_error();
		}

		// 리스트 내부를 반복
		for (obj_t* innerListNow = &innerList; innerListNow != NULL; innerListNow = innerListNow->list.next) {
			if (newList == NULL) {
				// 첫 루프라면 리스트 생성
				newList = makeListWithValue(innerListNow->list.value);
				// 메모리 할당실패시 오류 반환
				if (newList == NULL)
					return create_error();
				newTail = newList;
			}
			else {
				// 첫 루프가 아니라면 리스트의 뒤에 추가
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

	// 매개변수 평가
	obj_t evaluated = evaluateObject(pObj->list.value);
	switch (evaluated.type) {
	case NIL:
		return makeNumber(0);
	case LIST: {
		// 갯수를 저장할 변수
		int count = 0;

		// 갯수를 센다
		for (obj_t* now = &evaluated; now != NULL; now = now->list.next) {
			count++;
		}
		// 결과 반환
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

	// 첫번째 매개변수 평가, 리스트 내에서 찾아야할 값이다.
	obj_t target = evaluateObject(pObj->list.value);
	// 두번째 매개변수 평가, 특정 값을 찾을 리스트이다.
	obj_t evaluatedList = evaluateObject(pObj->list.next->list.value);
	// 두번째 매개변수가 리스트가 아니라면 오류 반환
	if (evaluatedList.type == NIL)
		return makeNIL();
	if (evaluatedList.type != LIST)
	{
		return create_error();
	}
	
	for (obj_t* now = &evaluatedList; now != NULL; now = now->list.next) {
		// 찾았다면 현재 리스트를 반환
		if (obj_equals(now->list.value, &target)) {
			return *now;
		}
	}
	// 못 찾았다면 nil 반환
	return makeNIL();
}

obj_t fn_assoc(obj_t* pObj)
{
	if (argument_count(pObj) != 2)
		return create_error();

	// 키로 사용할 첫번째 매개변수 평가
	obj_t key = evaluateObject(pObj->list.value);
	// DB로 사용할 두번째 매개변수 평가, 리스트가 아니라면 오류를 반환한다.
	obj_t evaluatedList = evaluateObject(pObj->list.next->list.value);
	if (evaluatedList.type == NIL)
		return makeNIL();
	if (evaluatedList.type != LIST)
	{
		return create_error();
	}

	for (obj_t* now = &evaluatedList; now != NULL; now = now->list.next) {
		// DB 요소
		obj_t* entry = now->list.value;
		if (entry->type == NIL)
			continue;
		if (entry->type != LIST)
			return create_error();
		// DB 요소의 키
		obj_t* entryKey = entry->list.value;
		
		// DB 요소의 키와 전달받은 키가 일치한다면 해당 요소를 반환한다.
		if (obj_equals(entryKey, &key)) {
			return *entry;
		}
	}

	// 찾지 못했다면 NIL을 반환한다.
	return makeNIL();
}

obj_t fn_remove(obj_t* pObj)
{
	if (argument_count(pObj) != 2)
		return create_error();

	// 매개변수 평가 및 유형 검증
	obj_t target = evaluateObject(pObj->list.value);
	obj_t evaluatedList = evaluateObject(pObj->list.next->list.value);
	if (evaluatedList.type == NIL)
		return makeNIL();
	if (evaluatedList.type != LIST)
	{
		return create_error();
	}

	// 기존 리스트를 수정하지 말아야 함
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

	// 매개변수 평가 및 유형 검증
	obj_t newValue = evaluateObject(pObj->list.value);
	obj_t oldValue = evaluateObject(pObj->list.next->list.value);
	obj_t evaluatedList = evaluateObject(pObj->list.next->list.next->list.value);
	if (evaluatedList.type == NIL)
		return makeNIL();
	if (evaluatedList.type != LIST)
	{
		return create_error();
	}

	// 기존 리스트를 수정하지 말아야 함
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
