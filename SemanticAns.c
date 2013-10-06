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
extern int current_error;
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

int iCreateNewNode(LPVOID* node, int typeNode) {
	if(*node == NULL) {
		switch(typeNode) {
			case TYPE_SMT_PROCEDURE: {
				if((*node = (LPVOID) malloc(sizeof(smt_procedure))) != NULL) {
					((smt_procedure *) (*node))->_name = NULL;
					((smt_procedure *) (*node))->_head_paras = NULL;
					((smt_procedure *) (*node))->_tail_paras = NULL;
					((smt_procedure *) (*node))->_ptr = NULL;

					current_error = SEMANTIC_SUCCESS;
				} else { 
					current_error = SEMANTIC_COULDNT_CREATE_NEW_NODE_SMT_PROCEDURE;
				}
			} break;

			case TYPE_PROCEDURE_PARAS: {
				if((*node = (LPVOID) malloc(sizeof(procedure_paras))) != NULL) {
					((procedure_paras *) (*node))->_order = -1;
					((procedure_paras *) (*node))->_type = -1;
					((procedure_paras *) (*node))->_ptr = NULL;

					current_error = SEMANTIC_SUCCESS;
				} else {
					current_error = SEMANTIC_COULDNT_CREATE_NEW_NODE_PROCEDURE_PARAS;
				}
			} break;

			default: {

			} break;
		}
	} else {
		current_error = SEMANTIC_NON_NULL_POINTER_CREATE_NEW_NODE;
	}

	*node = NULL;

	return current_error;
}

int iInsertProcedurePara(procedure_paras **head, procedure_paras **tail, procedure_paras **node, int order, int type) {
	procedure_paras *tmp;

	if(*node != NULL) {
		(*node)->_order = order;
		(*node)->_type = type;

		if((*head == NULL) && (*tail == NULL)) {
			*head = *tail = *node;

			(*head)->_ptr = NULL;
		} else if(*tail == *head) {
			*tail = *node;

			(*tail)->_ptr = (procedure_paras *) ((u_int) (*head) ^ (u_int) (NULL));
			(*head)->_ptr = (procedure_paras *) ((u_int) (*tail) ^ (u_int) (NULL));
		} else {
			tmp = *tail;
			*tail = *node;
			tmp->_ptr = (procedure_paras *) ((u_int) (tmp->_ptr) ^ (u_int) (*tail));
			(*tail)->_ptr = (procedure_paras *) ((u_int) (tmp) ^ (u_int) (NULL));
		}

		current_error = SEMANTIC_SUCCESS;
	} else {
		current_error = SEMANTIC_NULL_NODE_INSERT_PROCEDURE_PARAS;

		print_error();
	}

	return current_error;
}

int iGetSumProcedurePara(procedure_paras **head, procedure_paras **tail) {
	procedure_paras *crt, *prv, *tmp;
	int iCount = 0;

	if(*head != NULL) {
		for(crt = *head, prv = NULL; crt != NULL; tmp = crt, crt = (procedure_paras *) ((u_int) (prv) ^ (u_int) (crt->_ptr)), prv = tmp) {
			iCount ++;
		}

		current_error = SEMANTIC_SUCCESS;
	} else {
		current_error = SEMANTIC_NULL_NODE_GET_SUM_PROCEDURE_PARAS;
	}

	return iCount;
}

int iPrintProcedurePara(procedure_paras **head, procedure_paras **tail) {
	procedure_paras *crt, *prv, *tmp;

	if(*head != NULL) {
		printf("Number parameters: %d\n", iGetSumProcedurePara(head, tail));

		for(crt = *head, prv = NULL; crt != NULL; tmp = crt, crt = (procedure_paras *) ((u_int) (prev->_ptr) ^ (u_int) (crt)), prev = tmp) {
			printf("\nOrder: %d\n", crt->_order);
			printf("\nType: ");

			switch(crt->_type) {
				case smtParameterValue: printf("Parameter passed by value\n");
					break;

				case smtParameterReference: printf("Parameter passed by reference\n");
					break;

				default: printf("Unknow type of parameter\n");
					break;
			}
		}

		printf("\n");

		current_error = SEMANTIC_SUCCESS;
	} else {
		current_error = SEMANTIC_NULL_NODE_PRINT_PROCEDURE_PARAS;

		print_error();
	}

	return current_error;
}

int iFilePrintProcedurePara(FILE *file, procedure_paras **head, procedure_paras **tail){
	procedure_paras *crt, *prv, *tmp;

	if(*head != NULL) {
		if(file != NULL) {
			fprintf(file, "Number parameters: %d\n", iGetSumProcedurePara(head, tail));

			for(crt = *head, prv = NULL; crt != NULL; tmp = crt, crt = (procedure_paras *) ((u_int) (prev->_ptr) ^ (u_int) (crt)), prev = tmp) {
				fprintf(file, "\tOrder: %d\n", crt->_order);
				fprintf(file, "\tType: ");

				switch(crt->_type) {
					case smtParameterValue: fprintf(file, "Parameter passed by value\n");
						break;

					case smtParameterReference: fprintf(file, "Parameter passed by reference\n");
						break;

					default: fprintf(file, "Unknow type of parameter\n");
						break;
				}
			}

			fprintf(file, "\n");

			current_error = SEMANTIC_SUCCESS;
		} else {
			current_error = SEMANTIC_NULL_FILE_POINTER_FILE_PRINT_PROCEDURE_PARAS;

			print_error();
		}
	} else {
		current_error = SEMANTIC_NULL_NODE_PRINT_PROCEDURE_PARAS;

		print_error();
	}

	return current_error;
}
//Report details all functions for semantic procedure

