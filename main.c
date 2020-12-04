#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
//#include "run.h"

/// <"run.c" for testing>
typedef struct element {
    int code;
    union {
        char lexeme[100];   // 기본
        struct element* listElem[100]; //code가 LIST_CODE인 경우에만 사용!
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

        // 입력받기
        printf("> ");
        fgets(input, INPUT_MAX, stdin);
        //fprintf(stdout, &input);
  
        /* LEXER */
        /* PASER */

        /* RUN */
        //run(head);   // paser의 반환값(head)
    }

    return 0;
}