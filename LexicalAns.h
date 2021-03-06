/*
	Author: Lý Việt Dũng - Hanoi University of Science and Technology
	Date created: 2nd, October 2013
	Operating System: Ubuntu 13.04 Desktop x86_64
	Compiler: GNU compiler 4.7.3
	Program: PL0 translate program
	Module: Lexical analysis

	Description: this is header file for lexical analysis process
*/

#ifndef _LEXICAL_ANS_MODULE_H_
#define _LEXICAL_ANS_MODULE_H_

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

//Define some const here

#define MAX_IDEN_LENGTH 20
#define MAX_NUMBER_LENGTH 5
#define MAX_TOKEN 5000
#define END_LEXICAL_TABLE -1
#define INDEX_NOT_FOUND -1
#define MAX_LEXICAL_KEYWORD 42

#define CHECK_FIRST_COMMENT_STATUS(prev, cur) ((prev == '/') && (cur == '*'))
#define CHECK_END_FIRST_COMMENT(prev, cur) ((prev == '*') && (cur == '/'))
#define CHECK_SECOND_COMMENT_STATUS(prev, cur) ((prev == '/') && (cur == '/'))
#define CHECK_END_SECOND_COMMENT(cur) (cur == '\n')
#define BEGIN_WORD(cur) (((cur >= 'a') && (cur <= 'z')) || ((cur >= 'A') && (cur <= 'Z')) || (cur == '_'))
#define CONDITION_WORD(cur) (((cur >= 'a') && (cur <= 'z')) || ((cur >= 'A') && (cur <= 'Z')) || ((cur >= '0') && (cur <= '9')) || (cur == '_'))

#define CHECK_KEYWORD(token) ((strcmp(token, lexical_keyword_parsing[kBegin]) == 0) || (strcmp(token, lexical_keyword_parsing[kEnd]) == 0) || (strcmp(token, lexical_keyword_parsing[kIf]) == 0) || (strcmp(token, lexical_keyword_parsing[kThen]) == 0) || (strcmp(token, lexical_keyword_parsing[kWhile]) == 0) || (strcmp(token, lexical_keyword_parsing[kDo]) == 0) || (strcmp(token, lexical_keyword_parsing[kCall]) == 0) || (strcmp(token, lexical_keyword_parsing[kConst]) == 0) || (strcmp(token, lexical_keyword_parsing[kVar]) == 0) || (strcmp(token, lexical_keyword_parsing[kProcedure]) == 0) || (strcmp(token, lexical_keyword_parsing[kProgram]) == 0) || (strcmp(token, lexical_keyword_parsing[kElse]) == 0) || (strcmp(token, lexical_keyword_parsing[kFor]) == 0) || (strcmp(token, lexical_keyword_parsing[kTo]) == 0) || (strcmp(token, lexical_keyword_parsing[kTo]) == 0))

#define CHECK_MATH_OPER(cur) ((cur == '+') || (cur == '-') || (cur == '*') || (cur == '/') || (cur == '%'))
#define BEGIN_COMPARE_OPER(cur) ((cur == '!') || (cur == '>') || (cur == '<') || (cur == '='))
#define BEGIN_SPECIAL_CHARACTER(cur) ((cur == '(') || (cur == ')') || (cur == '.') || (cur == ':') || (cur == '?') || (cur == ';') || (cur == ',') || (cur == '[') || (cur == ']'))
#define BEGIN_CHARACTER(cur) (cur == '\'')
#define END_CHARACTER(cur) (cur == '\'')
#define BEGIN_STRING(cur) (cur == '\"')
#define END_STRING(cur) (cur == '\"')
#define BEGIN_NUMBER(cur) ((cur >= '0') && (cur <= '9'))
#define CONDITION_NUMBER(prev, cur) (((cur >= '0') && (cur <= '9')) || (cur == '.') || (cur == 'e') || (cur == 'E') || (((prev == 'e') || (prev == 'E')) && ((cur == '-') || (cur == '+'))))


//Define some data types here

typedef enum lexical_keyword lexical_keyword;
typedef struct token_str token_str;
typedef enum lexical_status lexical_status;
//Extern some external variable here

#include "ErrorHandler.h"

extern int current_error;

//Report for all lexical keywords in PL0 translating program

enum lexical_keyword {
	kBegin, kEnd, kIf, kThen, kWhile, kDo, kCall, kConst, kVar, kProcedure, kProgram, kElse, kFor, kTo, 
	kAdd, kSubtract, kMultiply, kDivide, kModule, kEqualCpr, kNotEqualCpr, kGreaterCpr, kLowerCpr, kGreaterEqualCpr, kLowerEqualCpr, 
	kLParentheses, kRParentheses, kDot, kTwoDot, kQues, kSemicolon, kCommas, kLSquareParentheses, kRSquareParentheses, kCharIden, kStringIden,
	kAssign, kIdentifier, vInteger, vReal, vChar, vString
};

//char* lexical_keyword_parsing[42];

enum lexical_status {
	normalStatus, commentFirstStatus, commentSecondStatus, compareStatus, mathOperStatus, wordStatus, 
	specialStatus, numValueStatus, charValueStatus, stringValueStatus
};

//Report for some structs

struct token_str {
	int _line;
	int _column;
	int _index;
	int _error;
	char* _value;
};

void vInitializeTokenTable(token_str *table, int iMaxElements);
int iConstructLexicalTable(char *fileName, token_str *table, int iMaxElements);
int iGetIndex(char *token);
int iComputeNumberLength(char *tokenNumber, int iLength);
int iGetNumToken(token_str *table);
int iPrintTokenTable(token_str *table);
int iFilePrintTokenTable(char *fileName, token_str *table);
token_str* iNextToken(token_str *table);
void vLexicalAnalysis(char *fileName, token_str *table, int iMaxElements);

#endif 
