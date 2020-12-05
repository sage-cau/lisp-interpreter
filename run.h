#ifndef RUN_H
#define RUN_H

#include "total.h"

// 변수 저장
typedef struct Variable {
	char* name;
	element value;
	struct Variable* next;
} Variable;

#define KEYWORD1 100
typedef enum
{
	CAR = 100,
	CDR,
	CADDR,
	REVERSE,
	LENGTH,
	_ATOM,
	_NULL,
	NUMBERP,
	ZEROP,
	MINUSP,
	STRINGP,
	PRINT,
} Keyword1;	// func_type1에 속하는 함수

#define KEYWORD2 200
typedef enum
{
	DEFVAR = 200,
	SETQ,
	NTH,
	CONS,
	MEMBER,
	REMOVE,
	EQUAL,
} Keyword2;	// func_type2에 속하는 함수

#define KEYWORD3 300
typedef enum
{
	SUBST = 300,
	IF,
} Keyword3;	// fund_type3에 속하는 함수

void run(const struct TreeNode* const, const struct Variable* const);

element* func_type1(const struct TreeNode* const);	// 매개변수가 1개인 함수
element* func_type2(const struct TreeNode* const);	// 매개변수가 2개인 함수
element* func_type3(const struct TreeNode* const);	// 매개변수가 3개인 함수
element* func_type4(const struct TreeNode* const);

element* comparison(const struct TreeNode* const);
element* numeric_operation(const struct TreeNode* const);

int find_func_index(char* [], int);
int var_listElem_length(const struct Variable* const);
int tree_listElem_length(const struct TreeNode* const);
Variable* find_variable(char*);
element* make_listElem_cdr(element*, int);
element* make_listElem_reverse(element*, int);

bool isEqual(element* arg1, element* arg2);
void print_l(element*);
static element* error(char* message);

#endif