#ifndef RUN_H
#define RUN_H

/* Token codes */
#define INT_LIT 10      // 0 1 2...
#define IDENT 11        // X Y Z...

//#define KEYWORD 12
#define FUNC_TYPE1 12
#define FUNC_TYPE2 13
#define FUNC_TYPE3 14
#define FUNC_TYPE4 15

#define STRING 16     // "X" "Y" "GOOD" (�߰��ؾ� ��)
#define ATOM 17     // 'X 'Y 'GOOD (�߰��ؾ� ��)
#define NIL 18      // (�߰��ؾ� ��)

#define ADD_OP 21       // +
#define SUB_OP 22       // -
#define MULT_OP 23      // *
#define DIV_OP 24       // /
#define LEFT_PAREN 25   // (
#define RIGHT_PAREN 26  // )
#define LESS_COMP 27    // <
#define GREATER_COMP 28 // >
#define EQUAL_COMP 29   // =
#define NOT_COMP 30     // !
#define APOSTROPHE 31   // '
#define BACKSLASH 32    // "\"
#define HASH 33         // #
#define DOUBLE_QUOT 34  // "
#define DOT 35          // .
#define SEMI_COL 40     // ;

#define LIST_CODE 50        // ����Ʈ���� �ǹ��ϴ� token code (���ǻ� �߰�)

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

// ���� ����
typedef struct Variable {
	char* name;
	element value;
	struct Variable* next;
} Variable;

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
} Keyword1;	// func_type1�� ���ϴ� �Լ�

typedef enum
{
	SETQ = 200,
	NTH,
	CONS,
	MEMBER,
	REMOVE,
	EQUAL,
} Keyword2;	// func_type2�� ���ϴ� �Լ�

void run(const struct TreeNode* const, const struct Variable* const);

element* func_type1();
element* func_type2();

int find_func_index(char* [], int);
int var_listElem_length(const struct Variable* const);
int tree_listElem_length(const struct TreeNode* const);
Variable* find_variable(char*);
element* make_listElem_cdr(element*, int);
element* make_listElem_reverse(element*, int);

void print_l(element*);

#endif