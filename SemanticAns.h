/*
	Author: Lý Việt Dũng - Hanoi University of Science and Technology
	Date created: 3rd, October 2013
	Operating System: Ubuntu 13.04 Desktop x86_64
	Compiler: GNU compiler 4.7.3
	Program: PL0 translate program
	Module: Semantic analysis

	Description: this is header file for semantic analysis process
*/

#ifndef _SEMANTIC_ANS_H_
#define _SEMANTIC_ANS_H_

#include "SyntaxAns.h"
#include <stdio.h>
#include <string.h>
#include <stdio.h>
//Extern some variable here

extern token_str tokenTable[MAX_TOKEN];
extern int current_error;
extern int semantic_error;
//Define some needed value

#define END_SEMANTIC -1
#define MAX_TOKEN_EXPAND 2000
#define MAX_SMT_ELEMENTS 3000
#define MAX_SMT_ERRORS 2000

#define void* LPVOID
#define TYPE_SMT_PROCEDURE 0
#define TYPE_PROCEDURE_PARAS 1

#define SMT_SEARCH_NAME_FOUND 1
#define SMT_SEARCH_NAME_NOT_FOUND 0

#define VALUE_TYPE_SEARCH_NOT_FOUND -1
#define UPDATE_VALUE_TYPE_FAILURE -1

#define VALUE_DICCUSSED_STATUS 0
#define VALUE_INTIALIZE_STATUS 1
#define OUT_STATUS_SMT_PROCEDURE -1
//Define some data type here

typedef struct smt_error smt_error;
typedef struct table_smt_data table_smt_data;
typedef struct token_expand token_expand;
typedef enum status_semantic status_semantic;
typedef enum value_smt_type value_smt_type;
typedef struct procedure_paras procedure_paras;
typedef struct smt_procedure smt_procedure;
typedef enum status_common status_common;
typedef unsigned int u_int;

smt_data smtTable[MAX_SMT_ELEMENTS];
smt_error smtError[MAX_SMT_ERRORS];
token_expand tokenExpand[MAX_SEMANTIC_ELEMENTS];
smt_procedure *head_procedure, *tail_procedure;

int topStack;
int statusProgram, statusProcedure, statusBegin, statusCommon;
int semanticStatus;
//Define some statuses and switch statuses here

			//For statuses
#define BEGIN_END_PROGRAM(stProgram, stProcedure, stBegin) ((stProgram == 0) && (stProcedure == 0) && (stBegin == 0))
#define READ_PROGRAM_NAME(stProgram, stProcedure, stBegin, stCommon) ((stProgram == 1) && (stProcedure == 0) && (stBegin == 0) && (stCommon == cmmProgram))
#define READ_VAR(stProgram, stProcedure, stBegin, stCommon) ((stProgram == 1) && (stProcedure >= 0) && (stBegin == 0) && (stCommon == cmmVar))
#define READ_CONST(stProgram, stProcedure, stBegin, stCommon) ((stProgram == 1) && (stProcedure >=0) && (stBegin == 0) && (stCommon == cmmConst))
#define READ_NAME_PROCEDURE(stProgram, stProcedure, stBegin, stCommon) ((stProgram == 1) && (stProcedure >= 1) && (stBegin == 0) && (stCommon == cmmNameProcedure))
#define READ_STATEMENT(stProgram, stProcedure, stBegin, stCommon) ((stProgram == 1) && (stProcedure >= 1) && (stBegin >= 1) && (stCommon == stStatement))
#define READ_STATEMENT_IF(value) (value == cmmIf)
#define READ_ASSIGN(value) (value == cmmAssign)
#define READ_STATEMENT_WHILE(value) (value == cmmWhile)
#define READ_STATEMENT_CALL(value) (value == cmmCall)
#define READ_STATEMENT_FOR(value) (value == cmmFor)
#define END_PROCEDURE(stProgram, stProcedure, stBegin, stCommon) ((stProgram == 1) && () )

#define NEXT_IDEN_PROGRAM(index) ((index == kVar) || (index == kConst) || (index == kProcedure) || (index == kBegin))
#define AFTER_VAR(index) ((index == kVar) || (index == kConst) || (index == kProcedure) || (index == kBegin))
#define AFTER_CONST(index) ((index == kVar) || (index == kConst) || (index == kProcedure) || (index == kBegin))
#define NEXT_VALUE_CONST(index) ((index == vInteger) || (index == vReal) || (index == vChar) || (index == vString))
#define AFTER_NAME_PROCEDURE(index) ((index == kVar) || (index == kConst) || (index == kProcedure) || (index == kBegin))
#define END_STATEMENT(index) ((index == kSemicolon) || (index == kEnd))
//Report for some structs hold information about variable, parameter and procedure here

struct smt_error {				//This is struct hold the information of an error occurred in semantic analysing
	int _line;				//Line error occurred
	int _column;				//Column error occurred
	int _type;				//Type of data error occurred
	char *_name;				//Name of data error occurred
	int _error;				//Error identifier
};

