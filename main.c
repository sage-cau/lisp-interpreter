#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "total.h"

FILE* in_fp;

/*UI �Լ�*/
int lexer_interface();


/* main �Լ� 
- UI ���� 
- �Է°� lexer�� �Ѱ���(�ѱ涧�� ���Ϸ� �Ѱ��ְ� lexer�� �м��� ��°� ���������� ����)
- parser �۾�(���������� ����� lexer �м������� ��� �����ͷ� parser tree ����)
(�߰��� �ؾߵ� �۾��� parser.c�� �۾��� parser tree�� run.c�� �Ѱ��ְ� ���ϰ��� �޾Ƽ� ��� ����??) */

void main()
{
    /* Open the input data file and process its contents */
    TreeNode *head1;
    char input[1000];
    lexer_interface();
    while (1) {
        printf(">> ");
        
        gets(input); // �Է� �ޱ�
        if (strcmp(input, "EXIT") == 0) { // EXIT�� ����
            printf("����...");
            return 0;
        }
        in_fp = fopen("out.txt", "w");
        if (in_fp == NULL)
            printf("error");
        else
            fputs(input, in_fp); // �Է°� out.txt ���Ͽ� ����

        fclose(in_fp);
        lexer(); // lexer �м�

        /* lexer Ȯ�� ȣ��(�������� ����) */
       for (int i = 0; i < num; i++) {
            printf("Next token: %d, Next lexeme: %s\n", tokens[i].code, tokens[i].lexeme);
        }

        head1 = parser(); // parser �м�

        /* parser Ȯ�� �Լ� ȣ��(������ ����) */
        preorderPrint(head1);
    }

    return;
}


/*���۶� ����ȭ�� ��� �Լ�*/
int lexer_interface() {
    printf("/***************************************************/\n");
    printf("/                                                   /\n");
    printf("/              LISP INTERPRETER !!!!!!              /\n");
    printf("/                                                   /\n");
    printf("/             �强��, ������, ������                /\n");
    printf("/                                                   /\n");
    printf("/          --���α׷��־��� ������Ʈ--            /\n");
    printf("/                                                   /\n");
    printf("/***************************************************/\n");
    printf("��ɾ �Է��Ͻÿ�...(����� EXIT �Է�)\n");
}

