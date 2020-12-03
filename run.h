#ifndef RUN_H
#define RUN_H

typedef struct element {
	int code;
	union {
		char lexeme[100];   // �⺻
		struct element* listElem[100]; //code�� LIST_CODE�� ��쿡�� ���!
	};
} element;

typedef struct TreeNode {
	element key;
	struct TreeNode* child1, * child2, * child3;
} TreeNode;

typedef enum
{
	SETQ = 0, 
	LIST, 
	CAR, 
	CDR, 
	CADDR, 
	NTH, 
	CONS,
	REVERSE, 
	APPEND, 
	LENGTH, 
	MEMBER, 
	ASSOC,
	REMOVE,
	SUBST, 
	ATOM, 
	_NULL,	// "NULL"
	NUMBERP,
	ZEROP,
	MINUSP, 
	EQUAL, 
	STRINGP, 
	IF, 
	COND,
} Keyword;

#define KEYWORDS_LENGTH sizeof(keywords) / sizeof(char*)

char* keywords[] = { "SETQ", "LIST", "CAR", "CDR", "CADDR", "NTH", "CONS",
					 "REVERSE", "APPEND", "LENGTH", "MEMBER", "ASSOC",
					 "REMOVE", "SUBST", "ATOM", "NULL", "NUMBERP", "ZEROP",
					 "MINUSP", "EQUAL", "STRINGP", "IF", "COND" };

void run(const struct TreeNode*);
void lisp_setq();	// setq�� ó���ϴ� �Լ�

#endif
