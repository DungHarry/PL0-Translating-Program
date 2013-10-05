/*
	Author: Lý Việt Dũng - Hanoi University of Science and Technology
	Date created: 2nd, October 2013
	Operating System: Ubuntu 13.04 Desktop x86_64
	Compiler: GNU compiler 4.7.3
	Program: PL0 translate program
	Module: Error handler

	Description: this is header file for all error handling in the translate program
*/

#ifndef _ERROR_HANDLER_H_
#define _ERROR_HANDLER_H_

#include <stdio.h>
#include "LexicalAns.h"

//Define all variable in this header here

#define SUCCESS_ANALYSIS 1
#define FAILURE_ANALYSIS 0

int current_error;
int status_lexical_error;

//Define all of types are declared there

typedef enum lexical_error lexical_error;
/*--------------------------------------------------------------Part handling errors for lexical analysis--------------------------------------*/

enum lexical_error {
	LEXICAL_SUCCESS,					//Not having error
	LEXICAL_MAX_SIZE_NOT_ENOUGH,				//The max size of token table is not enought for all lexical in the PL0 source code
	LEXICAL_NULL_TOKEN_TABLE_INITIALIZE,			//The null node was passed into initialize function for lexical
	LEXICAL_NULL_PARAMETER_CONSTRUCT,			//The null node was passed into construct function for lexical
	LEXICAL_COULDNT_OPEN_FILE_SOURCE_CODE,			//Couldn't open source code file for lexical analysis
	LEXICAL_TOO_MUCH_TOKEN,					//Have too much token in PL0 source code
	LEXICAL_INVALID_IDENTIFIER,				//Invalid begin of identifier
	LEXICAL_INVALID_TOO_LONG_IDENTIFIER,			//Invalid identifier too long length
	LEXICAL_INVALID_MATH_OPER,				//Invalid math operator
	LEXICAL_INVALID_COMPARE_OPER,				//Invalid compare operator
	LEXICAL_INVALID_SPECIAL_OPER,				//Invalid special operator
	LEXICAL_INVALID_CHARACTER,				//Invalid character
	LEXICAL_TOO_MUCH_E,					//Too much e or E character
	LEXICAL_TOO_MUCH_DOT,					//Too much . character
	LEXICAL_TOO_BIG_VALUE,					//The value too big
	LEXICAL_LOST_VALUE_AFTER_E				//Lost value after e or E character
};

void vPrintLexicalErrors(token_struct *tokenTable, int iNumElements);
int print_error();

#endif
