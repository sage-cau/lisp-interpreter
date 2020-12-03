#include <stdio.h>
#include "run.h"

void run(const struct TreeNode* const head)
{
	int func_index = -1;
	
	for (int i = 0; i < KEYWORDS_LENGTH; i++) {
		if (head->key.lexeme == keywords[i]) {
			func_index = i;
			break;
		}
	}
	
	switch (func_index)
	{
	case SETQ:
		break;
	case LIST:
		break;
	/*
	* ...
	* 
		*/
	default:
		break;
	}
}
