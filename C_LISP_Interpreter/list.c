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
	obj_t* newList = NULL;
	// 매개변수로 전달받은 리스트들을 반복
	for (obj_t* listNow = pObj; listNow != NULL; listNow = listNow->list.next) {
		// 매개변수 평가
		obj_t innerList = evaluateObject(listNow->list.value);
		// 만약 평가값이 리스트가 아니라면 오류 반환
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
			}
			else {
				// 첫 루프가 아니라면 리스트의 뒤에 추가
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
	// 매개변수 평가
	obj_t evaluatedList = evaluateObject(pObj->list.value);
	// 매개변수가 리스트 형식이 아니라면 오류 반환
	if (evaluatedList.type != LIST)
	{
		return create_error();
	}
	// 갯수를 저장할 변수
	int count = 0;
	
	// 갯수를 센다
	for (obj_t* now = &evaluatedList; now != NULL; now = now->list.next) {
		count++;
	}
	// 결과 반환
	return makeNumber(count);
}

obj_t fn_member(obj_t* pObj)
{
	// 첫번째 매개변수 평가, 리스트 내에서 찾아야할 값이다.
	obj_t target = evaluateObject(pObj->list.value);
	// 두번째 매개변수 평가, 특정 값을 찾을 리스트이다.
	obj_t evaluatedList = evaluateObject(pObj->list.next->list.value);
	// 두번째 매개변수가 리스트가 아니라면 오류 반환
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
	// 키로 사용할 첫번째 매개변수 평가
	obj_t key = evaluateObject(pObj->list.value);
	// DB로 사용할 두번째 매개변수 평가, 리스트가 아니라면 오류를 반환한다.
	obj_t evaluatedList = evaluateObject(pObj->list.next->list.value);
	if (evaluatedList.type != LIST)
	{
		return create_error();
	}

	
	for (obj_t* now = &evaluatedList; now != NULL; now = now->list.next) {
		// DB 요소
		obj_t* entry = now->list.value;
		// DB 요소의 키
		obj_t* entryKey = entry->list.value;
		// DB 요소의 값
		obj_t* entryValue = entry->list.next->list.value;
		
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
	// 매개변수 평가 및 유형 검증
	obj_t target = evaluateObject(pObj->list.value);
	obj_t evaluatedList = evaluateObject(pObj->list.next->list.value);
	if (evaluatedList.type != LIST)
	{
		return create_error();
	}

	obj_t* head = &evaluatedList, * now = head, * prev = NULL;
	while (now != NULL && head != NULL) {
		if (obj_equals(now->list.value, &target)) {
			// 일치한다면 제거한다.
			if (prev == NULL) { // 첫 요소를 삭제하는 경우라면, head를 cdr로 대입하는 것만으로 삭제의 효과가 나타난다.
				head = now->list.next;
			}
			else { // 첫 요소가 아니라면, 링크드리스트에서 삭제하는 방식과 같이 접근한다.
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
	// 매개변수 평가 및 검증
	obj_t newValue = evaluateObject(pObj->list.value);
	obj_t oldValue = evaluateObject(pObj->list.next->list.value);
	obj_t evaluatedList = evaluateObject(pObj->list.next->list.next->list.value);
	if (evaluatedList.type != LIST)
	{
		return create_error();
	}

	
	for (obj_t* now = &evaluatedList; now != NULL; now = now->list.next) {
		if (obj_equals(now->list.value, &oldValue)) {
			// 일치한다면 value를 바꾼다.
			*now->list.value = newValue;
		}
	}
	return evaluatedList;

}
