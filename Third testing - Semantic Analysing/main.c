/*
	Author: Lý Việt Dũng - Hanoi University of Science and Technology
	Date created: 3rd, October 2013
	Operating System: Ubuntu 13.04 Desktop x86_64
	Compiler: GNU compiler 4.7.3
	Program: PL0 translate program
	Module: Semantic analysis

	Description: this is source code testing file for semantic analysis process
*/

#include "SemanticAns.h"

extern token_str tokenTable[MAX_TOKEN];
extern int status_lexical_error;
extern int semanticStatus;
extern smt_data smtTable[MAX_SMT_ELEMENTS];
extern smt_error smtError[MAX_SMT_ERRORS];
extern token_expand tokenExpand[MAX_SMT_ELEMENTS];

void main(int argc, char *argv[]) {
	if(argc == 2) {
		assert((argv[1] != "") && (argv[1] != NULL) && (tokenTable != NULL));

		vInitializeTokenTable(tokenTable, MAX_TOKEN);
		iConstructLexicalTable(argv[1], tokenTable, MAX_TOKEN);

		iFilePrintTokenTable("ResultLexical.txt", tokenTable);

		vPrintLexicalErrors(tokenTable, MAX_TOKEN);

		if(status_lexical_error == SUCCESS_ANALYSIS) {
			printf("Lexical analysing successfully!\n");
		}

		iProccessSyntax(tokenTable);

		iAnalysisSemantic(tokenTable, smtTable, smtError, tokenExpand);

		if(semanticStatus == SEMANTIC_SUCCESS) {
			printf("Semantic analysing successfully!\n");
		} else {
			vPrintSemanticErrors(smtError);
		}
	} else {
		printf("Usage: program code.pl0\n");
	} 
}
