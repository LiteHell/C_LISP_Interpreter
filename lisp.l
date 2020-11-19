%{
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parsedef.h"
#include "lisp.tab.h"
int yywrap(void) { return 1; }
%}

space [ \n\t\r]
tokenchar [!#-&*-~]

%%
{tokenchar}+ {
		// token (number or symbol)
		// number is {digit}+(\.{digit}*)? or \.{digit}+

		double val = 0, sub = 1;
		int number = 0, period = 0;
		char *const lit = malloc((yyleng + 1)*sizeof(char));
		for(int i = 0; yytext[i]; i++)
			lit[i] = toupper(yytext[i]);
		lit[yyleng] = 0;

		if(!strcmp(lit, "NIL")) {
			free(lit);
			yylval.type = NIL;
			return LEX_NIL;
		}

		for(int i = 0; yytext[i]; i++) {
			if(yytext[i] == '.') {
				if(period)
					goto L_SYMBOL;
				period = 1;
			} else if(yytext[i] >= '0' && yytext[i] <= '9') {
				int digit = yytext[i] - '0';
				number = 1;
				if(period) {
					sub /= 10;
					val += sub*digit;
				} else
					val = 10*val + digit;
			} else
				goto L_SYMBOL;
		}

		if(number)
			goto L_NUMBER;
		
	L_SYMBOL:
		yylval.type = SYMBOL;
		yylval.symbol.symbol = lit;
		return LEX_SYMBOL;
	
	L_NUMBER:
		yylval.type = NUMBER;
		yylval.number.literal = lit;
		yylval.number.value = val;
		return LEX_NUMBER;
	}
\"(\\.|[^"\n\\])*\" {
		// string literal
		// supported escapes are \" \\ \a \b \f \n \r \t \v

		char *const lit = calloc(yyleng - 1, sizeof(char));
		for(int i = 1, j = 0; yytext[i] != '"'; i++) {
			if(yytext[i] == '\\')
				switch(yytext[++i]) {
					case 'a':
						lit[j++] = '\a';
					continue;
					case 'b':
						lit[j++] = '\b';
					continue;
					case 'f':
						lit[j++] = '\f';
					continue;
					case 'n':
						lit[j++] = '\n';
					continue;
					case 'r':
						lit[j++] = '\r';
					continue;
					case 't':
						lit[j++] = '\t';
					continue;
					case 'v':
						lit[j++] = '\v';
					continue;
				}
			lit[j++] = yytext[i];
		}

		yylval.type = STRING;
		yylval.string.string = realloc(lit, strlen(lit) + 1);
		return LEX_STRING;
	}
"'" { return LEX_SQUOTE; }
"(" { return LEX_LPAREN; }
")" { return LEX_RPAREN; }
{space} {}
. { return LEX_UNKNOWN; }
%%