#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "run.h"

#define INPUT_MAX 2048

int main()
{
    struct Variable* v_head = malloc(sizeof(struct Variable));

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
        //run(head, v_head);   // paser의 반환값(head), 변수 저장 head
    }

    return 0;
}