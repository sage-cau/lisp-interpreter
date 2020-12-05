#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "run.h"

struct TreeNode* head;
struct Variable* v_head;	// 변수 저장

void run(const struct TreeNode* const h, const struct Variable* const v_h)
{
	head = h;
	v_head = v_h;
	
	switch(head->key.code)
	{
		case FUNC_TYPE1:
			func_type1();
			break;
		case FUNC_TYPE2:
			func_type2();
			break;
		case FUNC_TYPE3:
			break;
		case FUNC_TYPE4:
			break;
	}
}

// 어떤 함수인지 찾기
int find_func_index(char* keywords[], int len)
{
    int func_index = -1;

	for (int i = 0; i < len; i++) {
		if (!strcmp(head->key.lexeme, keywords[i])) {
			func_index = i;
			break;
		}
	}

	return func_index;
}

element* func_type1()
{
	char* keywords[] = { "CAR", "CDR", "CADDR", "REVERSE", "LENGTH", "ATOM", 
						"NULL", "NUMBERP", "ZEROP", "MINUSP", "STRINGP" };
	int keywords_len = sizeof(keywords) / sizeof(char*);	// keywords 배열 길이
	int func_index = find_func_index(keywords, keywords_len);

	// child1이 변수냐, 아니냐
	Variable* var = find_varialbe(head->key.lexeme[0]);	// 저장된 변수인지
	int child1_code;
	int child1_num;
	int listElem_len;

	// 변수면
	if (var)
	{
		child1_code = var->value.code;
		if (child1_code == INT_LIT)
			child1_num = atoi(var->value.lexeme);
		listElem_len = var_listElem_length(var);
	}
	// 변수가 아니면
	else
	{
		child1_code = head->child1->key.code;
		if (child1_code == INT_LIT)
			child1_num = atoi(head->child1->key.lexeme);
		listElem_len = tree_listElem_length(head->child1);
	}

	element* result = malloc(sizeof(struct element));
	bool isPredicateFunc = false;	// Predicate 함수인지
	bool isTrue = false;	// Predicate 함수에서 값이 true인지
	
	switch (func_index + 100)
	{
	case CAR:	// 리스트의 첫번째 원소 가져옴
		result->code = LIST_CODE;	// ??????????
		strcpy(result->lexeme, head->child1->key.listElem[0]->lexeme);	// 첫번째 원소
		break;
	case CDR:
		result->code = LIST_CODE;	// ??????????
		result = make_listElem_cdr(result, listElem_len);
		break;
	case CADDR:
		result->code = LIST_CODE;	// ??????????
		strcpy(result->lexeme, head->child1->key.listElem[2]->lexeme);	// 세번째 원소
		break;
	case REVERSE:
		result->code = LIST_CODE;	// ??????????
		result = make_listElem_reverse(result, listElem_len);
		break;
	case LENGTH:
		result->code = INT_LIT;
		sprintf(result->lexeme, "%d", listElem_len);
		break;
	case _ATOM:
		isPredicateFunc = true;
		if (child1_code == ATOM)
			isTrue = true;
		break;
	case _NULL:
		isPredicateFunc = true;
		if (child1_code == NIL)
			isTrue = true;
		break;
	case NUMBERP:
		isPredicateFunc = true;
		if (child1_code == INT_LIT)
			isTrue = true;
		break;
	case ZEROP:
		isPredicateFunc = true;
		if (child1_num == 0)
			isTrue = true;
		break;
	case MINUSP:
		isPredicateFunc = true;
		if (child1_num < 0)
			isTrue = true;
		break;
	case STRINGP:
		isPredicateFunc = true;
		if (child1_code == STRING)
			isTrue = true;
		break;
	default:
		break;
	}

	// Predicate 함수의 경우
	if (isPredicateFunc) {
		//result->code = BOOLEAN;	// BOOLEAN 코드 추가해야 함
		isTrue ? strcpy(result->lexeme, "T") : strcpy(result->lexeme, "NIL");
	}
	
	/* 테스트용 */
	//printf("%s\n", result->lexeme);
	/*for (int i = 0; i < listElem_len - 1; i++)
		printf("%s\n", result->listElem[i]->lexeme);*/

	return result;
}

// 리스트 개수 구하기
// 파라미터가 변수인 경우
int var_listElem_length(const struct Variable* const var)
{
	int len = 0;

	for (int i = 0; i < 100; i++)	// listElem 배열 크기: 100
		if (var->value.listElem[i] == 0xcdcdcdcd) {
			len = i;
			break;
		}

	return len;
}

// 리스트 개수 구하기
// 파라미터가 변수가 아닌 경우
int tree_listElem_length(const struct TreeNode* const child)
{
	int len = 0;

	for (int i = 0; i < 100; i++)	// listElem 배열 크기: 100
		if (child->key.listElem[i] == 0xcdcdcdcd) {
			len = i;
			break;
		}

	return len;
}

// 원하는 변수 찾기
Variable* find_varialbe(char* var)
{
	if (v_head == NULL)
		return NULL;

	Variable* curr = v_head->next;
	while (curr != NULL)
	{
		if (curr->name == var)
			return curr;

		curr = curr->next;
	}

	return NULL;
}

// CDR FUNC
element* make_listElem_cdr(element* result, int len)
{
	for (int i = 0; i < len - 1; i++) {
		result->listElem[i] = malloc(sizeof(struct element));
		result->listElem[i]->code = head->child1->key.listElem[i + 1]->code;
		strcpy(result->listElem[i]->lexeme, head->child1->key.listElem[i + 1]->lexeme);	// 첫번째 원소 제외한 나머지
	}

	return result;
}

// REVERSE FUNC
element* make_listElem_reverse(element* result, int len)
{
	for (int i = 0; i < len; i++) {
		result->listElem[i] = malloc(sizeof(struct element));
		result->listElem[i]->code = head->child1->key.listElem[len - 1 - i]->code;
		strcpy(result->listElem[i]->lexeme, head->child1->key.listElem[len - 1 - i]->lexeme);	// 첫번째 원소 제외한 나머지
	}

	return result;
}

element func_type2()
{
	char* keywords[] = { "SETQ", "NTH", "CONS", "MEMBER", "REMOVE", "EQUAL"};
	int keywords_len = sizeof(keywords) / sizeof(char*);	// keywords 배열 길이
	int func_index = find_func_index(keywords, keywords_len);

	switch (func_index + 200)
	{
	case SETQ:
		break;
	case NTH:
		break;
	case CONS:
		break;
	case MEMBER:
		break;
	case REMOVE:
		break;
	case EQUAL:
		break;
	default:
		/* 사칙연산, 비교연산... */
		break;
	}
}
