#include <stdio.h>
#include "run.h"

void run(const struct TreeNode* const head)
{
	switch(head->key.code)
	{
		case FUNC_TYPE1:
			func_type1(head);
			break;
		case FUNC_TYPE2:
			break;
		case FUNC_TYPE3:
			break;
		case FUNC_TYPE4:
			break;
	}
}

void func_type1(const struct TreeNode* const head)
{
	char* keywords[] = { "REVERSE", "LENGTH", "ATOM", "NULL", "NUMBERP",
						"ZEROP", "MINUSP", "STRINGP" };
	int keywords_len = sizeof(keywords) / sizeof(char*);	// keywords 배열 길이
	int func_index = -1;

	for (int i = 0; i < keywords_len; i++) {
		if (head->key.lexeme == keywords[i]) {
			func_index = i;
			break;
		}

		switch (func_index + 100)
		{
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
}