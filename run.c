#include <stdio.h>
#include <stdlib.h>
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

void func_type1()
{
	char* keywords[] = { "CAR", "CDR", "CADDR", "REVERSE", "LENGTH",
						"ATOM", "NULL", "NUMBERP", "ZEROP", "MINUSP",
						"STRINGP" };
	int keywords_len = sizeof(keywords) / sizeof(char*);	// keywords 배열 길이
	int func_index = -1;

	for (int i = 0; i < keywords_len; i++) {
		if (head->key.lexeme == keywords[i]) {
			func_index = i;
			break;
		}
	}

	switch (func_index + 100)
	{
	case CAR:
		break;
	case CDR:
		break;
	case CADDR:
		break;
	case REVERSE:
		break;
	case LENGTH:
		break;
	case _ATOM:
		break;
	case _NULL:
		break;
	case NUMBERP:
		break;
	case ZEROP:
		break;
	case MINUSP:
		break;
	case STRINGP:
		break;
	default:
		break;
	}
}

void func_type2()
{
	char* keywords[] = { "SETQ", "NTH", "CONS", "MEMBER", "REMOVE"
						"EQUAL"};
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
