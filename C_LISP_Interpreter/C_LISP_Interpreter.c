/*
드디어 완성했다 으악 documentation은 여기 쓸게요 flex + bison이 너무 복잡해서
타입 체계가 좀 복잡해보일 수도 있어요

obj_t: 단일 lisp 객체를 나타냅니다. 타입 정의는 parsedef.h에 있습니다.
아래의 값 중 포인터인 것은 전부 동적할당이고 아직 free 로직을 구현하지 않았기
때문에 메모리 누수가 납니다. 나중에 프로젝트가 좀 진전되면 추가할 예정입니다.

- type_t .type: 객체의 타입을 나타냅니다. NUMBER, SYMBOL, CODE, LIST, NIL 중
  하나입니다. obj_t가 union이기 때문에 이 값을 변경하면 같은 객체의 다른 .type도
  전부 변경됩니다.
  .type의 값에 따라 아래 3종류의 union 타입 중 하나만 접근해야 합니다. 단, NIL일
  경우 모든 타입이 쓰레기 값을 가지며 사용해서는 안 됩니다. NIL은 빈 리스트와
  문법적으로 같은 의미를 가집니다.

- .number (.type = NUMBER)
  - .type
  - char * .literal: 입력한 원본 문자열을 나타냅니다. 혹시 몰라서 넣었고 이 값을
	쓸 일이 없다고 확신할 수 있을 때 지울 예정입니다.
  - double .value: 입력한 수의 실제 값을 나타냅니다.

- .symbol (.type = SYMBOL)
  - .type
  - char * .symbol: 입력한 심볼 이름을 나타냅니다.

- .list (.type = CODE or LIST)
  - .type
  - obj_t * .value: (CAR this)의 값입니다. NIL일 경우에도 non-null 포인터를
	값으로 가집니다.
  - obj_t * .next: (CDR this)의 값입니다. NIL일 경우에는 null 포인터를 값으로
	가집니다.

- .any도 있긴 한데 쓰지 말아주세요

int yyparse(obj_t *const result): lisp 객체 하나를 파싱하는 함수입니다. 여러
객체를 파싱하려면 여러 번 돌리면 됩니다. 파싱에 성공했다면 0을 반환하고 result가
가리키는 변수에 그 결과를 복사하며, 실패했다면 1을 반환합니다.

void lisp_debug(obj_t const *const result, int indent): 디버그 함수입니다. lisp
객체 하나를 포인터로 넣으면 그 객체에 대한 정보를 출력합니다. indent는 들여쓰기
깊이이며, 별 일이 없을 경우 0을 쓰면 됩니다.

lisp.yy.* 파일은 lisp.l 파일에 flex를, lisp.tab.* 파일은 lisp.y 파일에 bison을
돌려서 생성했습니다. Visual Studio에 무작정 파일을 넣어봤는데 좀 수정하고 나니까
실행이 잘 되네요!

혹시 lisp.l이나 lisp.y를 고쳐야 될 경우 잇창명에게 연락해 주세요. 우분투에서
빌드해서 푸시해드립니다.
*/

#include "parsedef.h"
#include "lisp.yy.h"
#include "lisp.tab.h"
#include "lisp_processor.h"

void lisp_debug(obj_t const* const result, int indent) {
	for (int i = 0; i < 4 * indent; i++)
		printf(" ");
	switch (result->type) {
	case NUMBER:
		printf("\"%s\" %f", result->number.literal, result->number.value);
		break;
	case SYMBOL:
		printf("\"%s\"", result->symbol.symbol);
		break;
	case CODE:
		printf("(\n");
		{
			obj_t const* ptr = result;
			while (ptr) {
				lisp_debug(ptr->list.value, indent + 1);
				ptr = ptr->list.next;
				printf("\n");
			}
		}
		for (int i = 0; i < 4 * indent; i++)
			printf(" ");
		printf(")");
		break;
	case LIST:
		printf("'(\n");
		{
			obj_t const* ptr = result;
			while (ptr) {
				lisp_debug(ptr->list.value, indent + 1);
				ptr = ptr->list.next;
				printf("\n");
			}
		}
		for (int i = 0; i < 4 * indent; i++)
			printf(" ");
		printf(")");
		break;
	case NIL:
		printf("NIL");
		break;
	}
}

int main() {
#define PARSER_DEBUG
#ifdef PARSER_DEBUG
	// Debug-purpose code

	printf("PARSER_DEBUG preprocessor constant is set, meaning no processer is running and only parser output is printed\n");
	obj_t result;
	while (1) {
		if (!yyparse(&result)) {
			lisp_debug(&result, 0);
			printf("\n");
		}
	}
#else
	obj_t result;
	while (1) {
		if (!yyparse(&result)) {
			result = evaluateObject(&result);
			lisp_debug(&result, 0);
			printf("\n");
		}
	}
	return 0;
#endif
}