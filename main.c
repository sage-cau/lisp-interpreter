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

        // �Է¹ޱ�
        printf("> ");
        fgets(input, INPUT_MAX, stdin);
        //fprintf(stdout, &input);
  
        /* LEXER */
        /* PASER */

        /* RUN */
        //run(head, v_head);   // paser�� ��ȯ��(head), ���� ���� head
    }

    return 0;
}