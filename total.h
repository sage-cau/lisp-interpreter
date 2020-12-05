
/*lexer / parser / main 공통 헤더 파일입니다. 
공통으로 사용하는 함수나 자료구조 전역변수등이 많아서 하나의 헤더 파일로 정리했습니다*/

/* 정의한 자료구조 */

typedef struct element {
    int code;
    union {
        char lexeme[100];
        struct element* listElem[100];
    };
} element;

typedef struct TreeNode {
    element key;
    struct TreeNode* child1, * child2, * child3;
} TreeNode;

/* boolean type define */
typedef enum _boolean {
    FALSE,
    TRUE
} boolean;

#define FALSE 0
#define TRUE 1

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token codes */
#define INT_LIT 10      // 0 1 2...
#define IDENT 11        // X Y Z...
#define FLOAT_LIT 12    // 1.5 2.5...

#define FUNC_TYPE1 12
#define FUNC_TYPE2 13
#define FUNC_TYPE3 14
#define FUNC_TYPE4 15

#define STRING 16 // "X", "Y", "GOOD"
#define ATOM 17 // 'X, 'Y, 'GOOD
#define NIL 18
#define T 19

#define ADD_OP 21       // +
#define SUB_OP 22       // -
#define MULT_OP 23      // *
#define DIV_OP 24       // /
#define LEFT_PAREN 25   // (
#define RIGHT_PAREN 26  // )

#define LESS_COMP 27    // <
#define GREATER_COMP 28 // >
#define EQUAL_COMP 29   // =
#define LESS_EQUAL_COMP 30    // <=
#define GREATER_EQUAL_COMP 31 // >=

#define APOSTROPHE 32   // '

#define HASH 33         // #
#define DOUBLE_QUOT 34  // "
#define DOT 35          // .
#define SEMI_COL 40     // ;

#define LIST_CODE 50

/* Global Variable */

element tokens[100]; // return lexer 값
int num; // tokens 수


/* lexer 함수들 */

int lexer();
void addChar();
void getChar();
void getNonBlank();
int lex();
void ident_change();
void string_check();
void float_check();

/* parser 함수들 */

TreeNode* new_node(element key);
void postorderDelete(TreeNode* root);
void destroyTree(TreeNode* root);
void printTree(TreeNode* root);
void preorderPrint(TreeNode* root);
TreeNode* parser();