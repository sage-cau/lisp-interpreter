#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "total.h"



/* Local Function declarations */
int lexer();
void addChar();
void getChar();
void getNonBlank();
int lex();
void ident_change();
void string_check();
void float_check();
void string_check2();
/* Global Variable */


int nextToken;
int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
char* keywords1[] = { "CAR", "CDR", "CADDR", "REVERSE", "LENGTH", "ATOM", "NULL",
                    "NUMBERP", "ZEROP", "MINUSP", "STRINGP", "PRINT" };
char* keywords2[] = { "DEFVAR", "SETQ", "NTH", "CONS", "MEMBER", "REMOVE", "EQUAL" };
char* keywords3[] = { "COND", "SUBST", "IF" };
char* keywords4[] = { "LIST", "APPEND" };

FILE* in_fp;

/******************************************/
/* lexer함수                              */
/******************************************/
int lexer()
{
    /* Open the input data file and process its contents */

    num = 0;
    in_fp = fopen("out.txt", "r"); // 파일 열어서 읽기
    getChar();

    do {
        lex();
    } while (nextToken != EOF);

    string_check(); // string 체크 띄어쓰기 되어 있는 IDENT-> STRING으로 묶기
    string_check2();
    float_check();  // 4.5 -> float로 분류
    ident_change(); // IDENT -> ATOM/STRING/IDENT 분류 이중리스트도 처리
    fclose(in_fp);

    return 0;
}

/******************************************
 * lookup - a function to lookup operators
 * and parentheses and return the token
 ******************************************/
int lookup(char ch) {
    switch (ch) {
    case '(':
        addChar();
        nextToken = LEFT_PAREN;
        break;
    case ')':
        addChar();
        nextToken = RIGHT_PAREN;
        break;
    case '+':
        addChar();
        nextToken = ADD_OP;
        break;
    case '-':
        addChar();
        nextToken = SUB_OP;
        break;
    case '*':
        addChar();
        nextToken = MULT_OP;
        break;
    case '/':
        addChar();
        nextToken = DIV_OP;
        break;
    case ';':
        addChar();
        nextToken = SEMI_COL;
        break;
    case '<':
        addChar();
        nextToken = LESS_COMP;
        break;
    case '>':
        addChar();
        nextToken = GREATER_COMP;
        break;
    case '=':
        addChar();
        nextToken = EQUAL_COMP;
        break;
    case '\'':
        addChar();
        nextToken = APOSTROPHE;
        break;
    case '#':
        addChar();
        nextToken = HASH;
        break;
    case '"':
        addChar();
        nextToken = DOUBLE_QUOT;
        break;
    case '.':
        addChar();
        nextToken = DOT;
        break;
    default:
        addChar();
        nextToken = EOF;
        break;
    }
    return nextToken;
}

/**************************************************/
/* addChar - a function to add nextChar to lexeme */
/**************************************************/
void addChar() {
    if (lexLen <= 98) {  // max length of Lexime is 99
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0; // '\0'
    }
    else {
        printf("Error - lexeme is too long \n");
    }
}

/*****************************************************/
/* getChar - a function to get the next character
          of input and determine its character class */
          /*****************************************************/
void getChar() {
    if ((nextChar = getc(in_fp)) != EOF) {
        nextChar = toupper(nextChar);   // to upper

        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else
            charClass = UNKNOWN;
    }
    else {
        charClass = EOF;
    }
}

/*****************************************************/
/* getNonBlank - a function to call getChar until it
           returns a non-whitespace character        */
           /*****************************************************/
void getNonBlank() {
    while (isspace(nextChar))
        getChar();
}

boolean isReservedWords1(char* s) {
    int len = sizeof(keywords1) / sizeof(keywords1[0]);

    for (int i = 0; i < len; i++) {
        if (!strcmp(keywords1[i], s))
            return TRUE;
    }

    return FALSE;
}

boolean isReservedWords2(char* s) {
    int len = sizeof(keywords2) / sizeof(keywords2[0]);

    for (int i = 0; i < len; i++) {
        if (!strcmp(keywords2[i], s))
            return TRUE;
    }

    return FALSE;
}

boolean isReservedWords3(char* s) {
    int len = sizeof(keywords3) / sizeof(keywords3[0]);

    for (int i = 0; i < len; i++) {
        if (!strcmp(keywords3[i], s))
            return TRUE;
    }

    return FALSE;
}

