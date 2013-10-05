/*
	Author: Lý Việt Dũng - Hanoi University of Science and Technology
	Date created: 3rd, October 2013
	Operating System: Ubuntu 13.04 Desktop x86_64
	Compiler: GNU compiler 4.7.3
	Program: PL0 translate program
	Module: Semantic analysis

	Description: this is source code file for semantic analysis process
*/

#include "SemanticAns.h"
//Declare for all extern variables here

extern token_str tokenTable[MAX_TOKEN];
//Initialize all vallue of variables are declared in the SemanticAns.h header

topStack = 0;
statusProgram = statusProcedure = statusBegin = 0;
head_procedure = tail_procedure = NULL;
/*---------------------------------------------------------Report details for all functions are declared in the SemanticAns.h-----------------------*/

void vInitialize() {
	table_smt_data smtTable[MAX_SMT_ELEMENTS];
	smt_error smtError[MAX_SMT_ERRORS];
	token_expand tokenSemantic[MAX_SEMANTIC_ELEMENTS];
	int i;

	for(i = 0; i < MAX_SMT_ELEMENTS; i ++) {
		(smtTable + i)->_status = END_SEMANTIC;
	}

	for(i = 0; i < MAX_SMT_ERRORS; i ++) {
		(smtError + i)->_error = END_SEMANTIC;
	}

	for(i = 0; i < MAX_TOKEN_EXPAND; i ++) {
		(tokenSemantic + i)->_index = END_SEMANTIC;
	}
}

int iAnalysisSemantic(token_str *tokenTable) {
	
}

int vPushSmtTable(smt_data *smtTable, token_expand *tokenExpandTable, smt_data smtElement) {
	
}

int vPopSmtTable(smt_data *smtTable) {
	
}

int iSearchSmtTable(char *idenName) {
	iSearch
}

int iSearchValueType(token_expand *tokenExpandTable, char *idenValue) {
	int i = 0;

	while(((tokenExpandTable + i)->_index != END_SEMANTIC) && (i < MAX_TOKEN_EXPAND)) {
		if(strcmp(idenValue, (tokenTable + (tokenExpandTable + i)->_index)->_value)) {
			return (tokenExpandTable + i)->_t_value;
		}
	}

	return VALUE_TYPE_SEARCH_NOT_FOUND;
}

int iUpdateValueType(token_expand *tokenExpandTable, char *idenValue, int iNewValue) {
	int i = 0;

	while(((tokenExpandTable + i)->_index != END_SEMANTIC) && (i < MAX_TOKEN_EXPAND)) {
		if(strcmp(idenValue, (tokenTable + (tokenExpandTable + i)->_index)->_value)) {
			(tokenExpandTable + i)->_t_value = iNewValue;

			return iNewValue;
		}
	}

	return UPDATE_VALUE_TYPE_FAILURE;

}

int iInserSmtError(smt_error *smtErrorTable, smt_error *errorElement) {
	static int index = 0;

	if(index >= MAX_SMT_ERRORS) {
		current_error = SEMANTIC_TOO_MUCH_ERRORS;

		print_error();
	} else {
		(smtErrorTable + i)->_line = errorElement->_line;
		(smtErrorTable + i)->_column = errorElement->_column;
		(smtErrorTable + i)->_type = errorElement->_type;
		(smtErrorTable + i)->_error = errorElement->_error;

		(smtErrorTable + i)->name = malloc(sizeof(char) * (strlen(errorElement->_name) + 2));

		strcmp((smtErrorTable + i)->_name, errorElement->_name);

		current_error = SEMANTIC_SUCCESS;
	}

	return current_error;
}
//Report details all functions for procedure parameters

int iInsertProcedurePara(procedure_paras **head, procedure_paras **tail, int order, int type) {
	procedure_paras *tmp;

}

int iPrintProcedurePara(procedure_paras **head, procedure_paras **tail) {

}

int iFilePrintProcedurePara(FILE *file, procedure_paras **head, procedure_paras **tail){

}
//Report details all functions for semantic procedure

int iInsertSmtProcedure(smt_procedure **head, smt_procedure **tail, char *name) {

}

int iSearchSmtProcedure(smt_procedure **head, smt_procedure **tail, smt_procedure **result, char *name) {

}

int iPrintSmtProcedure(smt_procedure **head, smt_procedure **tail) {

}

int iFilePrintSmtProcedure(char *fileName, smt_procedure **head, smt_procedure **tail) {

}
