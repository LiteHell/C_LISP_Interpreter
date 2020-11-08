%{
#include <stdio.h>
#include <stdlib.h>
#include "parsedef.h"
#include "lisp.yy.h"

void yyerror(obj_t *const r, char const *s) {
	fprintf(stderr, "Syntax error!\n");
}
%}

%start root

%parse-param {obj_t *const result}

%define api.value.type {obj_t}

%token <number> LEX_NUMBER
%token <symbol> LEX_SYMBOL
%token <list> LEX_NIL
%token LEX_SQUOTE
%token LEX_LPAREN
%token LEX_RPAREN
%token LEX_UNKNOWN

%type <any> expr
%type <list> list

%%

root:
	expr {
			*result = *(obj_t *)&$1;
			YYACCEPT;
		}
	;

expr:
	LEX_NUMBER { $$.number = $1; }
	| LEX_SQUOTE LEX_NUMBER { $$.number = $2; }
	| LEX_SYMBOL { $$.symbol = $1; }
	| LEX_SQUOTE LEX_SYMBOL { $$.symbol = $2; }
	| LEX_NIL {
			$$.type = NIL;
		}
	| LEX_LPAREN list LEX_RPAREN {
			$$.list = $2;
			if($2.type != NIL)
				$$.type = CODE;
		}
	| LEX_SQUOTE LEX_LPAREN list LEX_RPAREN {
			$$.list = $3;
			if($3.type != NIL)
				$$.type = LIST;
		}
	;

list:
	%empty {
			$$.type = NIL;
		}
	| expr list {
			$$.type = LIST;
			$$.value = malloc(sizeof(obj_t));
			*$$.value = *(obj_t *)&$1;
			if($2.type != NIL) {
				$$.next = malloc(sizeof(obj_t));
				*$$.next = *(obj_t *)&$2;
			} else
				$$.next = NULL;
		}
	;

%%