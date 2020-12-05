#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "total.h"

bool isSyntaxError = false;
static element* tokenList;
int curr;
static element nextToken; //lexer�� nextChar�� ������ ����� �����Դϴ�.

/* Token codes */
#define INT_LIT 10      // 0 1 2...
#define IDENT 11        // X Y Z...
#define FLOAT_LIT 12    // 1.5 2.5...

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

/*local �Լ� ����*/
static TreeNode* error(char* message);
static void getToken();
static TreeNode* block();
static TreeNode* par();
static TreeNode* list();
TreeNode* new_node(element key);
void postorderDelete(TreeNode* root);
void destroyTree(TreeNode* root);
void printTree(TreeNode* root);
void preorderPrint(TreeNode* root);


/*****    main driver    *****/
TreeNode* parser() {
    curr = -1; // �ʱ�ȭ
    TreeNode* head = NULL;
    tokenList = tokens;
    head = block();
    if (isSyntaxError) {
        destroyTree(head);
        return NULL;
    }
    return head;
}


/* error - ���� ó�� �Լ� */
static TreeNode* error(char* message) {
    if (!isSyntaxError)
        printf("syntax error - %s\n", message);
    isSyntaxError = true;
    return NULL;
}


/* getToken - get the next token of tokenList */
static void getToken() {
    if (tokenList[++curr].code != -1)
        nextToken = tokenList[curr];
}

/* block - parsing subprogram for <BLOCK> */
/*<BLOCK> -> <PAR>
            | (<FUNC_TYPE1> <BLOCK>)
            | (<FUNC_TYPE2> <BLOCK> <BLOCK>)
            | (<FUNC_TYPE3> <BLOCK> <BLOCK> <BLOCK>)
            | (<FUNC_TYPE4> (<BLOCK> <BLOCK>) (<BLOCK> <BLOCK>) (<BLOCK> <BLOCK>))
*/
static TreeNode* block() {
    TreeNode* root = NULL;

    getToken();
    if (nextToken.code != LEFT_PAREN) {
        return par();  // <PAR>
    }
    getToken();
    switch (nextToken.code) {
        // �Լ� ����1 - �Ű����� block�� 1��
    case FUNC_TYPE1:
        root = new_node(nextToken);
        root->child1 = block();       // ù��° �ڽĿ��� block()
        break;

        // �Լ� ����2 - �Ű����� block�� 2��
    case FUNC_TYPE2:
    case ADD_OP: case SUB_OP: case MULT_OP: case DIV_OP:
    case LESS_COMP: case GREATER_COMP: case EQUAL_COMP:
        root = new_node(nextToken);
        root->child1 = block();      // ù��° �ڽĿ��� block()
        root->child2 = block();     // �ι��� �ڽĿ��� block()
        break;

        // �Լ� ����3 - �Ű����� block�� 3��
    case FUNC_TYPE3:
        root = new_node(nextToken);
        root->child1 = block();      // ù��° �ڽĿ��� block()
        root->child2 = block();     // �ι��� �ڽĿ��� block()
        root->child3 = block();      // ����° �ڽĿ��� block()
        break;

        // �Լ� ����4 - COND �Լ�
    case FUNC_TYPE4:
        root = new_node(nextToken);
        // (<��1���ǹ�> <���๮1>)
        getToken();
        if (nextToken.code == LEFT_PAREN) {
            root->child1 = new_node(nextToken); // ���ǻ� '(' ��� ��� �߰�
            root->child1->child1 = block();     // <��1���ǹ�>
            root->child1->child2 = block();     // <���๮1>
            getToken();
            if (nextToken.code != RIGHT_PAREN)
                return error("right paren is missing in block");

            // (<��2���ǹ�> <���๮2>)
            // ������ 1�� ����
            getToken();
            if (nextToken.code == LEFT_PAREN) {
                root->child2 = new_node(nextToken);
                root->child2->child1 = block();
                root->child2->child2 = block();
                getToken();
                if (nextToken.code != RIGHT_PAREN)
                    return error("right paren is missing in block");

                // (<��3���ǹ�> <���๮3>)
                // ������ 1�� ����
                getToken();
                if (nextToken.code == LEFT_PAREN) {
                    root->child3 = new_node(nextToken);
                    root->child3->child1 = block();
                    root->child3->child2 = block();
                    getToken();
                    if (nextToken.code != RIGHT_PAREN)
                        return error("right paren is missing in block");
                    break;
                }
            }
        }
        return error("left paren is missing in block");
    default:
        return error("undefined function in block");
    }
    getToken();
    if (nextToken.code != RIGHT_PAREN)
        return error("right paren is missing in block");

    return root;
}


