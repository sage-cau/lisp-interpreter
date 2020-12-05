#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "total.h"

bool isSyntaxError = false;
static element* tokenList;
int curr;
static element nextToken; //lexer의 nextChar과 역할이 비슷한 변수입니다.


/*local 함수 선언*/
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
    curr = -1; // 초기화
    TreeNode* head = NULL;
    tokenList = tokens;
    head = block();
    if (isSyntaxError) {
        destroyTree(head);
        return NULL;
    }
    return head;
}


/* error - 에러 처리 함수 */
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
        // 함수 유형1 - 매개변수 block이 1개
    case FUNC_TYPE1:
        root = new_node(nextToken);
        root->child1 = block();       // 첫번째 자식에서 block()
        break;

        // 함수 유형2 - 매개변수 block이 2개
    case FUNC_TYPE2:
    case EQUAL_COMP:
        root = new_node(nextToken);
        root->child1 = block();      // 첫번째 자식에서 block()
        root->child2 = block();     // 두번재 자식에서 block()
        break;

    case LESS_COMP:
        getToken();
        if (nextToken.code == EQUAL_COMP) { //<= 인 경우
            nextToken.code = LESS_EQUAL_COMP;
            strcpy(nextToken.lexeme, "<=");
        }
        root = new_node(nextToken);
        root->child1 = block();      // 첫번째 자식에서 block()
        root->child2 = block();     // 두번재 자식에서 block()
        break;

    case GREATER_COMP:
        getToken();
        if (nextToken.code == EQUAL_COMP) { //>= 인 경우
            nextToken.code = GREATER_EQUAL_COMP;
            strcpy(nextToken.lexeme, ">=");
        }
        root = new_node(nextToken);
        root->child1 = block();      // 첫번째 자식에서 block()
        root->child2 = block();     // 두번재 자식에서 block()
        break;

    case ADD_OP: case SUB_OP: case MULT_OP: case DIV_OP:
        root = new_node(nextToken);
        TreeNode* temp = root;
        while(1) {
            getToken();
            if (nextToken.code == INT_LIT || nextToken.code == FLOAT_LIT) {
                temp->child1 = new_node(nextToken);
                temp = temp->child1;
            }
            else if (nextToken.code == RIGHT_PAREN)
                break;
            else
                return error("wrong operand");
        }
        break;
        // 함수 유형3 - 매개변수 block이 3개
    case FUNC_TYPE3:
        root = new_node(nextToken);
        root->child1 = block();      // 첫번째 자식에서 block()
        root->child2 = block();     // 두번재 자식에서 block()
        root->child3 = block();      // 세번째 자식에서 block()
        break;

        // 함수 유형4 - COND 함수
    case FUNC_TYPE4:
        root = new_node(nextToken);
        // (<제1조건문> <수행문1>)
        getToken();
        if (nextToken.code == LEFT_PAREN) {
            root->child1 = new_node(nextToken); // 편의상 '(' 담는 노드 추가
            root->child1->child1 = block();     // <제1조건문>
            root->child1->child2 = block();     // <수행문1>
            getToken();
            if (nextToken.code != RIGHT_PAREN)
                return error("right paren is missing in block");

            // (<제2조건문> <수행문2>)
            // 원리는 1과 동일
            getToken();
            if (nextToken.code == LEFT_PAREN) {
                root->child2 = new_node(nextToken);
                root->child2->child1 = block();
                root->child2->child2 = block();
                getToken();
                if (nextToken.code != RIGHT_PAREN)
                    return error("right paren is missing in block");

                // (<제3조건문> <수행문3>)
                // 원리는 1과 동일
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

    if (nextToken.code == APOSTROPHE){
        getToken();
        if (nextToken.code == LEFT_PAREN)
            return list();
    }
    
    return error("there is a token that parser can't validate!");
}


/* list - parsing subprogram for <LIST> */
/*   <LIST> -> ‘(<PAR>)
*/
// (예시) ‘(X 1 2) 라는 리스트의 경우
// code는 LIST_CODE, listElem은 {{IDENT, X}, {INT_LIT, 1}, {INT_LIT, 2}}

static TreeNode* list() {
    TreeNode* root = new_node(nextToken);
    root->key.code = LIST_CODE;

    TreeNode* temp = NULL;
    int pos = 0;  // pos: root의 listElem의 index
    while(1) {
        getToken();
        if (nextToken.code == RIGHT_PAREN)
            break;
        else if (nextToken.code == EOF)
            return error("no more tokens to parse");

        else if (nextToken.code == LEFT_PAREN)
            temp = list();
        else
            temp = par();
        root->key.listElem[pos] = &temp->key;  
        pos++;
    }
    return root;
}


/***********************************/
/*** 트리 자료구조에 필요한 함수들 ***/
/***********************************/


/* 일반적인 트리의 노드 생성 함수*/
TreeNode* new_node(element key) {
    TreeNode* temp = (TreeNode*)malloc(sizeof(TreeNode));
    temp->key = key;
    temp->child1 = temp->child2 = temp->child3 = NULL;

    //TreeNode* temp = {key, NULL, NULL, NULL}; //와 동일
    return temp;
}

/* 후위순회로 트리를 삭제하는 함수*/
void postorderDelete(TreeNode* root) {
    if (root != NULL)
    {
        postorderDelete(root->child1);
        postorderDelete(root->child2);
        postorderDelete(root->child3);
        free(root);  // 노드 삭제
    }
}

/* 트리 전체 파기 함수 */
void destroyTree(TreeNode* root)
{
    postorderDelete(root);
}

/* 트리 출력 함수 (테스트용) */
void printTree(TreeNode* root) {
    if (root->key.code != LIST_CODE)
        printf("현재 노드 {code: %d lexeme: %s}", root->key.code, root->key.lexeme);
    if (root->key.code == LIST_CODE) {
        printf(" *이 노드는 lexeme 대신 listElem를 갖습니다: ");
        for (int i = 0; root->key.listElem[i] != NULL; i++)
            printf("{%d, %s} ", root->key.listElem[i]->code, root->key.listElem[i]->lexeme);
    }
    if (root->child1 != NULL)
        printf("\n - 자식 노드1: {code: %d lexeme: %s}", root->child1->key.code, root->child1->key.lexeme);
    if (root->child2 != NULL)
        printf(" 노드2: {code: %d lexeme: %s}", root->child2->key.code, root->child2->key.lexeme);
    if (root->child3 != NULL)
        printf(" 노드3: {code: %d lexeme: %s}", root->child3->key.code, root->child3->key.lexeme);
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