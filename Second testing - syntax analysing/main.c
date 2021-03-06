/*
	Author: Lý Việt Dũng - Hanoi University of Science and Technology
	Date created: 2nd, October 2013
	Operating System: Ubuntu 13.04 Desktop x86_64
	Compiler: GNU compiler 4.7.3
	Program: PL0 translate program
	Module: Testing lexical analysis

	Description: this is testing source code for lexical analysis process
*/

#include "LexicalAns.h"

extern token_str tokenTable[MAX_TOKEN];
extern int status_lexical_error;

void main(int argc, char *argv[]) {
	if(argc == 2) {
		assert((argv[1] != "") && (argv[1] != NULL) && (tokenTable != NULL));

		vInitializeTokenTable(tokenTable, MAX_TOKEN);
		iConstructLexicalTable(argv[1], tokenTable, MAX_TOKEN);

		iFilePrintTokenTable("ResultLexical.txt", tokenTable);

		vPrintLexicalErrors(tokenTable, MAX_TOKEN);

		if(status_lexical_error == SUCCESS_ANALYSIS) {
			printf("Analysis successfully\n");
		}

		iProccessSyntax(tokenTable);
	} else {
		printf("Usage: program code.pl0\n");
	} 
}
