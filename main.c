#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
//#include "run.h"

/// <"run.c" for testing>
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
/// </"run.c" for testing>

#define INPUT_MAX 2048

int main()
{
    while (!feof(stdin))
    {
        char input[INPUT_MAX];

        // �Է¹ޱ�
        printf("> ");
        fgets(input, INPUT_MAX, stdin);
        //fprintf(stdout, &input);
  
        /* LEXER */
        /* PASER */

        /* RUN */
        //run(head);   // paser�� ��ȯ��(head)
    }

    return 0;
}