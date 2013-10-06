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
int iLineSyntaxError = 1;
int iColumnSyntaxError = 1;

void vPrintLexicalErrors(token_str *tokenTable, int iNumElements) {
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

void vPrintSyntaxErrors() {
	printf("**Syntax error %d::%d: ", iLineSyntaxError, iColumnSyntaxError);

	switch(current_error) {
		case SYNTAX_LOST_FACTOR: printf("Lost factor\n");
			break;

		case SYNTAX_INVALID_FACTOR_TYPE: printf("Invalid factor\n");
			break;

		case SYNTAX_LOST_TOKEN_AFTER_FACTOR: printf("Lost token after factor\n");
			break;

		case SYNTAX_LOST_COMPARISION_OPER: printf("Lost comparision operator\n");
			break;

		case SYNTAX_LOST_R_EXPRESSION_IN_CONDITION: printf("Lost right expression in the condition processing\n");
			break;

		case SYNTAX_LOST_WORD_AT_BEGIN_STATEMENT: printf("Lost keyword or identifier at begin of statement\n");
			break;

		case SYNTAX_LOST_L_SQUARE_BRACKET_ASSGIN_STATEMENT: printf("Lost [ or := in statement processing\n");
			break;

		case SYNTAX_INVALID_AT_BEGIN_INDEX_ARRAY: printf("Invalid at begin of index array\n");
			break;

		case SYNTAX_LOST_R_SQUARE_BRACKET: printf("Lost ] in statement processing\n");
			break;

		case SYNTAX_LOST_ASSIGN_OPERATOR: printf("Lost assign operator :=\n");
			break;

		case SYNTAX_INVALID_AT_BEGIN_EXPRESSION: printf("Invalid at begin of expression\n");
			break;

		case SYNTAX_LOST_R_PARENTHESES: printf("Lost )\n");
			break;

		case SYNTAX_LOST_EXPRESSION_AFTER_BEGIN_KEYWORD: printf("Lost expression after keyword begin\n");
			break;

		case SYNTAX_LOST_END_KEYWORD_STATEMENT: printf("Lost keyword end in the statement\n");
			break;

		case SYNTAX_LOST_IDENTIFIER_CALL: printf("Lost identifier after keyword call\n");
			break;

		case SYNTAX_INVALID_WORD_CALL: printf("Invalid word in call procedure\n");
			break;

		case SYNTAX_LOST_CONDITION_IF: printf("Lost condition after keyword if\n");
			break;

		case SYNTAX_LOST_KEYWORD_THEN_IF: printf("Lost keyword then in if statement\n");
			break;

		case SYNTAX_LOST_STATEMENT_IF: printf("Lost statement in if statement\n");
			break;

		case SYNTAX_LOST_CONDITION_WHILE: printf("Lost condition after keyword while\n");
			break;

		case SYNTAX_LOST_KEYWORD_DO_WHILE: printf("Lost keyword do in while statement\n");
			break;

		case SYNTAX_LOST_STATEMENT_WHILE: printf("Lost statement in while statement\n");
			break;

		case SYNTAX_LOST_IDENTIFIER_FOR: printf("Lost identifier in for statement\n");
			break;

		case SYNTAX_LOST_ASSIGN_OPERATOR_FOR: printf("Lost := in for statement\n");
			break;

		case SYNTAX_LOST_STATEMENT_FOR: printf("Lost statement in the for loop\n");
			break;

		case SYNTAX_LOST_KEYWORD_TO_FOR: printf("Lost keyword to in the for statment\n");
			break;

		case SYNTAX_LOST_KEYWORD_DO_FOR: printf("Lost keyword do in the for statement\n");
			break;

		case SYNTAX_INVALID_BEGIN_STATEMENT: printf("Invalid at begin of statement\n");
			break;

		case SYNTAX_INVALID_BEGIN_BLOCK: printf("Invalid at begin of block\n");
			break;

		case SYNTAX_INVALID_IDENTIFIER_CONST: printf("Invalid the identifier after keyword const\n");
			break;

		case SYNTAX_LOST_EQUAL_CONST: printf("Lost = in constant definition\n");
			break;

		case SYNTAX_INVALID_VALUE_CONST: printf("Invalid constant value in const definition\n");
			break;

		case SYNTAX_INVALID_END_CONST: printf("Lost ; or , in constant definition\n");
			break;

		case SYNTAX_INVALID_THE_END_CONST: printf("Invalid the next token after constant definition\n");
			break;

		case SYNTAX_INVALID_IDENTIFIER_VAR: printf("Invalid identifier after keyword var\n");
			break;

		case SYNTAX_INVALID_AFTER_IDENTIFIER_VAR: printf("Lost , ; or [ after identifier in var declaration\n");
			break;

		case SYNTAX_INVALID_INTEGER_INDEX_VAR: printf("Invalid integer array index in var declaration\n");
			break;

		case SYNTAX_LOST_R_SQUARE_PARENTHESES_VAR: printf("Lost ] for array in var declaration\n");
			break;

		case SYNTAX_INVALID_NEXT_VAR: printf("Lost , or ; in var declaration\n");
			break;

		case SYNTAX_INVALID_THE_END_VAR: printf("Inalid at the end of var declaration\n");
			break;

		case SYNTAX_LOST_IDENTIFIER_AFTER_KEYWORD_PROCEDURE: printf("Lost identifier after keyword procedure\n");
			break;

		case SYNTAX_INVALID_TOKEN_AFTER_IDENTIFIER_PROCEDURE: printf("Lost ; or ( in procedure declaration\n");
			break;

		case SYNTAX_INVALID_BEGIN_PARAMETER_PROCEDURE: printf("Lost var or identifier in procedure declaration\n");
			break;

		case SYNTAX_LOST_IDENTIFIER_PARAMETER_PROCEDURE: printf("Lost identifier as parameter in procedure declaration\n");
			break;

		case SYNTAX_INVALID_NEXT_PARAMETER_PROCEDURE: printf("Lost character , or ) in procedure declaration\n");
			break;

		case SYNTAX_LOST_SEMICOLON_PROCEDURE: printf("Lost ; in procedure declaration\n");
			break;

		case SYNTAX_LOST_STATEMENT_PROCEDURE: printf("Lost statement in procedure declaration\n");
			break;

		case SYNTAX_INVALID_THE_END_PROCEDURE: printf("Invalid at the end of procedure declaration\n");
			break;

		case SYNTAX_INVALID_NEXT_BEGIN: printf("Invalid next token after keyword begin\n");
			break;

		case SYNTAX_INVALID_AFTER_STATEMENT_BEGIN: printf("Lost keyword end or ; in begin block\n");
			break;

		case SYNTAX_INVALID_THE_END_BEGIN: printf("Invalid at the end of begin block\n");
			break;

		case SYNTAX_LOST_KEYWORD_PROGRAM_MAIN: printf("Lost keyword program\n");
			break;

		case SYNTAX_LOST_IDENTIFIER_MAIN: printf("Lost program's identifier\n");
			break;

		case SYNTAX_LOST_SEMICOLON_MAIN: printf("Lost ; in main analysing\n");
			break;

		case SYNTAX_INVALID_TOKEN_MAIN: printf("Invalid token after program declaration\n");
			break;

		case SYNTAX_LOST_DOT_MAIN: printf("Lost character . in main analysing\n");
			break;

		case SYNTAX_LOST_END_BEGIN: printf("Lost keyword end\n");
			break;

		case SYNTAX_EXCESS_SEMICOLON_BEFORE_END_BEGIN_KEYWORK: printf("Excess ; before end keyword\n");
			break;

		case SYNTAX_LOST_KEYWORD_ASSIGN_OPERATOR: printf("Lost [ or := in assign operator\n");
			break;

		default: printf("Unknown error\n");
			break;
	}

	exit(1);
}

void vPrintSemanticErrors(smt_error *smtErrorTable) {
	int i;

	printf("\tHave some semantic errors occurred: \n\n");

	for(i = 0; i < MAX_SMT_ERRORS; i ++) {
		printf("**%d. Semantic error %d::%d at ", i + 1, (smtErrorTable + i)->_line, (smtErrorTable + i)->_column);

		switch((smtErrorTable + i)->_type) {
			case smtVariable: printf("variable ");
				break;

			case smtArray: printf("array ");
				break;

			case smtParameterValue: printf("parameter passed by value ");
				break;

			case smtParameterReference: printf("parameter passed by reference ");
				break;

			case smtConst: printf("constant ");
				break;

			case smtProcedure: printf("procedure ");
				break;

			default: printf("unknown ");
				break;
		}

		printf("'%s': ", (smtErrorTable + i)->_name);

		switch((smtErrorTable + i)->_error) {
			case SEMANTIC_SUCCESS: printf("Semantic successfully\n");
				break;

			default: printf("Unknown error\n");
				break;
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

		case LEXICAL_INVALID_INPUT_ANALYSIS: printf("Invalid input for lexical analysis process: the null, empty PL0 source code name or null token table\n");
			break;

		case SEMANTIC_TOO_MUCH_ERRORS: printf("Too much(more than %d) errors in semantic analysing\n", MAX_SMT_ERRORS);
			break;

		case SEMANTIC_NON_NULL_POINTER_CREATE_NEW_NODE: printf("None null pointer was passed to the create new node function in semantic analysing\n");
			break;

		case SEMANTIC_COULDNT_CREATE_NEW_NODE_SMT_PROCEDURE: printf("Couldn't create new node for smt_procedure data type in semantic analysing\n");
			break;

		case SEMANTIC_COULDNT_CREATE_NEW_NODE_PROCEDURE_PARAS: printf("Couldn't create new node for procedure_paras data type in semantic analysing\n");
			break;

		case SEMANTIC_NULL_NODE_INSERT_PROCEDURE_PARAS: printf("The null node was passed to the iInsertProcedurePara function in semantic analysing\n");
			break;

		case SEMANTIC_NULL_NODE_PRINT_PROCEDURE_PARAS: printf("The null node was passed to the iPrintProcedurePara function in semantic analysing\n");
			break;

		case SEMANTIC_NULL_NODE_FILE_PRINT_PROCEDURE_PARAS: printf("The null node was passed to the iFilePrintProcedurePara function in semantic analysing\n");
			break;

		case SEMANTIC_NULL_FILE_POINTER_FILE_PRINT_PROCEDURE_PARAS: printf("The null file pointer was passed to the iFilePrintProcedurePara function in semantic analysing\n");
			break;

		case SEMANTIC_NULL_NODE_INSERT_SMT_PROCEDURE: printf("The null node was passed to the iInsertSmtProcedure function in semantic analysing\n");
			break;

		case SEMANTIC_NOT_FOUND_SEARCH_SMT_PROCEDURE: printf("The node wanted to search not found in iSearchSmtProcedure for sematic analysing\n");
			break;

		case SEMANTIC_NULL_NODE_PRINT_SMT_PROCEDURE: printf("The null smt_procedure node was passed to the iPrintSmtProcedure function for semantic analysing\n");
			break;

		case SEMANTIC_NULL_NODE_FILE_PRINT_SMT_PROCEDURE: printf("The null smt_procedure node was passed to the iFilePrintSmtProcedure function for semantic analysing\n");
			break;

		case SEMANTIC_INVALID_FILE_NAME_FILE_PRINT_SMT_PROCEDURE: printf("The null or empty file name was passed to the iFilePrintSmtProcedure function for semantic analysing\n");
			break;

		case SEMANTIC_NULL_NODE_GET_SUM_SMT_PROCEDURE: printf("The null smt_procedure node was passed to the iGetSumSmtProcedure function for semantic analysing\n");
			break;

		case SEMANTIC_NULL_NODE_GET_SUM_PROCEDURE_PARAS: printf("The null procedure_paras node was passed to the iGetSumProcedurePara function for semantic analysing\n");
			break;

		case SEMANTIC_NULL_NODE_SEARCH_PROCEDURE_PARAS: printf("The null procedure_paras node was passed to the iSearchProcedurePara function for semantic analysing\n");
			break;

		case SEMANTIC_NULL_NODES_PASSED_ANALYSIS: printf("Some null nodes were passed to the iAnalysisSemantic function for semantic analysing\n");
			break;

		case SEMANTIC_NOT_FOUND_KEYWORD_PROGRAM_AT_FIRST: printf("The keyword program not found at the first of PL0 source code\n");
			break;

		default: printf("Unknown error\n");
			break;
	}

	return current_error;
}
