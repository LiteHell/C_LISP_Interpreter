%{
#include <stdio.h>
#include <stdlib.h>
#include "parsedef.h"
#include "lisp.yy.h"

void yyerror(obj_t *const r, char const *s) {
	fprintf(stderr, "Syntax error!\n");
}

void listify(obj_t *const o, int force) {
	if(!o)
		return;

	switch(o->type) {
		case SYMBOL:
			if(force)
				o->type = LITSYMBOL;
		break;
		case _EXPLICIT_LITERAL_LIST:
			force = 1;
		case CODE: case LIST:
			if(force)
				o->type = LIST;
			listify(o->list.next, force);
			listify(o->list.value, force);
		break;
	}
}
%}

%start root

%parse-param {obj_t *const result}

%define api.value.type {obj_t}

%token <number> LEX_NUMBER
%token <symbol> LEX_SYMBOL
%token <string> LEX_STRING
%token <list> LEX_NIL
%token LEX_SQUOTE
%token LEX_LPAREN
%token LEX_RPAREN
%token LEX_UNKNOWN

%type <any> expr
%type <any> pure_expr
%type <list> list

%%

root:
	expr {
			*result = *(obj_t *)&$1;
			listify(result, 0);
			YYACCEPT;
		}
	;

expr:
	pure_expr {
			$$ = $1;
		}
	| LEX_SQUOTE expr {
			$$ = $2;
			switch($$.type) {
				case SYMBOL:
					$$.type = LITSYMBOL;
				break;
				case CODE: case LIST:
					$$.type = _EXPLICIT_LITERAL_LIST;
				break;
			}
		}
	;

pure_expr:
	LEX_NUMBER { $$.number = $1; }
	| LEX_SYMBOL {
			$$.symbol = $1;
			$$.type = SYMBOL;
		}
	| LEX_STRING { $$.string = $1; }
	| LEX_LPAREN list LEX_RPAREN {
			$$.list = $2;
			if($2.type != NIL)
				$$.type = CODE;
		}
	| LEX_NIL {
			$$.type = NIL;
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