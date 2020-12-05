#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include "run.h"

bool isRunningError = false;// running 과정에서 에러가 있는 경우 true. (main.c에 전달할 정보)
struct TreeNode* head;
struct Variable* v_head;	// 변수 저장

void run(const struct TreeNode* const h, const struct Variable* const v_h)
{
	head = h;
	v_head = v_h;
	
	switch(head->key.code)
	{
		case FUNC_TYPE1:
			print_l(func_type1());
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

/* error - 에러 처리 함수 */
static element* error(char* message){
    if(!isRunningError)
        printf("syntax error - %s\n", message);    
    isRunningError = true;
    return NULL;
}

element* func_type1()
{
	char* keywords[] = { "CAR", "CDR", "CADDR", "REVERSE", "LENGTH", "ATOM",
						"NULL", "NUMBERP", "ZEROP", "MINUSP", "STRINGP" };
	int keywords_len = sizeof(keywords) / sizeof(char*);	// keywords 배열 길이
	int func_index = find_func_index(keywords, keywords_len);

	// child1이 변수냐, 아니냐
	Variable* var = find_variable(head->key.lexeme[0]);	// 저장된 변수인지
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
		result->code = child1_code;
		strcpy(result->lexeme, head->child1->key.listElem[0]->lexeme);
		break;
	case CDR:	// 리스트의 첫번째 원소를 제외한 나머지를 결과로 생성
		result->code = child1_code;
		result = make_listElem_cdr(result, listElem_len);
		break;
	case CADDR:	// 리스트의 세번째 원소를 구함
		result->code = child1_code;
		strcpy(result->lexeme, head->child1->key.listElem[2]->lexeme);
		break;
	case REVERSE:	// 주어진 리스트 안의 원소의 순서를 거꾸로 바꿈
		result->code = child1_code;
		result = make_listElem_reverse(result, listElem_len);
		break;
	case LENGTH:	// 주어진 리스트 내의 원소 개수를 값으로 반환
		result->code = INT_LIT;
		sprintf(result->lexeme, "%d", listElem_len);
		break;
	case _ATOM:	// ATOM(심볼)일 때만 참(true)를 반환
		isPredicateFunc = true;
		if (child1_code == ATOM)
			isTrue = true;
		break;
	case _NULL:	// NIL일 때만 참(true)을 반환
		isPredicateFunc = true;
		if (child1_code == NIL)
			isTrue = true;
		break;
	case NUMBERP:	// 숫자일 때만 참(true)을 반환
		isPredicateFunc = true;
		if (child1_code == INT_LIT)
			isTrue = true;
		break;
	case ZEROP:	// 0일 때만 참(true)을 반환
		isPredicateFunc = true;
		if (child1_num == 0)
			isTrue = true;
		break;
	case MINUSP:	// 음수일 때만 참(true)을 반환
		isPredicateFunc = true;
		if (child1_num < 0)
			isTrue = true;
		break;
	case STRINGP:	// STRING일 때만 참(true)을 반환
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

	return result;
}

element* func_type2()
{
	char* keywords[] = { "DEFVAR", "SETQ", "NTH", "CONS", "MEMBER", "REMOVE", "EQUAL"};
	int keywords_len = sizeof(keywords) / sizeof(char*);	// keywords 배열 길이
	int func_index = -1;

	for (int i = 0; i < keywords_len; i++) {
		if (head->key.lexeme == keywords[i]) {
			func_index = i;
			break;
		}
	}

	
	switch (func_index + 200)
	{
	case DEFVAR:
		if(head->child1->key.code != IDENT)
			return error("1st argument should be IDENT");
		
		Variable *p = find_variable(head->child1->key.lexeme);
		if (p == NULL) {
			// 새로운 Variable 추가
			Variable* temp = (Variable*)malloc(sizeof(Variable));
			strcpy(temp->name, head->child1->key.lexeme);
			temp->value = head->child2->key;
			temp->next = head;
			head = temp;
		}
		else
			p->value = head->child2->key;	// 바인딩
		return &head->child1->key;
		break;
	
	case SETQ:
		if(head->child1->key.code != IDENT)
			return error("1st argument should be IDENT");
		
		Variable *p = find_variable(head->child1->key.lexeme);
		if (p == NULL)
			return error("undefined variable");
		p->value = head->child2->key;	// 바인딩
		return &head->child1->key;
		break;
	
	case NTH:
		if(head->child1->key.code != INT_LIT)
			return error("1st argument should be INT");
		
		if(head->child2->key.code != LIST_CODE)
			return error("2nd argument should be LIST");
		
		int index = atoi(head->child1->key.lexeme);
		if (head->child2->key.listElem[index] != NULL)
			return head->child2->key.listElem[index];
		else {	// 리스트 크기를 벗어나는 위치를 참조한 경우
			element temp = {NIL, "NIL"};
			return &temp;
		}
		break;

	case CONS:
		switch(head->child1->key.code){
			case INT_LIT: case ATOM: case STRING: case NIL: /*case T: */
				break;
			default:
				return error("1st argument has wrong type");
				break;
		}
		if(head->child2->key.code != LIST_CODE)
			return error("2nd argument should be LIST");

		element* result = malloc(sizeof(struct element));
		result->code = LIST_CODE;
		result->listElem[0] = &head->child1->key;
		for (int i = 0; head->child2->key.listElem[i] != NULL; i++)
			result->listElem[i+1] = head->child2->key.listElem[i];
		return result;
		break;

	case MEMBER:
		switch(head->child1->key.code){
			case INT_LIT: case ATOM: case STRING: case NIL: /*case T: */
				break;
			default:
				return error("1st argument has wrong type");
				break;
		}
		if(head->child2->key.code != LIST_CODE)
			return error("2nd argument should be LIST");

		int foundIndex = -1;
		element* result = malloc(sizeof(struct element));
		result->code = LIST_CODE;

		for (int i = 0; head->child2->key.listElem[i] != NULL; i++){
			if(!strcmp(head->child1->key.lexeme, head->child2->key.listElem[i]->lexeme)){
				foundIndex = i;
				break;
			}
		}
		if (foundIndex != -1){
			for (int i = 0; head->child2->key.listElem[i+foundIndex] != NULL; i++)
					result->listElem[i] = head->child2->key.listElem[i+foundIndex];
			return result;
		}
		else {
			element temp = {NIL, "NIL"};
			return &temp;
		}
		break;

	case REMOVE:
		switch(head->child1->key.code){
			case INT_LIT: case ATOM: case STRING: case NIL: /*case T: */
				break;
			default:
				return error("1st argument has wrong type");
				break;
		}
		if(head->child2->key.code != LIST_CODE)
			return error("2nd argument should be LIST");

		int count = 0;
		element* result = malloc(sizeof(struct element));
		result->code = LIST_CODE;

		for (int i = 0; head->child2->key.listElem[i] != NULL; i++){
			if(!strcmp(head->child1->key.lexeme, head->child2->key.listElem[i]->lexeme)){
				count++;
				continue;
			}
			result->listElem[i-count] = head->child2->key.listElem[i];
		}
		return result;
		break;

	case EQUAL:
		element* result = malloc(sizeof(struct element));
		//result->code = BOOLEAN;
		if(isEqual(&head->child1->key, &head->child2->key))
			strcpy(result->lexeme, "T");
		else	
			strcpy(result->lexeme, "NIL");
		return result;
		break;
	
	default:
		/* 사칙연산, 비교연산... */
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
Variable* find_variable(char* var)
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
		strcpy(result->listElem[i]->lexeme, head->child1->key.listElem[len - 1 - i]->lexeme);	// 역순으로
	}

	return result;
}

// EQUAL FUNC의 재귀 호출을 이용한 구현
bool isEqual(element* arg1, element* arg2) {
	if(arg1->code == LIST_CODE) {
		for (int i = 0; arg1->listElem[i] != NULL; i++){
			if(!isEqual(arg1->listElem[i], arg2->listElem[i]))
				return false;
		}
		return true;
	}
	else {
		if (!strcmp(arg1->lexeme, arg2->lexeme))
			return true;
	}


// 결과 출력하기
void print_l(element* result)
{
	// 리스트가 아닌 경우
	if (result->code != LIST_CODE)
		printf("%s ", result->lexeme);
	// 리스트인 경우
	else
	{
		int list_len = tree_listElem_length(result);

		if (list_len == 1)	// 원소의 개수가 1개인 리스트
			printf("%s ", result->lexeme);
		else
		{
			printf("(");

			int i = 0;
			for (i = 0; i < list_len - 1; i++)
				printf("%s ", result->listElem[i]->lexeme);

			printf("%s)", result->listElem[i]->lexeme);
		}
	}

	printf("\n");
}