int iInsertSmtProcedure(smt_procedure **head, smt_procedure **tail, smt_procedure **node, char *name) {
	smt_procedure *tmp;

	if(*node != NULL) {
		(*node)->_name = name;

		if((*head == NULL) && (*tail == NULL)) {
			*head = *tail = *node;

			(*head)->_ptr = NULL;
		} else if(*head == *tail) {
			*tail = *node;

			(*tail)->_ptr = (smt_procedure *) ((u_int) (*head) ^ (u_int) (NULL));
			(*head)->_ptr = (smt_procedure *) ((u_int) (*tail) ^ (u_int) (NULL));
		} else {
			tmp = *tail;
			*tail = *node;

			tmp->_ptr = (smt_procedure *) ((u_int) (tmp->_ptr) ^ (u_int) (*tail));
			(*tail)->_ptr = (smt_procedure *) ((u_int) (tmp) ^ (u_int) (NULL));
		}

		current_error = SEMANTIC_SUCCESS;
	} else {
		current_error = SEMANTIC_NULL_NODE_INSERT_SMT_PROCEDURE;

		print_error();
	}

	return current_error;
}

int iGetSumSmtProcedure(smt_procedure **head, smt_procedure **tail) {
	smt_procedure *crt, *prv, *tmp;
	int iCount = 0;

	if(*head != NULL) {
		for(crt = *head, prv = NULL; crt != NULL; tmp = crt, crt = (smt_procedure *) ((u_int) (crt->_ptr) ^ (u_int) (prv)), prv = tmp) {
			iCount ++;
		}

		current_error = SEMANTIC_SUCESS;
	} else {
		current_error = SEMANTIC_NULL_NODE_GET_SUM_SMT_PROCEDURE;

		print_error();
	}

	return iCount;
}

int iSearchSmtProcedure(smt_procedure **head, smt_procedure **tail, smt_procedure **result, char *name) {
	smt_procedure *crt, *prv, *tmp;

	for(crt = *head, prv = NULL; crt != NULL; tmp = crt, crt = (smt_procedure *) ((u_int) (prv) ^ (u_int) (crt->_ptr)), prv = tmp) {
		if(strcmp(crt->_name, name) == 0) {
			*result = crt;

			return (current_error = SEMANTIC_SUCCESS);
		}
	}

	*result = NULL;

	return (current_error = SEMANTIC_NOT_FOUND_SEARCH_SMT_PROCEDURE);
}

int iPrintSmtProcedure(smt_procedure **head, smt_procedure **tail) {
	smt_procedure *crt, *prv, *tmp;
	int i = 1;

	if(smt_procedure != NULL) {
		printf("Number of procedure: %d\n", iGetSumSmtProcedure(head, tail));

		for(crt = *head, prv = NULL; crt != NULL; tmp = crt, crt = (smt_procedure *) ((u_int) (crt->_ptr) ^ (u_int) (prv)), prv = tmp, i ++) {
			printf("%d. Name: %s\n", i, crt->_name);

			if((crt->_head_paras != NULL) && (crt->_tail_paras != NULL)) {
				iPrintProcedurePara(crt->_head_paras, crt->_head_paras);
			} else {
				printf("Emtpy parameters\n");

				printf("\n");
			}
		}

		current_error = SEMANTIC_SUCCESS;
	} else {
		current_error = SEMANTIC_NULL_NODE_PRINT_SMT_PROCEDURE;

		print_erorr();
	}

	return current_error;
}

int iFilePrintSmtProcedure(char *fileName, smt_procedure **head, smt_procedure **tail) {
	smt_procedure *crt, *prv, *tmp;
	FILE *file;
	int i = 1;

	if(smt_procedure != NULL) {
		if((fileName != NULL) && (strcmp(fileName, "") != 0)) {
			if((file = fopen(fileName, "w")) != NULL) {
				printf("Number of procedure: %d\n", iGetSumSmtProcedure(head, tail));

				for(crt = *head, prv = NULL; crt != NULL; tmp = crt, crt = (smt_procedure *) ((u_int) (crt->_ptr) ^ (u_int) (prv)), prv = tmp, i ++) {
					printf("%d. Name: %s\n", i, crt->_name);

					if((crt->_head_paras != NULL) && (crt->_tail_paras != NULL)) {
						iPrintProcedurePara(crt->_head_paras, crt->_head_paras);
					} else {
						printf("Emtpy parameters\n");

						printf("\n");
					}
				}

				fclose(file);

				current_error = SEMANTIC_SUCCESS;
			} else {
				current_error = SEMANTIC_COULDNT_CREATE_NEW_FILE_PRINT_SMT_PROCEDURE;

				print_error();
			}
		} else {
			current_error = SEMANTIC_INVALID_FILE_NAME_FILE_PRINT_SMT_PROCEDURE;

			print_error();
		}
	} else {
		current_error = SEMANTIC_NULL_NODE_PRINT_SMT_PROCEDURE;

		print_error();
	}

	return current_error;
}
