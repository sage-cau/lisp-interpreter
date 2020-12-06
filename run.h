#ifndef RUN_H
#define RUN_H

#include "total.h"

#define KEYWORD1 100
#define KEYWORD2 200
#define KEYWORD3 300
#define KEYWORD4 400

typedef enum
{
	CAR = KEYWORD1,
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
} Keyword1;	// func_type1�� ���ϴ� �Լ�

typedef enum
{
	DEFVAR = KEYWORD2,
	SETQ,
	NTH,
	CONS,
	MEMBER,
	REMOVE,
	EQUAL,
} Keyword2;	// func_type2�� ���ϴ� �Լ�

typedef enum
{
	COND = KEYWORD3,
	SUBST,
	IF,
} Keyword3;	// fund_type3�� ���ϴ� �Լ�

typedef enum
{
	LIST = KEYWORD4,
	APPEND,
} Keyword4;	// func_type4�� ���ϴ� �Լ�

Variable* run(const struct TreeNode* const, const struct Variable* const);

element* func_type1(const struct TreeNode* const);	// �Ű������� 1���� �Լ�
element* func_type2(const struct TreeNode* const);	// �Ű������� 2���� �Լ�
element* comparison(const struct TreeNode* const);	// �񱳿���(>, <, ...)
element* numeric_operation(const struct TreeNode* const);	// ��Ģ����
element* func_type3(const struct TreeNode* const);	// �Ű������� 3���� �Լ�
element* func_type4(const struct TreeNode* const);	// �Ű������� ������ �������� �Լ�

int find_func_index(const struct TreeNode* const, char* [], int);
int var_listElem_length(const struct Variable* const);
int tree_listElem_length(const struct TreeNode* const);
Variable* find_variable(char*);
element* make_listElem_cdr(const struct TreeNode* const, element*, int);
element* make_listElem_reverse(const struct TreeNode* const, element*, int);

bool isEqual(element* arg1, element* arg2);
void print_l(element*);
static element* error(char* message);
void preorderIdentSearch(TreeNode* root);

#endif