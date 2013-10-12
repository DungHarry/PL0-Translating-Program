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
int semantic_error;

//Define all of types are declared there

typedef enum lexical_error lexical_error;
typedef enum syntax_error syntax_error; 
typedef enum semantic_errors semantic_errrors;
typedef enum internal_error internal_error;
typedef struct smt_error smt_error;
typedef enum type_smt_data type_smt_data;
//Report sime extended struct here

#define MAX_SMT_ERRORS 2000

struct smt_error {				//This is struct hold the information of an error occurred in semantic analysing
	int _line;				//Line error occurred
	int _column;				//Column error occurred
	int _type;				//Type of data error occurred
	char *_name;				//Name of data error occurred
	int _error;				//Error identifier
};

enum type_smt_data {				//Enum for type of semantic data
	smtVariable,				//Variabe type
	smtArray,				//Array type
	smtParameterValue,			//Parameter as value type
	smtParameterReference,			//Parameter as reference type
	smtConst,				//Constant type
	smtProcedure				//Procedure type
};
//Part handling for interal errors or errors are occurred in processing, not related to all processing progress

enum internal_error {
	LEXICAL_NULL_PARAMETER_CONSTRUCT = 70,			//The null node was passed into construct function for lexical
	LEXICAL_COULDNT_OPEN_FILE_SOURCE_CODE,			//Couldn't open source code file for lexical analysis
	LEXICAL_NULL_TOKEN_TABLE_INITIALIZE,			//The null node was passed into initialize function for lexical
	LEXICAL_TOO_MUCH_TOKEN,					//Have too much token in PL0 source code
	LEXICAL_INVALID_INPUT_ANALYSIS,				//Invalid input for lexical analysing
	SEMANTIC_TOO_MUCH_ERRORS,				//Too much errors in semantic analysing
	SEMANTIC_NON_NULL_POINTER_CREATE_NEW_NODE,		//None null pointer was passed to the create new node function in semantic analysing
	SEMANTIC_COULDNT_CREATE_NEW_NODE_SMT_PROCEDURE,		//Couldn't create new node for smt_procedure data structure in semantic analysing
	SEMANTIC_COULDNT_CREATE_NEW_NODE_PROCEDURE_PARAS,	//Couldn't create new node for procedure_paras in semantic analysing
	SEMANTIC_NULL_NODE_INSERT_PROCEDURE_PARAS,		//Null node was passed to iInsertProcedurePara function for semantic analysing
	SEMANTIC_NULL_NODE_PRINT_PROCEDURE_PARAS,		//Null procedure_paras node was passed to the iPrintProcedurePara function for semantic analysing
	SEMANTIC_NULL_NODE_FILE_PRINT_PROCEDURE_PARAS,		//Null procedure_paras node was passed to the iFilePrintProcedurePara function for semantic analysing
	SEMANTIC_NULL_FILE_POINTER_FILE_PRINT_PROCEDURE_PARAS,	//Null file pointer was passed to the iFilePrintProcedurePara function for semantic analysing
	SEMANTIC_NULL_NODE_INSERT_SMT_PROCEDURE,		//Null node was passed to iInsertSmtProcedure function for semantic analysing
	SEMANTIC_NOT_FOUND_SEARCH_SMT_PROCEDURE,		//The node wanted to search not found in the iSearchSmtProcedure for semantic analysing
	SEMANTIC_NULL_NODE_PRINT_SMT_PROCEDURE,			//The null node was passed to the iPrintSmtProcedure for semantic analysing
	SEMANTIC_NULL_NODE_FILE_PRINT_SMT_PROCEDURE,		//The null node was passed to the iFilePrintSmtProcedure for semantic analysing
	SEMANTIC_INVALID_FILE_NAME_FILE_PRINT_SMT_PROCEDURE,	//Null or empty file name was passed to the iFilePrintSmtProcedure for semantic analysing
	SEMANTIC_NULL_NODE_GET_SUM_SMT_PROCEDURE,		//Null smt_procedure node was passed to the iGetSumSmtProcedure function for semantic analysing
	SEMANTIC_NULL_NODE_GET_SUM_PROCEDURE_PARAS,		//Null procedure_paras node was passed to the iGetSumProcedurePara function for semantic analysing
	SEMANTIC_COULDNT_CREATE_NEW_FILE_PRINT_SMT_PROCEDURE,	//Couldn't create new file in iFilePrintSmtProcedure function for semantic analysing
	SEMANTIC_NULL_NODE_SEARCH_PROCEDURE_PARAS,		//Null procedure_paras node in iSearchProcedurePara function for semantic analysing
	SEMANTIC_NULL_NODES_PASSED_ANALYSIS,			//Some null nodes were passed to the iAnalysisSemantic function for semantic analysing
	SEMANTIC_NULL_NODE_INCREASE_STATUS,			//Null node was passed to the iIncreaseStatusAUnit function for semantic analysising
	SEMANTIC_NULL_NODE_DECREASE_STATUS,			//Null node was passed to the iDecreaseStatusAUnit function for semantic analysising
	SEMANTIC_NOT_FOUND_KEYWORD_PROGRAM_AT_FIRST,		//The keyword program not found at the first
	SEMANTIC_INVALID_AFTER_VAR_DECLARATION,			//Invalid keyword after var declaration
	SEMANTIC_INVALID_AFTER_PROGRAM_DECLARATION,		//Invalid keyword after var declaration
	SEMANTIC_INVALID_DATA_TYPE_CONST_DECLARATION,		//Invalid data type in const declaration
	SEMANTIC_INVALID_AFTER_CONST_DECLARATION,		//Invalid keyword after constant declaration
	SEMANTIC_NULL_NODE_DELETE_SUB_SMT_PROCEDURE,		//The null smt_procedure node was passed to the iDeleteSubSmtProcedure function for semantic analysing
	SEMANTIC_NULL_NODE_INCREASE_SMT_PROCEDURE,		//The null node was passed to the iIncreaseStatusSmtProcedure function for semantic analysing
	SEMANTIC_NULL_NODE_DECREASE_SMT_PROCEDURE,		//The null node was passed to the iDecreaseStatusSmtProcedure function for semantic analysing
	SEMANTIC_INVALID_AFTER_PROCEDURE_DECLARATION,		//The invalid after procedure declaration
	SEMANTIC_NULL_SMT_PROCEDURE_UPDATE_TYPE,		//The null node in the iUpdateValueType function for semantic analysing
	SEMANTIC_NULL_NODE_PRINT_SMT_TABLE,			//The null smt_data node was passed to the iPrintSmtTable function for semantic analysing
	SEMANTIC_NULL_NODE_FILE_PRINT_SMT_TABLE,		//The null smt_data node was passed to the iFilePrintSmtTable function for semantic analysing
	SEMANTIC_INVALID_FILE_NAME_FILE_PRINT_SMT_TABLE,	//Invalid fileName parameter was passed to the iFilePrintSmtTable function for semantic analysing
	SEMANTIC_COULDNT_CREATE_NEW_FILE_PRINT_SMT_TABLE	//Couldn't create new file in the iFilePrintSmtTable function for semantic analysing
};

