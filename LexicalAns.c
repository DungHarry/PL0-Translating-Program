/*
	Author: Lý Việt Dũng - Hanoi University of Science and Technology
	Date created: 2nd, October 2013	Operating System: Ubuntu 13.04 Desktop x86_64
	Compiler: GNU compiler 4.7.3
	Program: PL0 translate program
	Module: Lexical analysis

	Description: this is source code file for lexical analysis process
*/

#include "LexicalAns.h"

token_str tokenTable[MAX_TOKEN];

char* lexical_keyword_parsing[MAX_LEXICAL_KEYWORD] = {
							[kBegin] = "begin", [kEnd] = "end", [kIf] = "if", [kThen] = "then", [kWhile] = "while", 
							[kDo] = "do", [kCall] = "call", [kConst] = "const", [kVar] = "var", 
							[kProcedure] = "procedure", [kProgram] = "program", [kElse] = "else", [kFor] = "for", 
							[kTo] = "to", [kAdd] = "+", [kSubtract] = "-", [kMultiply] = "*", [kDivide] = "/", 
							[kModule] = "%", [kEqualCpr] = "=", [kNotEqualCpr] = "!=", [kGreaterCpr] = ">", 
							[kLowerCpr] = "<", [kGreaterEqualCpr] = ">=", [kLowerEqualCpr] = "<=", [kLParentheses] = "(", 
							[kRParentheses] = ")", [kDot] = ".", [kTwoDot] = ":", [kQues] = "?", [kSemicolon] = ";", 
							[kCommas] = ",", [kLSquareParentheses] = "[", [kCharIden] = "\'", [kStringIden] = "\"",
							[kRSquareParentheses] = "]", [kAssign] = ":=", [kIdentifier] = "Identifier", 
							[vInteger] = "Integer", [vReal] = "Real", [vChar] = "Char", [vString] = "String"
						};

void vInitializeTokenTable(token_str *table, int iMaxElements) {
	int i;

	if(table != NULL) {
		for(i = 0; i < iMaxElements; i ++) {
			(table + i)->_line = (table + i)->_column = (table + i)->_index = (table + i)->_error = END_LEXICAL_TABLE;
		}

		current_error = LEXICAL_SUCCESS;
	} else {
		current_error = LEXICAL_NULL_TOKEN_TABLE_INITIALIZE;
		print_error();
	}

	assert(current_error == LEXICAL_SUCCESS);
}

