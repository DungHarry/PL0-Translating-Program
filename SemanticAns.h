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
extern iLineSemanticError = 1;
extern iColumnSemanticError = 1;
//Define some needed value

#define END_SEMANTIC -1
#define MAX_TOKEN_EXPAND 2000
#define MAX_SMT_ELEMENTS 3000
#define MAX_SMT_ERRORS 2000

#define SMT_SEARCH_NAME_FOUND 1
#define SMT_SEARCH_NAME_NOT_FOUND 0

#define VALUE_TYPE_SEARCH_NOT_FOUND -1
#define UPDATE_VALUE_TYPE_FAILURE -1
//Define some data type here

typedef struct smt_error smt_error;
typedef struct table_smt_data table_smt_data;
typedef struct token_expand token_expand;
typedef enum status_smt_data status_smt_data;
typedef enum value_smt_type value_smt_type;
typedef struct procedure_paras procedure_paras;
typedef struct smt_procedure smt_procedure;

smt_data smtTable[MAX_SMT_ELEMENTS];
smt_error smtError[MAX_SMT_ERRORS];
token_expand tokenSemantic[MAX_TOKEN_EXPAND];
smt_procedure *head_procedure, *tail_procedure;

int topStack;
int statusProgram, statusProcedure, statusBegin;
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
	char *_name;				//Name of data
	int _type_value;			//Type of value: integer, real, char, string
	int _type;				//Type of data, it can be: variable, constant, parameter as value, paramerter as reference
	int _status;				//Status of data, this data is kept until the value of this member different with 0
};

enum status_smt_data {				//Enum for status of semantic data
	smtotConsidering,			//Not considering
	considering,				//Considering
	considered				//Considered
};

enum type_smt_data {				//Enum for type of semantic data
	smtVariable,				//Variabe type
	smtArray,				//Array type
	smtParameterValue,			//Parameter as value type
	smtParameterReference,			//Parameter as reference type
	smtConstant,				//Constant type
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

struct procedure_paras {
	int _order;
	int _type;
	procedure_paras *_next;
};

struct smt_procedure {
	char *_name;
	procedure_paras *_head_paras;
	procedure_paras *_tail_paras;
	smt_procedure *_next;
};
/*------------------------------------------Declare all function in the semantic analysing---------------------------------------------------------*/

void vInitialize();
int iAnalysisSemantic(token_str *tokenTable);
int vPushSmtTable(smt_data *smtTable, token_expand *tokenExpandTable, smt_data smtElement);
int vPopSmtTable(smt_data *smtTable);
int iSearchSmtTable(char *idenName);
int iSearchValueType(token_expand *tokenExpandTable, char *idenValue);
int iUpdateValueType(token_expand *tokenExpandTable, char *idenValue, int iNewValue);
int iInserSmtError(smt_error *smtErrorTable, smt_error *errorElement);
//Declare some functions for procedure parameters

int iInsertProcedurePara(procedure_paras **head, procedure_paras **tail, int order, int type);
int iPrintProcedurePara(procedure_paras **head, procedure_paras **tail);
int iFilePrintProcedurePara(FILE *file, procedure_paras **head, procedure_paras **tail);
//Declare some functions for semantic procedure

int iInsertSmtProcedure(smt_procedure **head, smt_procedure **tail, char *name);
int iSearchSmtProcedure(smt_procedure **head, smt_procedure **tail, smt_procedure **result, char *name);
int iPrintSmtProcedure(smt_procedure **head, smt_procedure **tail);
int iFilePrintSmtProcedure(char *fileName, smt_procedure **head, smt_procedure **tail);

#endif