/*--------------------------------------------------------------Part handling errors for lexical analysis--------------------------------------*/

enum lexical_error {
	LEXICAL_SUCCESS,					//Not having error
	LEXICAL_MAX_SIZE_NOT_ENOUGH,				//The max size of token table is not enought for all lexical in the PL0 source code
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

void vPrintLexicalErrors(token_str *tokenTable, int iNumElements);
/*----------------------------------------------------------Part handing errors for syntax analysis----------------------------------------*/

int iLineSyntaxError;
int iColumnSyntaxError;

enum syntax_error {
	SYNTAX_SUCCESS,						//Success syntax
	SYNTAX_LOST_FACTOR,					//End lost factor element
	SYNTAX_INVALID_FACTOR_TYPE,				//Invalid factor type
	SYNTAX_LOST_TOKEN_AFTER_FACTOR,				//Lost token after factor
	SYNTAX_LOST_COMPARISION_OPER,				//Lost comparision operators
	SYNTAX_LOST_R_EXPRESSION_IN_CONDITION,			//Lost right expression in condition process
	SYNTAX_LOST_WORD_AT_BEGIN_STATEMENT,			//Lost word at begin of statement
	SYNTAX_LOST_L_SQUARE_BRACKET_ASSGIN_STATEMENT,		//Lost left square bracket or assign operator in statement processing
	SYNTAX_INVALID_AT_BEGIN_INDEX_ARRAY,			//Invalid at begin of array
	SYNTAX_LOST_R_SQUARE_BRACKET,				//Lost right square bracket
	SYNTAX_LOST_ASSIGN_OPERATOR,				//Lost assign operator
	SYNTAX_INVALID_AT_BEGIN_EXPRESSION,			//Begining of expression must be number or identifier
	SYNTAX_LOST_R_PARENTHESES,				//Lost ) 
	SYNTAX_LOST_EXPRESSION_AFTER_BEGIN_KEYWORD,		//Lost expression after begin keyword
	SYNTAX_EXCESS_SEMICOLON_BEFORE_END_BEGIN_KEYWORK,	//Excess ; before end keyword in begin keyword
	SYNTAX_LOST_END_KEYWORD_STATEMENT,			//Lost end keyword in statement
	SYNTAX_LOST_IDENTIFIER_CALL,				//Invalid identifier after call keyword
	SYNTAX_INVALID_WORD_CALL,				//Invalid word in call procedure
	SYNTAX_LOST_CONDITION_IF,				//Lost condition after keyword if
	SYNTAX_LOST_KEYWORD_THEN_IF,				//Lost keyword then in if statement
	SYNTAX_LOST_STATEMENT_IF,				//Lost statement in if statement
	SYNTAX_LOST_CONDITION_WHILE,				//Lost condition after while keyword
	SYNTAX_LOST_KEYWORD_DO_WHILE,				//Lost keyword do in while statement
	SYNTAX_LOST_STATEMENT_WHILE,				//Lost statement in while statement
	SYNTAX_LOST_IDENTIFIER_FOR,				//Lost identifier in for loop
	SYNTAX_LOST_ASSIGN_OPERATOR_FOR,			//Lost assign operator in for loop
	SYNTAX_LOST_STATEMENT_FOR,				//Lost statement in for loop
	SYNTAX_LOST_KEYWORD_TO_FOR,				//Lost keyword to in for statement
	SYNTAX_LOST_KEYWORD_DO_FOR,				//Lost keyword do in for statement
	SYNTAX_INVALID_BEGIN_STATEMENT,				//Invalid at begin of statement
	SYNTAX_INVALID_BEGIN_BLOCK,				//Invalid at begin of block
	SYNTAX_INVALID_IDENTIFIER_CONST,			//Invalid identifier after keyword const
	SYNTAX_LOST_EQUAL_CONST,				//Lost = in const definition
	SYNTAX_INVALID_VALUE_CONST,				//Invalid const value in constant definition
	SYNTAX_INVALID_END_CONST,				//Invalid at the end of constant definition
	SYNTAX_INVALID_THE_END_CONST,				//Invalid next word of constant definition
	SYNTAX_INVALID_IDENTIFIER_VAR,				//Invalid indentifier after keyword var
	SYNTAX_INVALID_AFTER_IDENTIFIER_VAR,			//Invalid token after identifier in var declaration
	SYNTAX_INVALID_INTEGER_INDEX_VAR,			//Invalid integer index in var declaration
	SYNTAX_LOST_R_SQUARE_PARENTHESES_VAR,			//Lost ] for array in var declaration
	SYNTAX_INVALID_NEXT_VAR,				//Invalid next in var declaration
	SYNTAX_INVALID_THE_END_VAR,				//Invalid at the end of var declaration
	SYNTAX_LOST_IDENTIFIER_AFTER_KEYWORD_PROCEDURE,		//Lost identifier after keyword procedure
	SYNTAX_INVALID_TOKEN_AFTER_IDENTIFIER_PROCEDURE,	//Lost ; or ( in procedure declaration
	SYNTAX_INVALID_BEGIN_PARAMETER_PROCEDURE,		//Lost var or identifier in procedure declaration
	SYNTAX_LOST_IDENTIFIER_PARAMETER_PROCEDURE,		//Lost identifier parameter in procedure declaration
	SYNTAX_INVALID_NEXT_PARAMETER_PROCEDURE,		//Lost , or ) token
	SYNTAX_LOST_SEMICOLON_PROCEDURE,			//Lost ; in procedure declaration
	SYNTAX_LOST_STATEMENT_PROCEDURE,			//Lost statement in procedure declaration
	SYNTAX_INVALID_THE_END_PROCEDURE,			//Invalid at the end in procedure declaration
	SYNTAX_INVALID_NEXT_BEGIN,				//Invaid next token after keyword begin
	SYNTAX_INVALID_AFTER_STATEMENT_BEGIN,			//Invalid token after statement in keyword begin
	SYNTAX_LOST_END_BEGIN,					//Lost keyword end in keyword begin
	SYNTAX_INVALID_THE_END_BEGIN,				//Invalid at the end in begin block
	SYNTAX_LOST_KEYWORD_PROGRAM_MAIN,			//Lost keyword program in main analysis
	SYNTAX_LOST_IDENTIFIER_MAIN,				//Lost program identifier in main analysis
	SYNTAX_LOST_SEMICOLON_MAIN,				//Lost ; in main analysis
	SYNTAX_INVALID_TOKEN_MAIN,				//Invalid token after program declaration main analysis
	SYNTAX_LOST_DOT_MAIN,					//Lost . character main analysis
	SYNTAX_LOST_KEYWORD_ASSIGN_OPERATOR			//Lost [ or := in assign expression
};

void vPrintSyntaxErrors();

/*-------------------------------------------------Part handing errors for semantic analysing---------------------------------------------*/

enum semantic_errors {
	SEMANTIC_SUCCESS,					//Semantic successfully
	SEMANTIC_FAILURE,					//Semantic failure at global
	SEMANTIC_E_CHANGE_VALUE_CONST,				//Try to change value of constant
	SEMANTIC_E_IDEN_NOT_DECLARATION,			//The identifier wasn't declared
	SEMANTIC_E_IDEN_LOST_INDEX_ELEMENT_ARRAY,		//Lost index of array's element
	SEMANTIC_E_DUPLICATE_IDEN_IDEN_DECLARE,			//Duplicate identifier with identifier, const or parameter
	SEMANTIC_E_DUPLICATE_IDEN_PROCEDURE_DECLARATION,	//Duplicate identifier with procedure declaration
	SEMANTIC_E_DUPLICATE_CONST_IDEN_DECLARATION,		//Duplicate const with identifier, const or parameter
	SEMANTIC_E_DUPLICATE_CONST_PROCEDURE_DECLARATION,	//Duplicate identifier with procedure declaration
	SEMANTIC_E_DUPLICATE_PROCEDURE_IDEN_DECLARATION,	//Duplicate procedure with identifier declaration
	SEMANTIC_E_DUPLICATE_PROCEDURE_PROCEDURE_DECLARATION,	//Duplicate procedure with procedure declaration
	SEMANTIC_E_DIVIDE_CREATE_REAL_VALUE_INDEX_ARRAY,	//Devide create real index of array's element
	SEMANTIC_E_REAL_VALUE_IN_INDEX_ARRAY,			//The real value existed in index of array's element
	SEMANTIC_E_NOT_DECLARATION_V_INDEX_ARRAY,		//The variable wasn't declared in index array
	SEMANTIC_E_REAL_V_INDEX_ARRAY,				//The real variable, const or parameter in the index of array's element
	SEMANTIC_E_CHARACTER_V_INDEX_ARRAY,			//The variable, const or parameter has type of character in the index of array's element
	SEMANTIC_E_STRING_V_INDEX_ARRAY,			//The variable, const or parameter has type of string in the index of array's element
	SEMANTIC_E_NOT_DECLARATION,				//The variable, const or parameter wasn't declared before
	SEMANTIC_E_INVALID_R_SIDE_ASSIGN,			//Invalid the right side of assign expression
	SEMANTIC_E_NOT_DECLARATION_VARIABLE_FOR,		//The variable wasn't declared in condition of for loop
	SEMANTIC_E_NOT_DECLARATION_VARIABLE_R_SIGN_FOR,		//The variable wasn't declared in the right side initialized value
	SEMANTIC_E_NOT_DECLARATION_VARIABLE_IF,			//The variable in the condition of if statement wasn't declared
	SEMANTIC_E_NOT_DECLARATION_VARIABLE_WHILE,		//The variable in the condition of while statement wasn't declared
	SEMANTIC_E_NOT_DECLARATION_PROCEDURE_CALL,		//The procedure's name wasn't declared in the call procedure
	SEMANTIC_E_LOST_PARAMETER_LIST_PROCEDURE_CALL,		//Lost parameters in the call procedure
	SEMANTIC_E_EXCESS_PARAMETER_LIST_PROCEDURE_CALL,	//Excess parameter in the call procedure
	SEMANTIC_E_NOT_DECLARATION_PARAMETER_CALL,		//The parameter wasn't declaration in the call procedure
	SEMANTIC_E_INVALID_REFERENCE_PARAMETER_CALL,		//Invalid reference parameter in the call procedure
	SEMANTIC_E_NOT_MATCH_TYPE_REFERENCE_PARAMETER_CALL,	//The type of parameter didn't match with type of reference parameter in procedure defination
	SEMANTIC_NOT_MACTCH_TYPE_VALUE_PARAMETER_CALL,		//The type of parameter didn't match with type of value parameter in procedure defination
	SEMANTIC_E_INVALID_VALUE_PARAMETER_CALL,		//Invalid value parameter in the call procedure
	SEMANTIC_E_INVALID_NUM_PARAMETER_CALL			//Invalid number of parameters in the call procedure
};

void vPrintSemanticErrors(smt_error *smtErrorTable);

int print_error();

#endif
