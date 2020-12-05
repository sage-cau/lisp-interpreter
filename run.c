#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "run.h"

struct TreeNode* head;
struct Variable* v_head;	// ���� ����

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

element* func_type1()
{
	char* keywords[] = { "CAR", "CDR", "CADDR", "REVERSE", "LENGTH", "ATOM",
						"NULL", "NUMBERP", "ZEROP", "MINUSP", "STRINGP" };
	int keywords_len = sizeof(keywords) / sizeof(char*);	// keywords �迭 ����
	int func_index = find_func_index(keywords, keywords_len);

	// child1�� ������, �ƴϳ�
	Variable* var = find_varialbe(head->key.lexeme[0]);	// ����� ��������
	int child1_code;
	int child1_num;
	int listElem_len;

	// ������
	if (var)
	{
		child1_code = var->value.code;
		if (child1_code == INT_LIT)
			child1_num = atoi(var->value.lexeme);
		listElem_len = var_listElem_length(var);
	}
	// ������ �ƴϸ�
	else
	{
		child1_code = head->child1->key.code;
		if (child1_code == INT_LIT)
			child1_num = atoi(head->child1->key.lexeme);
		listElem_len = tree_listElem_length(head->child1);
	}

	element* result = malloc(sizeof(struct element));
	bool isPredicateFunc = false;	// Predicate �Լ�����
	bool isTrue = false;	// Predicate �Լ����� ���� true����

	switch (func_index + 100)
	{
	case CAR:	// ����Ʈ�� ù��° ���� ������
		result->code = child1_code;
		strcpy(result->lexeme, head->child1->key.listElem[0]->lexeme);
		break;
	case CDR:	// ����Ʈ�� ù��° ���Ҹ� ������ �������� ����� ����
		result->code = child1_code;
		result = make_listElem_cdr(result, listElem_len);
		break;
	case CADDR:	// ����Ʈ�� ����° ���Ҹ� ����
		result->code = child1_code;
		strcpy(result->lexeme, head->child1->key.listElem[2]->lexeme);
		break;
	case REVERSE:	// �־��� ����Ʈ ���� ������ ������ �Ųٷ� �ٲ�
		result->code = child1_code;
		result = make_listElem_reverse(result, listElem_len);
		break;
	case LENGTH:	// �־��� ����Ʈ ���� ���� ������ ������ ��ȯ
		result->code = INT_LIT;
		sprintf(result->lexeme, "%d", listElem_len);
		break;
	case _ATOM:	// ATOM(�ɺ�)�� ���� ��(true)�� ��ȯ
		isPredicateFunc = true;
		if (child1_code == ATOM)
			isTrue = true;
		break;
	case _NULL:	// NIL�� ���� ��(true)�� ��ȯ
		isPredicateFunc = true;
		if (child1_code == NIL)
			isTrue = true;
		break;
	case NUMBERP:	// ������ ���� ��(true)�� ��ȯ
		isPredicateFunc = true;
		if (child1_code == INT_LIT)
			isTrue = true;
		break;
	case ZEROP:	// 0�� ���� ��(true)�� ��ȯ
		isPredicateFunc = true;
		if (child1_num == 0)
			isTrue = true;
		break;
	case MINUSP:	// ������ ���� ��(true)�� ��ȯ
		isPredicateFunc = true;
		if (child1_num < 0)
			isTrue = true;
		break;
	case STRINGP:	// STRING�� ���� ��(true)�� ��ȯ
		isPredicateFunc = true;
		if (child1_code == STRING)
			isTrue = true;
		break;
	default:
		break;
	}

	// Predicate �Լ��� ���
	if (isPredicateFunc) {
		//result->code = BOOLEAN;	// BOOLEAN �ڵ� �߰��ؾ� ��
		isTrue ? strcpy(result->lexeme, "T") : strcpy(result->lexeme, "NIL");
	}

	return result;
}

element func_type2()
{
	char* keywords[] = { "SETQ", "NTH", "CONS", "MEMBER", "REMOVE", "EQUAL" };
	int keywords_len = sizeof(keywords) / sizeof(char*);	// keywords �迭 ����
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
		/* ��Ģ����, �񱳿���... */
		break;
	}
}

// � �Լ����� ã��
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

// ����Ʈ ���� ���ϱ�
// �Ķ���Ͱ� ������ ���
int var_listElem_length(const struct Variable* const var)
{
	int len = 0;

	for (int i = 0; i < 100; i++)	// listElem �迭 ũ��: 100
		if (var->value.listElem[i] == 0xcdcdcdcd) {
			len = i;
			break;
		}

	return len;
}

// ����Ʈ ���� ���ϱ�
// �Ķ���Ͱ� ������ �ƴ� ���
int tree_listElem_length(const struct TreeNode* const child)
{
	int len = 0;

	for (int i = 0; i < 100; i++)	// listElem �迭 ũ��: 100
		if (child->key.listElem[i] == 0xcdcdcdcd) {
			len = i;
			break;
		}

	return len;
}

// ���ϴ� ���� ã��
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
		strcpy(result->listElem[i]->lexeme, head->child1->key.listElem[i + 1]->lexeme);	// ù��° ���� ������ ������
	}

	return result;
}

// REVERSE FUNC
element* make_listElem_reverse(element* result, int len)
{
	for (int i = 0; i < len; i++) {
		result->listElem[i] = malloc(sizeof(struct element));
		result->listElem[i]->code = head->child1->key.listElem[len - 1 - i]->code;
		strcpy(result->listElem[i]->lexeme, head->child1->key.listElem[len - 1 - i]->lexeme);	// ��������
	}

	return result;
}

// ��� ����ϱ�
void print_l(element* result)
{
	// ����Ʈ�� �ƴ� ���
	if (result->code != LIST_CODE)
		printf("%s ", result->lexeme);
	// ����Ʈ�� ���
	else
	{
		int list_len = tree_listElem_length(result);

		if (list_len == 1)	// ������ ������ 1���� ����Ʈ
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