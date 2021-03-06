/*
	Author: Lý Việt Dũng - Hanoi University of Science and Technology
	Date created: 3rd, October 2013
	Operating System: Ubuntu 13.04 Desktop x86_64
	Compiler: GNU compiler 4.7.3
	Program: PL0 translate program
	Module: Syntax analysis

	Description: this is header file for syntax analysis process
*/

#ifndef _SYNTAX_ANS_H_
#define _SYNTAX_ANS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "LexicalAns.h"

typedef token_str* p_token_str;

//Extern some variables here

extern token_str tokenTable[MAX_TOKEN];
extern char* lexical_keyword_parsing[MAX_LEXICAL_KEYWORD];
extern int iLineSyntaxError;
extern int iColumnSyntaxError;
//Report all define for each type of analysis: factor, term, expression, condition, statement, block, syntax

#define IS_FACTOR(type) ((type == kIdentifier) || (type == vInteger) || (type == vReal) || (type == kLParentheses))
#define NEXT_TERM(type) ((type == kMultiply) || (type == kDivide) || (type == kModule))
#define NEXT_EXPRESSION(type) ((type == kAdd) || (type == kSubtract))
#define IS_CONDITION_OPERATOR(type) ((type == kEqualCpr) || (type == kNotEqualCpr) || (type == kGreaterCpr) || (type == kLowerCpr) || (type == kGreaterEqualCpr) || (type == kLowerEqualCpr))
#define NEXT_ASSIGN(type) ((type == kLSquareParentheses) || (type == kAssign))
#define BEGIN_INDEX_ARRAY(type) ((type == kIdentifier) || (type == vInteger) || (type == vReal) || (type == kLParentheses))
#define BEGIN_EXPRESSION(type) ((type == kIdentifier) || (type == vInteger) || (type == vReal) || (type == kLParentheses))
#define BEGIN_BLOCK(type) ((type == kConst) || (type == kVar) || (type == kProcedure) || (type == kBegin))
#define VALUE_CONST(type) ((type == vInteger) || (type == vReal) || (type == vChar) || (type == vString))
#define NEXT_CONST(type) ((type == kCommas) || (type == kSemicolon))
#define NEXT_VAR(type) ((type == kCommas) || (type == kSemicolon) || (type == kLSquareParentheses))
#define NEXT_VAR_SIMPLE(type) ((type == kCommas) || (type == kSemicolon))
#define NEXT_PROCEDURE(type) ((type == kSemicolon) || (type == kLParentheses))
#define BEGIN_PARAMETER_PROCEDURE(type) ((type == kVar) || (type == kIdentifier))
#define NEXT_PARAMETER_PRODURE(type) ((type == kRParentheses) || (type == kSemicolon))
#define NEXT_BEGIN(type) ((type == kEnd) || (type == kSemicolon))
//Report all variables here

token_str *lpToken;
//Define all syntax processing functions here

int iProcessFactor(token_str *table);
int iProcessTerm(token_str *table);
int iProcessExpression(token_str *table);
int iProcessCondition(token_str *table);
int iProcessStatement(token_str *table);
int iProcessBlock(token_str *table);
int iProccessSyntax(token_str *table);

#endif
