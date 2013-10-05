/*
	Author: Lý Việt Dũng - Hanoi University of Science and Technology
	Date created: 2nd, October 2013
	Operating System: Ubuntu 13.04 Desktop x86_64
	Compiler: GNU compiler 4.7.3
	Program: PL0 translate program
	Module: Error handler

	Description: this is source code file for all error handling in the translate program
*/

#include "ErrorHandler.h"

status_lexical_error = SUCCESS_ANALYSIS;

void vPrintLexicalErrors(token_struct *tokenTable, int iNumElements) {
	int i;

	for(i = 0; (i < iNumElements) && ((tokenTable + i)->_line != END_LEXICAL_TABLE); i ++) {
		if((tokenTable + i)->_error != LEXICAL_SUCCESS) {
			status_lexical_error = FAILURE_ANALYSIS;

			printf("**Lexical error %d:%d: ", (tokenTable + i)->_line, (tokenTable + i)->_column);

			switch((tokenTable + i)->_error) {
				case LEXICAL_INVALID_IDENTIFIER: printf("Invalid identifier\n");
					break;

				case LEXICAL_INVALID_TOO_LONG_IDENTIFIER: printf("Too long identifier\n");
					break;

				case LEXICAL_INVALID_MATH_OPER: printf("Invalid math operator\n");
					break;

				case LEXICAL_INVALID_COMPARE_OPER: printf("Invalid comparison operator\n");
					break;

				case LEXICAL_INVALID_SPECIAL_OPER: printf("Invalid special operator\n");
					break;

				case LEXICAL_INVALID_CHARACTER: printf("Invalid character\n");
					break;

				case LEXICAL_TOO_MUCH_E: printf("Too much e or E character in real value\n");
					break;

				case LEXICAL_TOO_MUCH_DOT: printf("Too much . character in real value\n");
					break;

				case LEXICAL_TOO_BIG_VALUE: printf("The value of number too big, greater than 99999\n");
					break;

				case LEXICAL_LOST_VALUE_AFTER_E: printf("Lost value after e or E character in real value\n");
					break;

				default: printf("Unknown error\n");
					break;
			}
		}
	}
}

int print_error() {
	printf("**Intenal error: ");

	switch(current_error) {
		case LEXICAL_NULL_PARAMETER_CONSTRUCT: printf("The null file name or null token table was passed to the construct function in elexical analysis\n");
			break;

		case LEXICAL_COULDNT_OPEN_FILE_SOURCE_CODE: printf("Couldn't open PL0 source code file in elexical analysis\n");
			break;

		case LEXICAL_NULL_TOKEN_TABLE_INITIALIZE: printf("The null token table node was passed into initialize function for elexical analysis\n");
			break;

		case LEXICAL_TOO_MUCH_TOKEN: printf("Have too much(>= %d) token in the PL0 source code\n", MAX_TOKEN);
			break;

		default: printf("Unknown error\n");
			break;
	}

	return current_error;
}
