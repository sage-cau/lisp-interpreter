#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "total.h"

FILE* in_fp;

/*UI 함수*/
int lexer_interface();


/* main 함수 
- UI 제공 
- 입력값 lexer로 넘겨줌(넘길때는 파일로 넘겨주고 lexer로 분석된 출력값 전역변수로 저장)
- parser 작업(전역변수로 저장된 lexer 분석값으로 사용 포인터로 parser tree 받음)
(추가로 해야될 작업은 parser.c로 작업된 parser tree를 run.c로 넘겨주고 리턴값을 받아서 출력 정도??) */

void main()
{
    /* Open the input data file and process its contents */
    TreeNode *head1;
    char input[1000];
    lexer_interface();
    while (1) {
        printf(">> ");
        
        gets(input); // 입력 받기
        if (strcmp(input, "EXIT") == 0) { // EXIT면 종료
            printf("종료...");
            return 0;
        }
        in_fp = fopen("out.txt", "w");
        if (in_fp == NULL)
            printf("error");
        else
            fputs(input, in_fp); // 입력값 out.txt 파일에 저장

        fclose(in_fp);
        lexer(); // lexer 분석

        /* lexer 확인 호출(전역변수 지정) */
       for (int i = 0; i < num; i++) {
            printf("Next token: %d, Next lexeme: %s\n", tokens[i].code, tokens[i].lexeme);
        }

        head1 = parser(); // parser 분석

        /* parser 확인 함수 호출(포인터 지정) */
        preorderPrint(head1);
    }

    return;
}


/*시작때 메인화면 출력 함수*/
int lexer_interface() {
    printf("/***************************************************/\n");
    printf("/                                                   /\n");
    printf("/              LISP INTERPRETER !!!!!!              /\n");
    printf("/                                                   /\n");
    printf("/             장성현, 전찬웅, 정다희                /\n");
    printf("/                                                   /\n");
    printf("/          --프로그래밍언어론 프로젝트--            /\n");
    printf("/                                                   /\n");
    printf("/***************************************************/\n");
    printf("명령어를 입력하시오...(종료시 EXIT 입력)\n");
}