struct smt_data {				//Struct holds the information of all variables, parameters, constants are declared in source code
	int _line;				//The line the data was declared
	int _column;				//The column the data was declared
	int _index;				//Index in token lexical
	char *_name;				//Name of data
	int _type_value;			//Type of value: integer, real, char, string
	int _type;				//Type of data, it can be: variable, constant, parameter as value, paramerter as reference
	int _status;				//Status of data, this data is kept until the value of this member different with 0
};

enum status_semantic {				//Status for common status
	cmmConst,				//Common constant
	cmmVar,					//Common variable
	cmmParameter,				//Common parameter
	cmmIf,					//Common if
	cmmAssign,				//Common assign
	cmmWhile,				//Common while
	cmmCall,				//Common call
	cmmFor,					//Common for
	cmmEnd,
	cmmProgram,				//Common program
	cmmProcedure,				//Common procedure
	cmmNameProcedure,			//Common name procedure
	stStatement				//Status statement
};

enum sub_status {				//Sub status when analysis var, const, statement
	varArray,				//Var array
	varNormal				//Var normal
};

enum type_smt_data {				//Enum for type of semantic data
	smtVariable,				//Variabe type
	smtArray,				//Array type
	smtParameterValue,			//Parameter as value type
	smtParameterReference,			//Parameter as reference type
	smtConst,				//Constant type
	smtProcedure				//Procedure type
};

enum value_smt_type {				//Enum for all types of semantic value
	smtInteger,				//Integer type
	smtReal,				//Real type
	smtChar,				//Character type
	smtStr					//String type
};

struct token_expand {				//The struct holds all information of lexical token, if token is identifier then set type of value
	int _index;				//The index defines the lexical token			
	int _t_value;				//Type of value: it can be integer, real, character or string
};

struct procedure_paras {			//The struct holds the information of parameters, contains: order and type of each parameter
	int _order;				//Order of parameter from left to right side
	char *_name;				//Parameter name
	int _type;				//Type of parameter, can be: passed by value or passed by reference
	int _type_value;
	procedure_paras *_ptr;			//The procedure_paras pointer
};

struct smt_procedure {				//The struct holds information of all procedure in PL0 source code
	char *_name;				//Name of procedure
	int _status;
	procedure_paras *_head_paras;		//The pointer to the head of parameters list
	procedure_paras *_tail_paras;		//The pointer to the tail of parameters list
	smt_procedure *_ptr;			//The smt_procedure pointer 
};
/*------------------------------------------Declare all function in the semantic analysing---------------------------------------------------------*/

void vInitialize(smt_data *smtTable, smt_error *smtError, token_expand *tokenExpand);
int iAnalysisSemantic(token_str *tokenTable, smt_data *smtTable, smt_error *smtError, token_expand *tokenExpand);
int vPushSmtTable(smt_data *smtTable, token_expand *tokenExpandTable, smt_data smtElement);
int vPopSmtTable(smt_data *smtTable);
int iSearchSmtTable(smt_data *smtTable, smt_data **result, char *idenName);
int iSearchValueType(token_expand *tokenExpandTable, char *idenValue);
int iUpdateValueType(token_expand *tokenExpandTable, char *idenValue, int iNewValue);
int iInserSmtError(smt_error *smtErrorTable, smt_error *errorElement);
int iIncreaseStatusAUnit(smt_data *smtTable);
int iDecreaseStatusAUnit(smt_data *smtTable);
int iSetSemanticStatus(int newStProgram, int newStProcedure, int newStBegin, int newStCommon);
int iAddSemanticError(int line, int column, int type, char *name, int error, int index);
//Declare some functions for procedure parameters

int iCreateNewNode(LPVOID* node, int typeNode);

int iInsertProcedurePara(procedure_paras **head, procedure_paras **tail, procedure_paras **node, char *name, int order, int _type_value, int type);
int iSearchProcedurePara(procedure_paras **head, procedure_paras **tail, procedure_paras **result, char *name);
int iDeleteProcedurePara(procedure_paras **head, procedure_paras **tail);
int iGetSumProcedurePara(procedure_paras **head, procedure_paras **tail);
int iPrintProcedurePara(procedure_paras **head, procedure_paras **tail);
int iFilePrintProcedurePara(FILE *file, procedure_paras **head, procedure_paras **tail);
//Declare some functions for semantic procedure

int iInsertSmtProcedure(smt_procedure **head, smt_procedure **tail, smt_procedure **node, char *name);
int iDeleteSmtProcedureStatus(smt_procedure **head, smt_procedure **tail, int status);
int iDeleteSubSmtProcedure(smt_procedure **head, smt_procedure **tail);						//Sub procedures have _status = -1
int iGetSumSmtProcedure(smt_procedure **head, smt_procedure **tail);
int iSearchSmtProcedure(smt_procedure **head, smt_procedure **tail, smt_procedure **result, char *name);
int iPrintSmtProcedure(smt_procedure **head, smt_procedure **tail);
int iFilePrintSmtProcedure(char *fileName, smt_procedure **head, smt_procedure **tail);
int iIncreaseStatusSmtProcedure(smt_procedure **head, smt_procedure **tail);				//Increase all _status of member 1 unit
int iDecreaseStatusSmtProcedure(smt_procedure **head, smt_procedure **tail);				//Decrease all _status of member 1 unit

#endif
