#ifndef _H_LISP_ALL_FUNCS
#define _H_LISP_ALL_FUNCS
#include "lisp_processor_functions_arithmetic.h"
#include "lisp_processor_functions_symbol.h"
#include "lisp_processor_functions_list.h"
#define LISP_FUNC_NAMES LISP_ARITHMETIC_FUNC_NAMES, LISP_SYMBOL_FUNC_NAMES, LISP_LIST_FUNC_NAMES
#define LISP_FUNC_COUNT LISP_ARITHMETIC_FUNC_COUNT + LISP_SYMBOL_FUNC_COUNT + LISP_LIST_FUNC_COUNT
#define LISP_FUNCS LISP_ARITHMETIC_FUNCS, LISP_SYMBOL_FUNCS, LISP_LIST_FUNCS
#endif