int iConstructLexicalTable(char *fileName, token_str *table, int iMaxElements) {
	FILE *file;
	int iStatus = normalStatus, iLine = 1, iColumn = 1, iPosition = 0, iLineSet, iColumnSet;
	char curChar, prevChar, tempChar;
	char temp[1000];

	if((fileName != NULL) && (table != NULL)) {
		if((file = fopen(fileName, "r")) != NULL) {
			for(prevChar = 1, curChar = getc(file); curChar != EOF; prevChar = curChar, curChar = getc(file), iColumn ++) {
				if(iPosition >= MAX_TOKEN) {
					current_error = LEXICAL_TOO_MUCH_TOKEN;

					print_error();

					exit(1);
				}

				switch(iStatus) {
					case normalStatus: {
						if(CHECK_FIRST_COMMENT_STATUS(prevChar, curChar)) {
							iStatus = commentFirstStatus;

							fseek(file, -1, SEEK_CUR);
							iColumn --;
						} else if(CHECK_SECOND_COMMENT_STATUS(prevChar, curChar)) {
							iStatus = commentSecondStatus;

							fseek(file, -1, SEEK_CUR);
							iColumn --;
						} else if(curChar == '/') {
							tempChar = getc(file);

							if((tempChar != '/') && (tempChar != '*')) {
								iStatus = mathOperStatus;

								fseek(file, -2, SEEK_CUR);
							} else {
								fseek(file, -1, SEEK_CUR);
							}

							iColumn --;
						} else if(BEGIN_WORD(curChar)) {
							iStatus = wordStatus;

							fseek(file, -1, SEEK_CUR);
							iColumn --;
						} else if(CHECK_MATH_OPER(curChar)) {
							iStatus = mathOperStatus;

							fseek(file, -1, SEEK_CUR);
							iColumn --;
						} else if(BEGIN_COMPARE_OPER(curChar)) {
							iStatus = compareStatus;

							fseek(file, -1, SEEK_CUR);
							iColumn --;
						} else if(BEGIN_SPECIAL_CHARACTER(curChar)) {
							iStatus = specialStatus;

							fseek(file, -1, SEEK_CUR);
							iColumn --;
						} else if(BEGIN_CHARACTER(curChar)) {
							iStatus = charValueStatus;
						} else if(BEGIN_STRING(curChar)) {
							iStatus = stringValueStatus;
						} else if(BEGIN_NUMBER(curChar)) {
							iStatus = numValueStatus;

							fseek(file, -1, SEEK_CUR);
							iColumn --;
						} else if(curChar == '\n') {
							iColumn = 0;
							iLine ++;
						} else if(curChar == '\t') {
							iColumn += 8;
						} else if(curChar == ' ') {
							
						} else {
							iStatus = 1000;

							fseek(file, -1, SEEK_CUR);
							iColumn --;
						}
					} break;

					case commentFirstStatus: {
						for(; !CHECK_END_FIRST_COMMENT(prevChar, curChar); prevChar = curChar, curChar = getc(file)) {
							if(curChar == '\n') {
								iLine ++;
								iColumn = 0;
							} else {
								iColumn ++;
							}
						}

						iStatus = normalStatus;
					} break;

					case commentSecondStatus: {
						for(;!CHECK_END_SECOND_COMMENT(curChar); prevChar = curChar, curChar = getc(file), iColumn ++);

						if(CHECK_END_SECOND_COMMENT(curChar)) {
							iLine ++;
							iColumn = 1;
						}

						iStatus = normalStatus;
					} break;

					case wordStatus: {
						int index;
						iLineSet = iLine;
						iColumnSet = iColumn;

						for(index = 0; CONDITION_WORD(curChar); prevChar = curChar, curChar = getc(file), index ++) {
							*(temp + index) = curChar;
							iColumn ++;
						}

						fseek(file, -1, SEEK_CUR);
						curChar = prevChar;
						iColumn --;

						*(temp + index) = '\0';

						(table + iPosition)->_line = iLineSet;
						(table + iPosition)->_column = iColumnSet;
						(table + iPosition)->_value = (char *) malloc((strlen(temp) + 2) * sizeof(char));
						strcpy((table + iPosition)->_value, temp);

						if(CHECK_KEYWORD(temp)) {
							(table + iPosition)->_index = iGetIndex(temp);
							(table + iPosition)->_error = LEXICAL_SUCCESS;
						} else {
							(table + iPosition)->_index = kIdentifier;

							if(strlen((table + iPosition)->_value) > MAX_IDEN_LENGTH) {
								(table + iPosition)->_error = LEXICAL_INVALID_TOO_LONG_IDENTIFIER;
							} else {
								(table + iPosition)->_error = LEXICAL_SUCCESS;
							}
						}

						iPosition ++;
						iStatus = normalStatus;
					} break;

					case mathOperStatus: {
						(table + iPosition)->_line = iLine;
						(table + iPosition)->_column = iColumn;
						(table + iPosition)->_value = (char *) malloc(2 * sizeof(char));
						sprintf((table + iPosition)->_value, "%c", curChar);
						(table + iPosition)->_index = iGetIndex((table + iPosition)->_value);

						if((table + iPosition)->_index != INDEX_NOT_FOUND) {
							(table + iPosition)->_error = LEXICAL_SUCCESS;
						} else {
							(table + iPosition)->_error = LEXICAL_INVALID_MATH_OPER;
						}

						iPosition ++;
						iStatus = normalStatus;
					} break;

					case compareStatus: {
						iLineSet = iLine;
						iColumnSet = iColumn;
						tempChar = getc(file);
						iColumn ++;

						(table + iPosition)->_line = iLineSet;
						(table + iPosition)->_column = iColumnSet;
						(table + iPosition)->_value = (char *) malloc(4 * sizeof(char));
						*((table + iPosition)->_value + 0) = curChar;

						if(BEGIN_COMPARE_OPER(curChar)) {
							if(curChar == '!') {
								if(tempChar == '=') {
									*((table + iPosition)->_value + 1) = tempChar;
									*((table + iPosition)->_value + 2) = '\0';
								} else {
									*((table + iPosition)->_value + 1) = '\0';
									fseek(file, -1, SEEK_CUR);
									curChar = prevChar;
									iColumn --;
								}
							} else {
								if(tempChar == '=') {
									*((table + iPosition)->_value + 1) = tempChar;
									*((table + iPosition)->_value + 2) = '\0';
								} else {
									*((table + iPosition)->_value + 1) = '\0';
									fseek(file, -1, SEEK_CUR);
									curChar = prevChar;
									iColumn --;
								}
							}
						} else {
							*((table + iPosition)->_value + 1) = '\0';
							fseek(file, -1, SEEK_CUR);
							curChar = prevChar;
							iColumn --;
						}

						(table + iPosition)->_index = iGetIndex((table + iPosition)->_value);

						if((table + iPosition)->_index != INDEX_NOT_FOUND) {
							(table + iPosition)->_error = LEXICAL_SUCCESS;
						} else {
							(table + iPosition)->_error = LEXICAL_INVALID_COMPARE_OPER;
						}

						iPosition ++;
						iStatus = normalStatus;
					} break;

					case specialStatus: {
						(table + iPosition)->_line = iLine;
						(table + iPosition)->_column = iColumn;
						(table + iPosition)->_value = (char *) malloc(4 * sizeof(char));
						*((table + iPosition)->_value + 0) = curChar;
						*((table + iPosition)->_value + 1) = '\0';

						if(curChar == ':') {
							tempChar = getc(file);

							if(tempChar == '=') {
								*((table + iPosition)->_value + 1) = tempChar;
								*((table + iPosition)->_value + 2) = '\0';
								prevChar = curChar;
								curChar = tempChar;

								iColumn ++;
							} else {
								fseek(file, -1, SEEK_CUR);
							}
						}

						(table + iPosition)->_index = iGetIndex((table + iPosition)->_value);

						if((table + iPosition)->_index != INDEX_NOT_FOUND) {
							(table + iPosition)->_error = LEXICAL_SUCCESS;
						} else {
							(table + iPosition)->_error = LEXICAL_INVALID_SPECIAL_OPER;
						}

						iPosition ++;
						iStatus = normalStatus;
					} break;

					case charValueStatus: {
						int index = 0;
						iLineSet = iLine;
						iColumnSet = iColumn - 1;

						for(; !END_CHARACTER(curChar); prevChar = curChar, curChar = getc(file), iColumn ++, index ++) {
							temp[index] = curChar;
						}

						temp[index] = '\0';

						(table + iPosition)->_line = iLineSet;
						(table + iPosition)->_column = iColumnSet;
						(table + iPosition)->_value = (char *) malloc((strlen(temp) + 2) * sizeof(char));
						strcpy((table + iPosition)->_value, temp);
						(table + iPosition)->_index = vChar;

						if(strlen((table + iPosition)->_value) == 1) {
							(table + iPosition)->_error = LEXICAL_SUCCESS;
						} else {
							(table + iPosition)->_error = LEXICAL_INVALID_CHARACTER;
						}

						iPosition ++;
						iStatus = normalStatus;
					} break;

					case stringValueStatus: {
						int index;
						iLineSet = iLine;
						iColumnSet = iColumn - 1;

						for(index = 0; !END_STRING(curChar); prevChar = curChar, curChar = getc(file), index ++) {
							temp[index] = curChar;

							if(curChar == '\n') {
								iLine ++;
								iColumn = 0;
							} else {
								iColumn ++;
							}
						}

						temp[index] = '\0';

						(table + iPosition)->_line = iLineSet;
						(table + iPosition)->_column = iColumnSet;
						(table + iPosition)->_value = (char *) malloc((strlen(temp) + 2) * sizeof(char));
						strcpy((table + iPosition)->_value, temp);
						(table + iPosition)->_index = vString;

						(table + iPosition)->_error = LEXICAL_SUCCESS;

						iPosition ++;
						iStatus = normalStatus;
					} break;

					case numValueStatus: {
						int index;
						int iNumE = 0;
						int iNumDot = 0;
						iLineSet = iLine;
						iColumnSet = iColumn;

						for(index = 0; CONDITION_NUMBER(prevChar, curChar); prevChar = curChar, curChar = getc(file), index ++, iColumn ++) {
							temp[index] = curChar;

							if((curChar == 'e') || (curChar == 'E')) {
								iNumE ++;
							} else if(curChar == '.') {
								iNumDot ++;
							}
						}

						temp[index] = '\0';

						(table + iPosition)->_line = iLineSet;
						(table + iPosition)->_column = iColumnSet;
						(table + iPosition)->_value = (char *) malloc((strlen(temp) + 2) * sizeof(char));
						strcpy((table + iPosition)->_value, temp);

						if((iNumE > 0) || (iNumDot > 0)) {
							(table + iPosition)->_index = vReal;

							if(iNumDot > 1) {
								(table + iPosition)->_error = LEXICAL_TOO_MUCH_DOT;
							} else if(iNumE > 1) {
								(table + iPosition)->_error = LEXICAL_TOO_MUCH_E;
							} else if((prevChar == '-') || (prevChar == '+') || (prevChar == 'e') || (prevChar == 'E')) {
								(table + iPosition)->_error = LEXICAL_LOST_VALUE_AFTER_E;
							} else if(iComputeNumberLength((table + iPosition)->_value, strlen((table + iPosition)->_value)) > MAX_NUMBER_LENGTH) {
								(table + iPosition)->_error = LEXICAL_TOO_BIG_VALUE;
							} else {
								(table + iPosition)->_error = LEXICAL_SUCCESS;
							}
						} else {
							(table + iPosition)->_index = vInteger;

							if(iComputeNumberLength((table + iPosition)->_value, strlen((table + iPosition)->_value)) > MAX_NUMBER_LENGTH) {
								(table + iPosition)->_error = LEXICAL_TOO_BIG_VALUE;
							} else {
								(table + iPosition)->_error = LEXICAL_SUCCESS;
							}
						}

						iPosition ++;
						iStatus = normalStatus;
						fseek(file, -1, SEEK_CUR);
						curChar = prevChar;
						iColumn --;
					} break;

					default: {
						int index = 0;
						iLineSet = iLine;
						iColumnSet = iColumn;

						*(temp + index) = curChar;
						prevChar = curChar; 
						curChar = getc(file);
						iColumn ++;

						for(index = 1; CONDITION_WORD(curChar); prevChar = curChar, curChar = getc(file), index ++, iColumn ++) {
							*(temp + index) = curChar;
						}

						fseek(file, -1, SEEK_CUR);
						curChar = prevChar;
						iColumn --;

						*(temp + index) = '\0';

						(table + iPosition)->_line = iLineSet;
						(table + iPosition)->_column = iColumnSet;
						(table + iPosition)->_value = (char *) malloc((strlen(temp) + 2) * sizeof(char));
						strcpy((table + iPosition)->_value, temp);
						(table + iPosition)->_error = LEXICAL_INVALID_IDENTIFIER;

						iPosition ++;
						iStatus = normalStatus;
					}	break;
				}
			}
			
			fclose(file);	
		} else {
			current_error = LEXICAL_COULDNT_OPEN_FILE_SOURCE_CODE;

			print_error();
		}
	} else {
		current_error = LEXICAL_NULL_PARAMETER_CONSTRUCT;

		print_error();
	}

	assert(current_error == LEXICAL_SUCCESS);

	return current_error;
}

