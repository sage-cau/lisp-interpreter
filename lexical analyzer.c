#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctype.h>
#include <string.h>

/* Global Variable */
int nextToken;
int charClass;
char lexeme [100];
char nextChar;
int lexLen;
int token;
int nextToken;
char* keywords[] = { "SETQ", "LIST", "CAR", "CDR", "CADDR", "NTH", "CONS", 
                     "REVERSE", "APPEND", "LENGTH", "CLUB", "MEMBER", "ASSOC", 
                     "REMOVE", "SUBST", "ATOM", "NULL", "NUMBERP", "ZEROP", 
                     "MINUSP", "EQUAL", "STRINGP", "IF", "COND" };

FILE* in_fp;
FILE* out_fp;

/* Local Function declarations */
void addChar();
void getChar();
void getNonBlank();
int lex();

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
#define KEYWORD 12
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



/******************************************/
/* main driver                            */
/******************************************/
int main() 
{
    /* Open the input data file and process its contents */
    if ((in_fp = fopen("lisp.in", "r")) == NULL) {
        printf("ERROR - cannot open code.in \n");
    } else {
        out_fp = fopen("LISP.out", "a");

        getChar();

        do {
            lex();
        } while (nextToken != EOF);

        fclose(out_fp);
        fclose(in_fp);
        printf("SUCCESS\n");
    }

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
        case '!':
            addChar();
            nextToken = NOT_COMP;
            break;
        case '\'':
            addChar();
            nextToken = APOSTROPHE;
            break;
        case '\\':
            addChar();
            nextToken = BACKSLASH;
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
    } else {
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

boolean isReservedWords(char* s) {
    int len = sizeof(keywords) / sizeof(keywords[0]);

    for (int i = 0; i < len; i++) {
        if (!strcmp(keywords[i], s))
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
            if (isReservedWords(lexeme))
                nextToken = KEYWORD;
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

    fprintf(out_fp, "Next token is: %d, Next lexeme is %s\n", nextToken, lexeme);
    return nextToken;
} /* End of function lex */
