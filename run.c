#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include "run.h"

bool isRunningError = false;// running �������� ������ �ִ� ��� true. (main.c�� ������ ����)
struct Variable* v_head = NULL;	// ���� ����

void run(const struct TreeNode* const head/*, const struct Variable* const v_h*/)
{
	//v_head = v_h;
	
	if(strcmp(head->key.lexeme, "DEFVAR") && strcmp(head->key.lexeme, "SETQ") && head->key.code != IDENT)
		preorderIdentSearch(head);	// AST�� ������ȸ�ϸ� ident����, variable ����Ʈ ���� value�� �ٲ��ش�.

	switch(head->key.code)
	{
		case FUNC_TYPE1:
			print_l(func_type1(head));
			break;
		case FUNC_TYPE2:
			func_type2(head);
			break;
		case LESS_COMP: case GREATER_COMP: case EQUAL_COMP: 
		case LESS_EQUAL_COMP: case GREATER_EQUAL_COMP:
			comparison(head);
			break;
		case ADD_OP: case SUB_OP: case MULT_OP: case DIV_OP:
			numeric_operation(head);
			break;
		case FUNC_TYPE3:
			func_type3(head);
			break;
		case FUNC_TYPE4:
			func_type4(head);
			break;
		case IDENT:
			print_l(find_variable(head->key.lexeme)->value.lexeme);
		default:
			print_l(head->key.lexeme);
			break;
	}
}

// �Ű������� 1���� �Լ�
element* func_type1(const struct TreeNode* const head)
{
	char* keywords[] = { "CAR", "CDR", "CADDR", "REVERSE", "LENGTH", "ATOM",
						"NULL", "NUMBERP", "ZEROP", "MINUSP", "STRINGP", "PRINT" };
	int keywords_len = sizeof(keywords) / sizeof(char*);	// keywords �迭 ����
	int func_index = find_func_index(head, keywords, keywords_len);

	// child1�� ������, �ƴϳ�
	Variable* var = find_variable(head->key.lexeme[0]);	// ����� ��������
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

	switch (func_index + KEYWORD1)
	{
	case CAR:	// ����Ʈ�� ù��° ���� ������
		result->code = child1_code;
		strcpy(result->lexeme, head->child1->key.listElem[0]->lexeme);
		break;
	case CDR:	// ����Ʈ�� ù��° ���Ҹ� ������ �������� ����� ����
		result->code = child1_code;
		result = make_listElem_cdr(head, result, listElem_len);
		break;
	case CADDR:	// ����Ʈ�� ����° ���Ҹ� ����
		result->code = child1_code;
		strcpy(result->lexeme, head->child1->key.listElem[2]->lexeme);
		break;
	case REVERSE:	// �־��� ����Ʈ ���� ������ ������ �Ųٷ� �ٲ�
		result->code = child1_code;
		result = make_listElem_reverse(head, result, listElem_len);
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
	case PRINT:
		result->code = child1_code;
		strcpy(result->lexeme, head->child1->key.lexeme);
		break;
	default:
		break;
	}

	// Predicate �Լ��� ���
	if (isPredicateFunc) {
		if (isTrue) {
			result->code = T;
			strcpy(result->lexeme, "T");
		}
		else {
			result->code = NIL;
			strcpy(result->lexeme, "NIL");
		}
	}

	return result;
}