boolean isReservedWords4(char* s) {
    int len = sizeof(keywords4) / sizeof(keywords4[0]);

    for (int i = 0; i < len; i++) {
        if (!strcmp(keywords4[i], s))
            return TRUE;
    }

    return FALSE;
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic
         expressions                                 */
         /*****************************************************/
int lex() {
    lexLen = 0;
    getNonBlank();

    switch (charClass) {
        /* Parse identifiers */
    case LETTER:
        addChar();
        getChar();
        while (charClass == LETTER || charClass == DIGIT) {
            addChar();
            getChar();
        }
        /* function type 세분화 */
        if (isReservedWords1(lexeme))
            nextToken = FUNC_TYPE1;
        else if (isReservedWords2(lexeme))
            nextToken = FUNC_TYPE2;
        else if (isReservedWords3(lexeme))
            nextToken = FUNC_TYPE3;
        else if (isReservedWords4(lexeme))
            nextToken = FUNC_TYPE4;
        else
            nextToken = IDENT;
        break;

        /* Parse integer literals */
    case DIGIT:
        addChar();
        getChar();
        while (charClass == DIGIT) {
            addChar();
            getChar();
        }
        nextToken = INT_LIT;
        break;

        /* Parentheses and operators */
    case UNKNOWN:
        lookup(nextChar);
        getChar();
        break;

        /* EOF */
    case EOF:
        nextToken = EOF;
        lexeme[0] = 'E';
        lexeme[1] = 'O';
        lexeme[2] = 'F';
        lexeme[3] = 0;
        break;
    } /* End of switch */

    // printf("Next token is: %d, Next lexeme is %s\n", nextToken, lexeme);
// printf("num : %d\n", num);
if (nextToken != -1) { // EOF 값은 제외했습니다.
    tokens[num].code = nextToken;
    strcpy(tokens[num].lexeme, lexeme);
    num++;
}
//tokens 구조체에 lexer 값(숫자, 문자 대입)

return nextToken;
} /* End of function lex */


/* " / HI / THERE / " -> " / HI THERE / " 다음처럼 동작 */
void string_check()
{
    int ele_num, sub_ele_num;
    int start, end, num_string;

    for (ele_num = 0; ele_num < num; ele_num++) {

        sub_ele_num = ele_num;
        if (tokens[sub_ele_num].code == DOUBLE_QUOT) {

            start = ++sub_ele_num;
            end = start;

            while (tokens[++sub_ele_num].code == IDENT) {
                strcat(tokens[start].lexeme, " ");
                strcat(tokens[start].lexeme, tokens[sub_ele_num].lexeme);
                end++;
            }
            num_string = end - start;

            for (int i = start + 1; i < num; i++) {
                tokens[i].code = tokens[i + num_string].code;
                strcpy(tokens[i].lexeme, tokens[i + num_string].lexeme);
            }
            num -= num_string;
        }
    }
    return;
}


/* " IDENT " -> " STRING/ATOM/IDENT " 다음으로 변환 */
void ident_change()
{
    int ele_num, sub_ele_num;
    int list_end = 0;

    for (ele_num = 0; ele_num < num; ele_num++) {

        sub_ele_num = ele_num;

        if (tokens[sub_ele_num].code == DOUBLE_QUOT) {
            if (tokens[++sub_ele_num].code == IDENT)
                tokens[sub_ele_num].code = STRING;
        }
        else if (tokens[sub_ele_num].code == APOSTROPHE) {
            sub_ele_num++;
            if (tokens[sub_ele_num].code == LEFT_PAREN) {
                list_end++;
                while (list_end > 0) {
                    sub_ele_num++;
                    if (tokens[sub_ele_num].code == IDENT)
                        tokens[sub_ele_num].code = ATOM;
                    else if (tokens[sub_ele_num].code == LEFT_PAREN)
                        list_end++;
                    else if (tokens[sub_ele_num].code == RIGHT_PAREN)
                        list_end--;
                }
            }

            else if (tokens[sub_ele_num].code == IDENT)
                tokens[sub_ele_num].code = ATOM;

        }
    }
    return;
}


void string_check2() {
    int ele_num, sub_ele_num;

    for (ele_num = 0; ele_num < num; ele_num++) {

        sub_ele_num = ele_num;
        
        if (tokens[sub_ele_num].code == DOUBLE_QUOT) {
            strcat(tokens[sub_ele_num].lexeme, tokens[sub_ele_num + 1].lexeme);
            strcat(tokens[sub_ele_num].lexeme, tokens[sub_ele_num + 2].lexeme);
            tokens[sub_ele_num].code = STRING;

            for (int i = sub_ele_num + 1; i < num; i++) {
                tokens[i].code = tokens[i + 2].code;
                strcpy(tokens[i].lexeme, tokens[i + 2].lexeme);
            }
            num -= 2;
        }

        if (strcmp(tokens[sub_ele_num].lexeme, "MINUSP") == 0){
            if (tokens[sub_ele_num + 1].code == SUB_OP) {
                if (tokens[sub_ele_num + 2].code == INT_LIT) {
                    strcat(tokens[sub_ele_num+1].lexeme, tokens[sub_ele_num + 2].lexeme);
                    tokens[sub_ele_num + 1].code = INT_LIT;
                }
                else if (tokens[sub_ele_num + 2].code == FLOAT_LIT) {
                    strcat(tokens[sub_ele_num + 1].lexeme, tokens[sub_ele_num + 2].lexeme);
                    tokens[sub_ele_num + 1].code = FLOAT_LIT;
                }
                if(num > sub_ele_num+2){
                    for (int i = sub_ele_num + 2; i < num; i++) {
                        tokens[i].code = tokens[i + 1].code;
                        strcpy(tokens[i].lexeme, tokens[i + 1].lexeme);
                    }
                }
                num -= 1;
            }
        }
    }
    return;
}

/* " (/+/4/./5/5/./5/) " -> " (/+/4.5/5.5/) " 다음으로 변환 */
void float_check()
{
    int ele_num, sub_ele_num;
    int start;

    for (ele_num = 0; ele_num < num; ele_num++) {

        sub_ele_num = ele_num;
        if (tokens[sub_ele_num].code == INT_LIT) {
            start = sub_ele_num;
            if (tokens[++sub_ele_num].code == DOT) {
                if (tokens[++sub_ele_num].code == INT_LIT) {
                    strcat(tokens[start].lexeme, tokens[start + 1].lexeme);
                    strcat(tokens[start].lexeme, tokens[start + 2].lexeme);
                    tokens[start].code = FLOAT_LIT;

                    for (int i = start + 1; i < num; i++) {
                        tokens[i].code = tokens[i + 2].code;
                        strcpy(tokens[i].lexeme, tokens[i + 2].lexeme);
                    }
                    num -= 2;
                }
            }
        }

    }
    return;
}