int iGetIndex(char *token) {
	int i = 0;

	for(i = 0; i < MAX_LEXICAL_KEYWORD; i ++) {
		if(strcmp(token, lexical_keyword_parsing[i]) == 0) {
			return i;
		}
	}

	return INDEX_NOT_FOUND;
}

int iComputeNumberLength(char *tokenNumber, int iLength) {
	int i;

	for(i = 0; (*(tokenNumber + i) != '.') && (*(tokenNumber + i) != 'e') && (*(tokenNumber + i) != 'E') && (i < iLength); i ++);

	return i;
}

int iGetNumToken(token_str *table) {
	int i;

	for(i = 0; (i < MAX_TOKEN) && ((table + i)->_line != END_LEXICAL_TABLE); i ++);

	return (i - 1);
}

int iPrintTokenTable(token_str *table) {
	int i;

	if(table != NULL) {
		printf("Number of token is: %d\n\n", iGetNumToken(table));

		for(i = 0; (i < MAX_TOKEN) && ((table + i)->_line != END_LEXICAL_TABLE); i ++) {
			printf("%d. Line: %d\n", i + 1, (table + i)->_line);
			printf("    Column: %d\n", (table + i)->_column);
			printf("    Value follow index: %s\n", lexical_keyword_parsing[(table + i)->_index]);
			printf("    Stored value: %s\n", (table + i)->_value);
			printf("    Index error: %d\n\n", (table + i)->_error);
		}

		current_error = 0;
	} else {
		current_error = -1;
	}

	return current_error;
}