// �Ű������� 2���� �Լ�
element* func_type2(const struct TreeNode* head)
{
	char* keywords[] = { "DEFVAR", "SETQ", "NTH", "CONS", "MEMBER", "REMOVE", "EQUAL"};
	int keywords_len = sizeof(keywords) / sizeof(char*);	// keywords �迭 ����
	int func_index = find_func_index(head, keywords, keywords_len);
	
	Variable *p;
	element* result = malloc(sizeof(struct element));
	bool isTrue = false;

	switch (func_index + KEYWORD2)
	{
	case DEFVAR:
		if(head->child1->key.code != IDENT)
			return error("1st argument should be IDENT");
		
		p = find_variable(head->child1->key.lexeme);
		if (p == NULL) {
			// ���ο� Variable �߰�
			Variable* temp = (Variable*)malloc(sizeof(Variable));
			strcpy(temp->name, head->child1->key.lexeme);
			temp->value = head->child2->key;
			temp->next = head;
			head = temp;
		}
		else
			p->value = head->child2->key;	// ���ε�
		return &head->child1->key;
		break;
	
	case SETQ:
		if(head->child1->key.code != IDENT)
			return error("1st argument should be IDENT");
		
		p = find_variable(head->child1->key.lexeme);
		if (p == NULL)
			return error("undefined variable");
		p->value = head->child2->key;	// ���ε�
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
		else {	// ����Ʈ ũ�⸦ ����� ��ġ�� ������ ���
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
		isTrue = isEqual(&head->child1->key, &head->child2->key);
		result->code = isTrue? T : NIL;
		isTrue? strcpy(result->lexeme, "T") : strcpy(result->lexeme, "NIL");
		return result;
		break;
	
	default:
		break;
	}
}

element* comparison(const struct TreeNode* const head) {
	if(head->child1->key.code == INT_LIT || head->child1->key.code == FLOAT_LIT) {
		return error("wrong comparison operand type");
	}

	double num1 = atof(head->child1->key.lexeme);
	double num2 = atof(head->child2->key.lexeme);
	bool isTrue = false;
	element* result = malloc(sizeof(struct element));

	switch(head->key.code) {
		case LESS_COMP:
			isTrue = (num1 < num2);
			break;
		case GREATER_COMP:
			isTrue = (num1 > num2);
			break;
		case EQUAL_COMP:
			isTrue = (num1 == num2);
			break;
		case LESS_EQUAL_COMP:
			isTrue = (num1 <= num2);
			break;
		case GREATER_EQUAL_COMP:
			isTrue = (num1 >= num2);
			break;
	}
	result->code = isTrue? T : NIL;
	isTrue? strcpy(result->lexeme, "T") : strcpy(result->lexeme, "NIL");
	return result;
}

element* numeric_operation(const struct TreeNode* const head) {
	element* result = malloc(sizeof(struct element));
	double answer = 0;
	bool isFloat = false;

	TreeNode* temp = head->child1;
	while (temp != NULL) {
		if(temp->key.code == FLOAT_LIT)
			isFloat = true;
		switch(head->key.code) {
			case ADD_OP:
				answer += atof(temp->key.lexeme);
				break;
			case SUB_OP:
				answer -= atof(temp->key.lexeme);
				break;
			case MULT_OP:
				answer *= atof(temp->key.lexeme);
				break;
			case DIV_OP:
				answer /= atof(temp->key.lexeme);
				break;
		}
		temp = temp->child1;
	}
	result->code = isFloat? FLOAT_LIT : INT_LIT;
	sprintf(result->lexeme, isFloat? "%.2f": "%.f", answer); // ���� ���ڿ��� ��ȯ
	// �Ǽ��� ���ԵǾ��� ������ �Ҽ��� ��°�ڸ�����, ������ ���ԵǾ��� ������ �Ҽ��� ���� ���
	return result;
}

// �Ű������� 3���� �Լ�
element* func_type3(const struct TreeNode* const head)
{
	char* keywords[] = { "COND", "SUBST", "IF" };
	int keywords_len = sizeof(keywords) / sizeof(char*);	// keywords �迭 ����
	int func_index = find_func_index(head, keywords, keywords_len);

	element* result = malloc(sizeof(struct element));
	bool if_expr_is_true = false;	// IF func
	bool cond_expr_is_true[3] = { false, false, false };	// COND func

	switch (func_index + KEYWORD3)
	{
	case COND:
		cond_expr_is_true[0] = comparison(head->child1->child1);
		cond_expr_is_true[1] = comparison(head->child2->child1);
		cond_expr_is_true[2] = comparison(head->child3->child1);

		// ù��° expr�� ���̸�
		if (cond_expr_is_true[0]) {
			if (head->child1->child2->child2 == NULL)
				result = func_type1(head->child1->child2);	// �Ű������� 1����			
			else
				result = numeric_operation(head->child1->child2);	// �Ű������� 2����
		}
		// �ι�° expr�� ���̸�
		else if (cond_expr_is_true[1]) {
			if (head->child2->child2->child2 == NULL)
				result = func_type1(head->child2->child2);	// �Ű������� 1����			
			else
				result = numeric_operation(head->child2->child2);	// �Ű������� 2����
		}
		// ����° expr�� ���̸�
		else if (cond_expr_is_true[2]) {
			if (head->child3->child2->child2 == NULL)
				result = func_type1(head->child3->child2);	// �Ű������� 1����			
			else
				result = numeric_operation(head->child3->child2);	// �Ű������� 2����
		}
		break;
	case SUBST:
		switch (head->child1->key.code) {
		case INT_LIT: case ATOM: /*case T: */
			break;
		default:
			return error("1st argument has wrong type");
			break;
		}

		switch (head->child2->key.code) {
		case INT_LIT: case ATOM: /*case T: */
			break;
		default:
			return error("2nd argument has wrong type");
			break;
		}

		if (head->child3->key.code != LIST_CODE)
			return error("3rd argument should be LIST");

		result->code = LIST_CODE;

		for (int i = 0; head->child2->key.listElem[i] != NULL; i++) {
			result->listElem[i] = malloc(sizeof(struct element));

			if (!strcmp(head->child2->key.lexeme, head->child3->key.listElem[i]->lexeme))
				strcpy(result->listElem[i], head->child1->key.lexeme);
			else
				strcpy(result->listElem[i], head->child3->key.listElem[i]->lexeme);
		}
		break;
	case IF:
		// comparison �Լ��� element*�� ��ȯ�ϱ� ������ �����߽��ϴ�.
		if_expr_is_true = comparison(head->child1)->code - NIL;	// ���̸� 1(T-NIL), �����̸� 0(NIL-NIL)

		// child1(expr)�� true��,
		// child2(stmt1) ����;
		if (if_expr_is_true) {
			if (head->child2->child2 == NULL)
				result = func_type1(head->child2);	// �Ű������� 1����			
			else
				result = numeric_operation(head->child2);	// �Ű������� 2����
		}
		// child3(stmt2) ����
		else {
			if (head->child3->child2 == NULL)
				result = func_type1(head->child3);
			else
				result = numeric_operation(head->child3);
		}
		break;
	default:
		break;
	}

	return result;
}

// �Ű������� ������ �������� �Լ�
element* func_type4(const struct TreeNode* const head) {
	char* keywords[] = { "LIST", "APPEND" };
	int keywords_len = sizeof(keywords) / sizeof(char*);	// keywords �迭 ����
	int func_index = find_func_index(head, keywords, keywords_len);

	//Variable* var = find_variable(head->child1->key.lexeme);
	element* result = malloc(sizeof(struct element));
	TreeNode* temp = head->child1;
	int index, i;

	switch (func_index + KEYWORD4)
	{
	case LIST:
		result->code = LIST_CODE;

		index = 0;
		while (temp != NULL) {
			result->listElem[index] = malloc(sizeof(struct element));
			result->listElem[index]->code = temp->key.code;
			strcpy(result->listElem[index]->lexeme, temp->key.lexeme);

			index++;
			temp = temp->child1;
		}
		break;
	case APPEND:
		result->code = LIST_CODE;

		index = 0;
		while (temp != NULL) {
			for (i = 0; temp->key.listElem[i] != NULL; i++) {
				result->listElem[index + i] = malloc(sizeof(struct element));
				result->listElem[index + i]->code = temp->key.listElem[i]->code;
				strcpy(result->listElem[index + i]->lexeme, temp->key.listElem[i]->lexeme);
			}

			index += i;
			temp = temp->child1;
		}
		break;
	default:
		break;
	}

	return result;
}

// � �Լ����� ã��
int find_func_index(const struct TreeNode* const head, char* keywords[], int len)
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
element* make_listElem_cdr(const struct TreeNode* const head, element* result, int len)
{
	for (int i = 0; i < len - 1; i++) {
		result->listElem[i] = malloc(sizeof(struct element));
		result->listElem[i]->code = head->child1->key.listElem[i + 1]->code;
		strcpy(result->listElem[i]->lexeme, head->child1->key.listElem[i + 1]->lexeme);	// ù��° ���� ������ ������
	}

	return result;
}

// REVERSE FUNC
element* make_listElem_reverse(const struct TreeNode* const head, element* result, int len)
{
	for (int i = 0; i < len; i++) {
		result->listElem[i] = malloc(sizeof(struct element));
		result->listElem[i]->code = head->child1->key.listElem[len - 1 - i]->code;
		strcpy(result->listElem[i]->lexeme, head->child1->key.listElem[len - 1 - i]->lexeme);	// ��������
	}

	return result;
}

// EQUAL FUNC�� ��� ȣ���� �̿��� ����
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

/* error - ���� ó�� �Լ� */
static element* error(char* message){
    if(!isRunningError)
        printf("running error - %s\n", message);    
    isRunningError = true;
    return NULL;
}


void preorderIdentSearch(TreeNode* root) {
	Variable* p;
    if (root != NULL) {
        if(root->key.code == IDENT){
			p = find_variable(root->key.lexeme);
			if (p != NULL)
				root->key = p->value;
			else
				error("undefined variable name");
		}
        preorderIdentSearch(root->child1);
        preorderIdentSearch(root->child2);
        preorderIdentSearch(root->child3);
    }
}