/* par - parsing subprogram for <PAR> */
/*<PAR> -> <id> | <num> | <string> | <atom> | <nil> | <LIST>
*/
static TreeNode* par() {
    if (nextToken.code == IDENT || nextToken.code == INT_LIT/*NUM*/ || nextToken.code == STRING
        || nextToken.code == ATOM || nextToken.code == NIL || nextToken.code == FLOAT_LIT)
        return new_node(nextToken);

    return list();
}


/* list - parsing subprogram for <LIST> */
/*   <LIST> -> ��(<PAR>)
*/
// (����) ��(X 1 2) ��� ����Ʈ�� ���
// code�� LIST_CODE, listElem�� {{IDENT, X}, {INT_LIT, 1}, {INT_LIT, 2}}

static TreeNode* list() {
    TreeNode* root = new_node(nextToken);
    root->key.code = LIST_CODE;

    TreeNode* temp = NULL;
    int pos = 0;
    if (nextToken.code == APOSTROPHE) {
        getToken();
        if (nextToken.code == LEFT_PAREN) {
            while (1) {
                getToken();
                if (nextToken.code == RIGHT_PAREN)
                    break;
                if (nextToken.code == EOF)
                    return error("no more tokens to parse");

                temp = par();
                if (temp->key.code != LIST_CODE) {
                    root->key.listElem[pos] = &temp->key;
                }
                else {  //����Ʈ�� ���� �߿� �� ����Ʈ�� �ִ� ���..
                    for (int i = 0; temp->key.listElem[i] != NULL; i++)
                        root->key.listElem[pos]->listElem[i] = temp->key.listElem[i];
                }
                pos++;
            }
            return root;
        }
    }
    return error("there is a token that parser can't validate!");
}
// ���� �ڵ�δ� '((x) y z) ó�� ���Ұ� ( )�� �ѷ��ο��ִ°� �ν� ����. ������ ����..
// (LIST X 1 2) ���� X�� IDENT �ε�, '(X 1 2) ���� X�� ATOM �̶�� �ϳ׿�... <LIST> �� Rule �ٲ�� �� ����..



/***********************************/
/*** Ʈ�� �ڷᱸ���� �ʿ��� �Լ��� ***/
/***********************************/


/* �Ϲ����� Ʈ���� ��� ���� �Լ�*/
TreeNode* new_node(element key) {
    TreeNode* temp = (TreeNode*)malloc(sizeof(TreeNode));
    temp->key = key;
    temp->child1 = temp->child2 = temp->child3 = NULL;

    //TreeNode* temp = {key, NULL, NULL, NULL}; //�� ����
    return temp;
}

/* ������ȸ�� Ʈ���� �����ϴ� �Լ�*/
void postorderDelete(TreeNode* root) {
    if (root != NULL)
    {
        postorderDelete(root->child1);
        postorderDelete(root->child2);
        postorderDelete(root->child3);
        free(root);  // ��� ����
    }
}

/* Ʈ�� ��ü �ı� �Լ� */
void destroyTree(TreeNode* root)
{
    postorderDelete(root);
}

/* Ʈ�� ��� �Լ� (�׽�Ʈ��) */
void printTree(TreeNode* root) {
    if (root->key.code != LIST_CODE)
        printf("���� ��� {code: %d lexeme: %s}", root->key.code, root->key.lexeme);
    if (root->key.code == LIST_CODE) {
        printf(" *�� ���� lexeme ��� listElem�� �����ϴ�: ");
        for (int i = 0; root->key.listElem[i] != NULL; i++)
            printf("{%d, %s} ", root->key.listElem[i]->code, root->key.listElem[i]->lexeme);
    }
    if (root->child1 != NULL)
        printf("\n - �ڽ� ���1: {code: %d lexeme: %s}", root->child1->key.code, root->child1->key.lexeme);
    if (root->child2 != NULL)
        printf(" ���2: {code: %d lexeme: %s}", root->child2->key.code, root->child2->key.lexeme);
    if (root->child3 != NULL)
        printf(" ���3: {code: %d lexeme: %s}", root->child3->key.code, root->child3->key.lexeme);
    printf("\n");
}
void preorderPrint(TreeNode* root) {
    if (root != NULL) {
        printTree(root);
        preorderPrint(root->child1);
        preorderPrint(root->child2);
        preorderPrint(root->child3);
    }
}