int iFilePrintTokenTable(char *fileName, token_str *table) {
	int i;
	FILE *file;

	if(table != NULL) {
		if((fileName != NULL) || (fileName != "")) {
			if((file = fopen(fileName, "w")) != NULL) {
				fprintf(file, "Number of token is: %d\n\n", iGetNumToken(table));

				for(i = 0; (i < MAX_TOKEN) && ((table + i)->_line != END_LEXICAL_TABLE); i ++) {
					fprintf(file, "%d. Line: %d\n", i + 1, (table + i)->_line);
					fprintf(file, "    Column: %d\n", (table + i)->_column);
					fprintf(file, "    Value follow index: %s\n", lexical_keyword_parsing[(table + i)->_index]);
					fprintf(file, "    Stored value: %s\n", (table + i)->_value);
					fprintf(file, "    Index error: %d\n\n", (table + i)->_error);
				}
				current_error = 0;

				fclose(file);
			} else {
				current_error = -3;
			}
		} else {
			current_error = -2;
		}
	} else {
		current_error = -1;
	}

	return current_error;
}

token_str* iNextToken(token_str *table) {
	static int i = 0;
	token_str *objectReturn;

	if(table != NULL) {
		if(iGetIndex(lexical_keyword_parsing[(table + i)->_index]) == INDEX_NOT_FOUND) {
			objectReturn = NULL;
		} else {
			objectReturn = table + i;
		}
	} else {
		objectReturn = NULL;
	}

	i ++;

	return objectReturn;
}

void vLexicalAnalysis(char *fileName, token_str *table, int iMaxElements) {
	if((fileName != NULL) && (strcmp(fileName, "") != 0) && (table != NULL)) {
		vInitializeTokenTable(table, iMaxElements);
		iConstructLexicalTable(fileName, table, iMaxElements);
	} else {
		current_error = LEXICAL_INVALID_INPUT_ANALYSIS;

		print_error();
	}
}
