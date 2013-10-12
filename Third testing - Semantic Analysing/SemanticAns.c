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
extern int semantic_error;
//Initialize all vallue of variables are declared in the SemanticAns.h header

semanticStatus = SEMANTIC_SUCCESS;
/*---------------------------------------------------------Report details for all functions are declared in the SemanticAns.h-----------------------*/

void vInitialize(smt_data *smtTable, smt_error *smtError, token_expand *tokenExpand) {
	int i;
	head_procedure = tail_procedure = NULL;
	topStack = -1;
	statusProgram = statusProcedure = statusBegin = statusCommon = 0;

	for(i = 0; i < MAX_SMT_ELEMENTS; i ++) {
		(smtTable + i)->_status = (smtTable + i)->_index = END_SEMANTIC;
	}

	for(i = 0; i < MAX_SMT_ERRORS; i ++) {
		(smtError + i)->_error = END_SEMANTIC;
	}

	for(i = 0; i < MAX_TOKEN_EXPAND; i ++) {
		(tokenExpand + i)->_index = END_SEMANTIC;
	}
}

int iAnalysisSemantic(token_str *tokenTable, smt_data *smtTable, smt_error *smtError, token_expand *tokenExpand) {
	int iCount = 0;
	char *nameProgram;
	int iStatus;
	char *leftIden, *tempIden;
	smt_data smtData;
	procedure_paras *newProcedureParas = NULL, *resultProcedureParas = NULL;
	smt_procedure *newProcedureNode = NULL, *resultSmtProcedure;
	int errorIndex = 0;
	token_str *tokenTemple;
	smt_data *resultSmtElement;
	token_str *tokenTemp;

	if((tokenTable != NULL) && (smtTable != NULL) && (smtError != NULL) && (tokenExpand != NULL)) {
		vInitialize(smtTable, smtError, tokenExpand);

		if((tokenTable + iCount)->_index == kProgram) {
			iSetSemanticStatus(1, 0, 0, cmmProgram);
		} else {
			current_error = SEMANTIC_NOT_FOUND_KEYWORD_PROGRAM_AT_FIRST;

			print_error();
		}

		iCount ++;

		while((iCount < MAX_TOKEN) && ((tokenTable + iCount)->_index != -1)) {//printf("%s %d %d - %d %d %d %d\n", (tokenTable + iCount)->_value, (tokenTable + iCount)->_line, (tokenTable + iCount)->_column, statusProgram, statusProcedure, statusBegin, statusCommon);
			if(READ_PROGRAM_NAME(statusProgram, statusProcedure, statusBegin, statusCommon)) {
				nameProgram = (tokenTable + iCount)->_value;

				iCount ++;

				while(!NEXT_IDEN_PROGRAM((tokenTable + iCount)->_index)) {
					iCount ++;
				}

				switch((tokenTable + iCount)->_index) {
					case kVar: iSetSemanticStatus(statusProgram, statusProcedure, statusBegin, cmmVar);
						break;

					case kConst: iSetSemanticStatus(statusProgram, statusProcedure, statusBegin, cmmConst);
						break;

					case kProcedure: iSetSemanticStatus(statusProgram, statusProcedure + 1, statusBegin, cmmNameProcedure);
						break;

					case kBegin: iSetSemanticStatus(statusProgram, statusProcedure, statusBegin + 1, stStatement);
						break;

					default: {
						current_error = SEMANTIC_INVALID_AFTER_PROGRAM_DECLARATION;

						print_error();
					} break;
				}

				iCount ++;
			} else if(READ_VAR(statusProgram, statusProcedure, statusBegin, statusCommon)) {
				while(!AFTER_VAR((tokenTable + iCount)->_index)) {
					while((tokenTable + iCount)->_index != kSemicolon) {
						if((tokenTable + iCount)->_index == kIdentifier) {
							if(iSearchSmtTable(smtTable, &resultSmtElement, (tokenTable + iCount)->_value) != SMT_SEARCH_NAME_FOUND) {
								if(iSearchSmtProcedure(&head_procedure, &tail_procedure, &resultSmtProcedure, (tokenTable + iCount)->_value) != SEMANTIC_SUCCESS) {
									smtData._line = (tokenTable + iCount)->_line;
									smtData._column = (tokenTable + iCount)->_column;
									smtData._name = (tokenTable + iCount)->_value;
									smtData._type_value = smtInteger;
									smtData._index = iCount;//printf("%s  ", (tokenTable + iCount)->_value);

									if((tokenTable + iCount + 1)->_index == kLSquareParentheses) {
										smtData._type = smtArray;
									} else {
										smtData._type = smtVariable;
									}

									vPushSmtTable(smtTable, smtData);
								} else {
									iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtVariable, (tokenTable + iCount)->_value, SEMANTIC_E_DUPLICATE_IDEN_PROCEDURE_DECLARATION, errorIndex);

									errorIndex ++;
								}
							} else {
								iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtVariable, (tokenTable + iCount)->_value, SEMANTIC_E_DUPLICATE_IDEN_IDEN_DECLARE, errorIndex);

								errorIndex ++;
							}
						}

						iCount ++;
					}

					iCount ++;
				}//printf("New......... %d %d %d %d\n", statusProgram, statusProcedure, statusBegin, statusCommon);iPrintSmtTable(smtTable);

				switch((tokenTable + iCount)->_index) {
					case kVar: iSetSemanticStatus(statusProgram, statusProcedure, statusBegin, cmmVar);
						break;

					case kConst: iSetSemanticStatus(statusProgram, statusProcedure, statusBegin, cmmConst);
						break;

					case kProcedure: iSetSemanticStatus(statusProgram, statusProcedure + 1, statusBegin, cmmNameProcedure);
						break;

					case kBegin: iSetSemanticStatus(statusProgram, statusProcedure, statusBegin + 1, stStatement);
						break;

					default: {
						current_error = SEMANTIC_INVALID_AFTER_VAR_DECLARATION;

						print_error();
					} break;
				}

				iCount ++;
			} else if(READ_CONST(statusProgram, statusProcedure, statusBegin, statusCommon)) {
				while(!AFTER_CONST((tokenTable + iCount)->_index)) {
					while((tokenTable + iCount)->_index != kSemicolon) {
						smtData._type = smtConst;

						if((tokenTable + iCount)->_index == kIdentifier) {
							if(iSearchSmtTable(smtTable, &resultSmtElement, (tokenTable + iCount)->_value) != SMT_SEARCH_NAME_FOUND) {
								if(iSearchSmtProcedure(&head_procedure, &tail_procedure, &resultSmtProcedure, (tokenTable + iCount)->_value) != SEMANTIC_SUCCESS) {
									smtData._line = (tokenTable + iCount)->_line;
									smtData._column = (tokenTable + iCount)->_column;
									smtData._name = (tokenTable + iCount)->_value;
									smtData._index = iCount;

									while(!NEXT_VALUE_CONST((tokenTable + iCount)->_index)) {
										iCount ++;
									}

									switch((tokenTable + iCount)->_index) {
										case vInteger: smtData._type_value = smtInteger;
											break;

										case vReal: smtData._type_value = smtReal;
											break;

										case vChar: smtData._type_value = smtChar;
											break;

										case vString: smtData._type_value = smtStr;
											break;

										default: {
											current_error = SEMANTIC_INVALID_DATA_TYPE_CONST_DECLARATION;

											print_error();
										} break;
									}

									vPushSmtTable(smtTable, smtData);
								} else {
									iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtConst, (tokenTable + iCount)->_value, SEMANTIC_E_DUPLICATE_CONST_PROCEDURE_DECLARATION, errorIndex);

									errorIndex ++;
								}
							} else {
								iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtConst, (tokenTable + iCount)->_value, SEMANTIC_E_DUPLICATE_CONST_IDEN_DECLARATION, errorIndex);

								errorIndex ++;
							}
						}

						iCount ++;
					}

					iCount ++;
				}

				switch((tokenTable + iCount)->_index) {
					case kVar: iSetSemanticStatus(statusProgram, statusProcedure, statusBegin, cmmVar);
						break;

					case kConst: iSetSemanticStatus(statusProgram, statusProcedure, statusBegin, cmmConst);
						break;

					case kProcedure: iSetSemanticStatus(statusProgram, statusProcedure + 1, statusBegin, cmmNameProcedure);
						break;

					case kBegin: iSetSemanticStatus(statusProgram, statusProcedure, statusBegin + 1, stStatement);
						break;

					default: {
						current_error = SEMANTIC_INVALID_AFTER_CONST_DECLARATION;

						print_error();
					} break;
				}

				iCount ++;
			} else if(READ_NAME_PROCEDURE(statusProgram, statusProcedure, statusBegin, statusCommon)) {
				iIncreaseStatusAUnit(smtTable);

				if(iSearchSmtTable(smtTable, &resultSmtElement, (tokenTable + iCount)->_value) != SMT_SEARCH_NAME_FOUND) {
					if(iSearchSmtProcedure(&head_procedure, &tail_procedure, &resultSmtProcedure, (tokenTable + iCount)->_value) != SEMANTIC_SUCCESS) {
						if(iCreateNewNode((LPVOID*) (&newProcedureNode), TYPE_SMT_PROCEDURE) == SEMANTIC_SUCCESS) {
							iInsertSmtProcedure(&head_procedure, &tail_procedure, &newProcedureNode, (tokenTable + iCount)->_value);
							iIncreaseStatusSmtProcedure(&head_procedure, &tail_procedure);//printf("%s\n", (tokenTable + iCount)->_value);
						} else {
							print_error();
						}
					} else {
						iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtProcedure, (tokenTable + iCount)->_value, SEMANTIC_E_DUPLICATE_PROCEDURE_PROCEDURE_DECLARATION, errorIndex);

						errorIndex ++;
					}
				} else {
					iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtProcedure, (tokenTable + iCount)->_value, SEMANTIC_E_DUPLICATE_PROCEDURE_IDEN_DECLARATION, errorIndex);

					errorIndex ++;
				}

				iCount ++;

				if((tokenTable + iCount)->_index == kLParentheses) {
					iCount ++;

					for(; (tokenTable + iCount)->_index != kRParentheses; iCount ++) {
						if((tokenTable + iCount)->_index == kVar) {
							iCount ++;

							if(iSearchSmtTable(smtTable, &resultSmtElement, (tokenTable + iCount)->_value) != SMT_SEARCH_NAME_FOUND) {
								if(iSearchSmtProcedure(&head_procedure, &tail_procedure, &resultSmtProcedure, (tokenTable + iCount)->_value) != SEMANTIC_SUCCESS) {
									smtData._line = (tokenTable + iCount)->_line;
									smtData._column = (tokenTable + iCount)->_column;
									smtData._name = (tokenTable + iCount)->_value;
									smtData._index = iCount;
									smtData._type_value = smtInteger;
									smtData._type = smtParameterReference;

									vPushSmtTable(smtTable, smtData);

									if(iCreateNewNode((LPVOID*) (&newProcedureParas), TYPE_PROCEDURE_PARAS) == SEMANTIC_SUCCESS) {
										iInsertProcedurePara(&(newProcedureNode->_head_paras), &(newProcedureNode->_tail_paras), &newProcedureParas, (tokenTable + iCount)->_value, iCount, smtInteger, smtParameterReference);

										newProcedureParas = NULL;
									} else {
										print_error();
									}
								} else {
									iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtParameterReference, (tokenTable + iCount)->_value, SEMANTIC_E_DUPLICATE_PROCEDURE_IDEN_DECLARATION, errorIndex);

									errorIndex ++;
								}
							} else {
								iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtParameterReference, (tokenTable + iCount)->_value, SEMANTIC_E_DUPLICATE_PROCEDURE_IDEN_DECLARATION, errorIndex);

								errorIndex ++;
							}
						} else if((tokenTable + iCount)->_index == kIdentifier) {
							smtData._line = (tokenTable + iCount)->_line;
							smtData._column = (tokenTable + iCount)->_column;
							smtData._name = (tokenTable + iCount)->_value;
							smtData._index = iCount;
							smtData._type_value = smtInteger;
							smtData._type = smtParameterValue;

							vPushSmtTable(smtTable, smtData);

							if(iCreateNewNode((LPVOID*) (&newProcedureParas), TYPE_PROCEDURE_PARAS) == SEMANTIC_SUCCESS) {
								iInsertProcedurePara(&(newProcedureNode->_head_paras), &(newProcedureNode->_tail_paras), &newProcedureParas, (tokenTable + iCount)->_value, iCount, smtInteger, smtParameterValue);

								newProcedureParas = NULL;
							} else {
								print_error();
							}
						}
					}
				}//iPrintProcedurePara(&(newProcedureNode->_head_paras), &(newProcedureNode->_tail_paras));
//printf("New.............\n");iPrintSmtTable(smtTable);
				while(!AFTER_NAME_PROCEDURE((tokenTable + iCount)->_index)) {
					iCount ++;
				}

				switch((tokenTable + iCount)->_index) {
					case kVar: iSetSemanticStatus(statusProgram, statusProcedure, statusBegin, cmmVar);
						break;

					case kConst: iSetSemanticStatus(statusProgram, statusProcedure, statusBegin, cmmConst);
						break;

					case kProcedure: iSetSemanticStatus(statusProgram, statusProcedure + 1, statusBegin, cmmNameProcedure);
						break;

					case kBegin: iSetSemanticStatus(statusProgram, statusProcedure, statusBegin + 1, stStatement);
						break;

					default: {
						current_error = SEMANTIC_INVALID_AFTER_PROCEDURE_DECLARATION;

						print_error();
					} break;
				}//printf("Name procedure %d %d %d %d\n", statusProgram, statusProcedure, statusBegin, stStatement);iPrintSmtProcedure(&head_procedure, &tail_procedure);

				newProcedureNode = NULL;

				iCount ++;
			} else if(READ_STATEMENT(statusProgram, statusProcedure, statusBegin, statusCommon)) {
				switch((tokenTable + iCount)->_index) {
					case kIdentifier: statusCommon = cmmAssign;
						break;

					case kIf: statusCommon = cmmIf;
						break;

					case kCall: statusCommon = cmmCall;
						break;

					case kFor: statusCommon = cmmFor;
						break;

					case kWhile: statusCommon = cmmWhile;
						break;

					case kEnd: statusCommon = cmmEnd;
						break;

					default: printf("%s %d %d %d - %d %d %d %d\n", (tokenTable + iCount)->_value, (tokenTable + iCount)->_line, (tokenTable + iCount)->_column, (tokenTable + iCount)->_index, statusProgram, statusProcedure, statusBegin, statusCommon);
						break;
				}
			} else if(READ_STATEMENT_IF(statusCommon)) {
				while((tokenTable + iCount)->_index != kThen) {
					if((tokenTable + iCount)->_index == kIdentifier) {
						if(iSearchSmtTable(smtTable, &resultSmtElement, (tokenTable + iCount)->_value) != SMT_SEARCH_NAME_FOUND) {
							iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtVariable, (tokenTable + iCount)->_value, SEMANTIC_E_NOT_DECLARATION_VARIABLE_IF, errorIndex);

							errorIndex ++;
						}
					}

					iCount ++;
				}

				iCount ++;

				if((tokenTable + iCount)->_index == kBegin) {
					iSetSemanticStatus(statusProgram, statusProcedure, statusBegin + 1, stStatement);

					iCount ++;
				} else {
					iSetSemanticStatus(statusProgram, statusProcedure, statusBegin, stStatement);
				}
			} else if(READ_STATEMENT_WHILE(statusCommon)) {
				while((tokenTable + iCount)->_index != kDo) {
					if((tokenTable + iCount)->_index == kIdentifier) {
						if(iSearchSmtTable(smtTable, &resultSmtElement, (tokenTable + iCount)->_value) != SMT_SEARCH_NAME_FOUND) {
							iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtVariable, (tokenTable + iCount)->_value, SEMANTIC_E_NOT_DECLARATION_VARIABLE_WHILE, errorIndex);

							errorIndex ++;
						}
					}

					iCount ++;
				}

				iCount ++;

				if((tokenTable + iCount)->_index == kBegin) {
					iSetSemanticStatus(statusProgram, statusProcedure, statusBegin + 1, stStatement);

					iCount ++;
				} else {
					iSetSemanticStatus(statusProgram, statusProcedure, statusBegin, stStatement);
				}
			} else if(READ_STATEMENT_FOR(statusCommon)) {
				iCount ++;

				int iNumCharString = 0;
				int iNumValueNumber = 0;

				if(iSearchSmtTable(smtTable, &resultSmtElement, (tokenTable + iCount)->_value) != SMT_SEARCH_NAME_FOUND) {
					iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtVariable, (tokenTable + iCount)->_value, SEMANTIC_E_NOT_DECLARATION_VARIABLE_FOR, errorIndex);

					errorIndex ++;

					while((tokenTable + iCount)->_index != kTo) {
							iCount ++;
					}
				} else {
					leftIden = (tokenTable + iCount)->_value;

					while((tokenTable + iCount)->_index != kTo) {
						if((tokenTable + iCount)->_index == kIdentifier) {
							
							if(iSearchSmtTable(smtTable, &resultSmtElement, (tokenTable + iCount)->_value) != SMT_SEARCH_NAME_FOUND) {
								iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtVariable, (tokenTable + iCount)->_value, SEMANTIC_E_NOT_DECLARATION_VARIABLE_R_SIGN_FOR, errorIndex);

								errorIndex ++;
							} else {
								if(resultSmtElement->_type == smtArray) {
									tokenTemple = tokenTable + iCount;

									iCount ++;

									if((tokenTable + iCount)->_index != kLSquareParentheses) {
										iAddSemanticError(tokenTemple->_line, tokenTemple->_column, smtArray, tokenTemple->_value, SEMANTIC_E_IDEN_LOST_INDEX_ELEMENT_ARRAY, errorIndex);

										errorIndex ++;
									} else {
										iCount ++;

										while((tokenTable + iCount)->_index != kRSquareParentheses) {
											switch((tokenTable + iCount)->_index) {
												case kDivide: {
													iAddSemanticError(tokenTemple->_line, tokenTemple->_column, smtArray, tokenTemple->_value, SEMANTIC_E_DIVIDE_CREATE_REAL_VALUE_INDEX_ARRAY, errorIndex);

													errorIndex ++;
												} break;

												case vReal: {
													iAddSemanticError(tokenTemple->_line, tokenTemple->_column, smtArray, tokenTemple->_value, SEMANTIC_E_REAL_VALUE_IN_INDEX_ARRAY, errorIndex);

													errorIndex ++;
												} break;

												case kIdentifier: {
													if(iSearchSmtTable(smtTable, &resultSmtElement, leftIden) == SMT_SEARCH_NAME_FOUND) {
														switch(resultSmtElement->_type_value) {
															case smtReal: {
																iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtVariable, (tokenTable + iCount)->_value, SEMANTIC_E_REAL_V_INDEX_ARRAY, errorIndex);

																errorIndex ++;
															} break;

															case smtChar: {
																iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtVariable, (tokenTable + iCount)->_value, SEMANTIC_E_CHARACTER_V_INDEX_ARRAY, errorIndex);

																errorIndex ++;
															} break;

															case smtStr: {
																iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtVariable, (tokenTable + iCount)->_value, SEMANTIC_E_STRING_V_INDEX_ARRAY, errorIndex);

																errorIndex ++;
															} break;

															default: 
																break;
														}
													} else {
														iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtVariable, (tokenTable + iCount)->_value, SEMANTIC_E_NOT_DECLARATION_V_INDEX_ARRAY, errorIndex);

														errorIndex ++;
													}
												} break;

												default: {

												} break;
											}

											iCount ++;
										}
									}
								}

								switch(resultSmtElement->_type_value) {
									case smtReal: {
										iUpdateValueType(tokenTable, smtTable, leftIden, smtReal);

										iNumValueNumber ++;
									} break;

									case smtChar: {
										iUpdateValueType(tokenTable, smtTable, leftIden, smtChar);

										iNumCharString ++;
									} break;

									case smtStr: {
										iUpdateValueType(tokenTable, smtTable, leftIden, smtStr);

										iNumCharString ++;
									} break;

									case smtInteger: {
										iNumValueNumber ++;
									} break;

									default: 
										break;
								}
							}
						}

						iCount ++;
					}

					if(INVALID_R_EXPRESSION(iNumCharString, iNumValueNumber)) {
						iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtVariable, leftIden, SEMANTIC_E_INVALID_R_SIDE_ASSIGN, errorIndex);

						errorIndex ++;
					}

					iNumCharString = iNumValueNumber = 0;

					while((tokenTable + iCount)->_index != kDo) {
						if((tokenTable + iCount)->_index == kIdentifier) {
							if(iSearchSmtTable(smtTable, &resultSmtElement, (tokenTable + iCount)->_value) != SMT_SEARCH_NAME_FOUND) {
								iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtVariable, (tokenTable + iCount)->_value, SEMANTIC_E_NOT_DECLARATION_VARIABLE_R_SIGN_FOR, errorIndex);

								errorIndex ++;
							} else {
								if(resultSmtElement->_type == smtArray) {
									tokenTemple = tokenTable + iCount;

									iCount ++;


									if((tokenTable + iCount)->_index != kLSquareParentheses) {
										iAddSemanticError(tokenTemple->_line, tokenTemple->_column, smtArray, tokenTemple->_value, SEMANTIC_E_IDEN_LOST_INDEX_ELEMENT_ARRAY, errorIndex);


										errorIndex ++;
									} else {
										iCount ++;


										while((tokenTable + iCount)->_index != kRSquareParentheses) {
											switch((tokenTable + iCount)->_index) {

												case kDivide: {
													iAddSemanticError(tokenTemple->_line, tokenTemple->_column, smtArray, tokenTemple->_value, SEMANTIC_E_DIVIDE_CREATE_REAL_VALUE_INDEX_ARRAY, errorIndex);

													errorIndex ++;
												} break;

												case vReal: {
													iAddSemanticError(tokenTemple->_line, tokenTemple->_column, smtArray, tokenTemple->_value, SEMANTIC_E_REAL_VALUE_IN_INDEX_ARRAY, errorIndex);

													errorIndex ++;
												} break;


												case kIdentifier: {
													if(iSearchSmtTable(smtTable, &resultSmtElement, leftIden) == SMT_SEARCH_NAME_FOUND) {
														switch(resultSmtElement->_type_value) {
															case smtReal: {
																iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtVariable, (tokenTable + iCount)->_value, SEMANTIC_E_REAL_V_INDEX_ARRAY, errorIndex);

																errorIndex ++;
															} break;


															case smtChar: {
																iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtVariable, (tokenTable + iCount)->_value, SEMANTIC_E_CHARACTER_V_INDEX_ARRAY, errorIndex);

																errorIndex ++;
															} break;

															case smtStr: {
																iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtVariable, (tokenTable + iCount)->_value, SEMANTIC_E_STRING_V_INDEX_ARRAY, errorIndex);

																errorIndex ++;
															} break;

															default: 
																break;
														}
													} else {
														iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtVariable, (tokenTable + iCount)->_value, SEMANTIC_E_NOT_DECLARATION_V_INDEX_ARRAY, errorIndex);

														errorIndex ++;
													}
												} break;

												default: {

												} break;
											}

											iCount ++;
										}
									}
								}

								switch(resultSmtElement->_type_value) {
									case smtReal: {
										iUpdateValueType(tokenTable, smtTable, leftIden, smtReal);

										iNumValueNumber ++;
									} break;

									case smtChar: {
										iUpdateValueType(tokenTable, smtTable, leftIden, smtChar);

										iNumCharString ++;
									} break;

									case smtStr: {
										iUpdateValueType(tokenTable, smtTable, leftIden, smtStr);

										iNumCharString ++;
									} break;

									case smtInteger: {
										iNumValueNumber ++;
									} break;

									default: 
										break;
								}
							}
						}

						iCount ++;
					}

					if(INVALID_R_EXPRESSION(iNumCharString, iNumValueNumber)) {
						iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtVariable, leftIden, SEMANTIC_E_INVALID_R_SIDE_ASSIGN, errorIndex);

						errorIndex ++;
					}
				}

				iCount ++;

				if((tokenTable + iCount)->_index == kBegin) {
					iSetSemanticStatus(statusProgram, statusProcedure, statusBegin + 1, stStatement);

					iCount ++;
				} else {
					iSetSemanticStatus(statusProgram, statusProcedure, statusBegin, stStatement);
				}
			} else if(READ_STATEMENT_CALL(statusCommon)) {
				int iNumParameter = 0;
				procedure_paras *crt, *prv, *tmp;

				iCount ++;

				leftIden = (tokenTable + iCount)->_value;
				tokenTemp = tokenTable + iCount;

				if(iSearchSmtProcedure(&head_procedure, &tail_procedure, &resultSmtProcedure, (tokenTable + iCount)->_value) == SEMANTIC_SUCCESS) {
					iCount ++;

					if(NEXT_IDEN_CALL((tokenTable + iCount)->_index)) {
						switch((tokenTable + iCount)->_index) {
							case kSemicolon: {
								if((resultSmtProcedure->_head_paras != NULL) || (resultSmtProcedure->_tail_paras != NULL)) {
									iAddSemanticError(tokenTemp->_line, tokenTemp->_column, smtProcedure, leftIden, SEMANTIC_E_LOST_PARAMETER_LIST_PROCEDURE_CALL, errorIndex);

									errorIndex ++;
								}

								iCount ++;

								iSetSemanticStatus(statusProgram, statusProcedure, statusBegin, stStatement);
							} break;

							case kEnd: {
								if((resultSmtProcedure->_head_paras != NULL) || (resultSmtProcedure->_tail_paras != NULL)) {
									iAddSemanticError(tokenTemp->_line, tokenTemp->_column, smtProcedure, leftIden, SEMANTIC_E_LOST_PARAMETER_LIST_PROCEDURE_CALL, errorIndex);

									errorIndex ++;
								}

								iCount ++;

								iSetSemanticStatus(statusProgram, statusProcedure, statusBegin, cmmEnd);
							} break;

							case kLParentheses: {
								if((resultSmtProcedure->_head_paras == NULL) && (resultSmtProcedure->_tail_paras == NULL)) {
									iAddSemanticError(tokenTemp->_line, tokenTemp->_column, smtProcedure, leftIden, SEMANTIC_E_EXCESS_PARAMETER_LIST_PROCEDURE_CALL, errorIndex);

									errorIndex ++;
								} else {
									iCount ++;

									crt = resultSmtProcedure->_head_paras;
									prv = NULL;

									while(((tokenTable + iCount)->_index != kRParentheses) && (crt != NULL)) {
										if(IDENTIFIER_TOKEN_CALL((tokenTable + iCount)->_index)) {
											if(iSearchSmtTable(smtTable, &resultSmtElement, (tokenTable + iCount)->_value) == SMT_SEARCH_NAME_FOUND) {
												if(TYPE_REFERENCE_CALL(resultSmtElement->_type)) {
													if(crt->_type != smtParameterReference) {
														iAddSemanticError(tokenTemp->_line, tokenTemp->_column, smtParameterReference, leftIden, SEMANTIC_E_INVALID_REFERENCE_PARAMETER_CALL, errorIndex);

														errorIndex ++;
													} else {
														if(!VERIFY_TYPE_CALL(resultSmtElement->_type_value, crt->_type_value)) {
															iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtParameterReference, (tokenTable + iCount)->_value, SEMANTIC_E_NOT_MATCH_TYPE_REFERENCE_PARAMETER_CALL, errorIndex);

															errorIndex ++;
														}

														if(resultSmtElement->_type == smtArray) {
															token_str *tokenTemple1 = tokenTable + iCount;
															iCount ++;

															if((tokenTable + iCount)->_index != kLSquareParentheses) {
																iAddSemanticError(tokenTemple1->_line, tokenTemple1->_column, smtArray, tokenTemple1->_value, SEMANTIC_E_IDEN_LOST_INDEX_ELEMENT_ARRAY, errorIndex);

																errorIndex ++;
															} else {
																iCount ++;

																int iNumParentheses = 1;

																while(iNumParentheses != 0) {
																	switch((tokenTable + iCount)->_index) {
																		case kDivide: {
																			iAddSemanticError(tokenTemple1->_line, tokenTemple1->_column, smtArray, tokenTemple1->_value, SEMANTIC_E_DIVIDE_CREATE_REAL_VALUE_INDEX_ARRAY, errorIndex);

																			errorIndex ++;
																		} break;

																		case vReal: {
																			iAddSemanticError(tokenTemple1->_line, tokenTemple1->_column, smtArray, tokenTemple1->_value, SEMANTIC_E_REAL_VALUE_IN_INDEX_ARRAY, errorIndex);

																			errorIndex ++;
																		} break;

																		case kIdentifier: {
																			if(iSearchSmtTable(smtTable, &resultSmtElement, leftIden) == SMT_SEARCH_NAME_FOUND) {
																				switch(resultSmtElement->_type_value) {
																					case smtReal: {
																						iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtVariable, (tokenTable + iCount)->_value, SEMANTIC_E_REAL_V_INDEX_ARRAY, errorIndex);

																						errorIndex ++;
																					} break;

																					case smtChar: {
																						iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtVariable, (tokenTable + iCount)->_value, SEMANTIC_E_CHARACTER_V_INDEX_ARRAY, errorIndex);

																						errorIndex ++;
																					} break;

																					case smtStr: {
																						iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtVariable, (tokenTable + iCount)->_value, SEMANTIC_E_STRING_V_INDEX_ARRAY, errorIndex);

																						errorIndex ++;
																					} break;

																					default: 
																						break;
																				}
																			} else {
																				iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtVariable, (tokenTable + iCount)->_value, SEMANTIC_E_NOT_DECLARATION_V_INDEX_ARRAY, errorIndex);

																				errorIndex ++;
																			}
																		} break;

																		case kRSquareParentheses: iNumParentheses --;
																			break;

																		case kLSquareParentheses: iNumParentheses ++;
																			break;

																		default: {

																		} break;
																	}

																	iCount ++;
																}
															}
														}
														
													}
												} else if(TYPE_VALUE_CALL(resultSmtElement->_type)) {
													if(crt->_type != smtParameterValue) {
														iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtParameterReference, (tokenTable + iCount)->_value, SEMANTIC_E_INVALID_VALUE_PARAMETER_CALL, errorIndex);

														errorIndex ++;
													} else if(!VERIFY_TYPE_CALL(resultSmtElement->_type_value, crt->_type_value)) {
														iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtParameterReference, (tokenTable + iCount)->_value, SEMANTIC_NOT_MACTCH_TYPE_VALUE_PARAMETER_CALL, errorIndex);

														errorIndex ++;
													}
												}
											} else {
												iAddSemanticError(tokenTemp->_line, tokenTemp->_column, smtParameterReference, leftIden, SEMANTIC_E_NOT_DECLARATION_PARAMETER_CALL, errorIndex);

												errorIndex ++;
											}

											tmp = crt;
											crt = (procedure_paras *) ((u_int) (crt->_ptr) ^ (u_int) (prv));
											prv = tmp;
										} else if(VALUE_TOKEN_CALL((tokenTable + iCount)->_index)) {
											if(!VERIFY_TYPE_CALL(resultSmtElement->_type_value, crt->_type_value)) {
												iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtParameterReference, (tokenTable + iCount)->_value, SEMANTIC_NOT_MACTCH_TYPE_VALUE_PARAMETER_CALL, errorIndex);

												errorIndex ++;
											}

											tmp = crt;
											crt = (procedure_paras *) ((u_int) (crt->_ptr) ^ (u_int) (prv));
											prv = tmp;
										}

										iCount ++;
									}

									if((((tokenTable + iCount)->_index == kRParentheses) && (crt != NULL)) || (((tokenTable + iCount)->_index != kRParentheses) && (crt == NULL))) {
										iAddSemanticError(tokenTemp->_line, tokenTemp->_column, smtParameterReference, leftIden, SEMANTIC_E_INVALID_NUM_PARAMETER_CALL, errorIndex);

										errorIndex ++;
									}
								}

								while(!NEXT_CALL_PROCEDURE((tokenTable + iCount)->_index)) {
									iCount ++;
								}

								switch((tokenTable + iCount)->_index) {
									case kSemicolon: {
										/*if((resultSmtProcedure->_head_paras != NULL) || (resultSmtProcedure->_tail_paras != NULL)) {
											iAddSemanticError(tokenTemp->_line, tokenTemp->_column, smtProcedure, leftIden, SEMANTIC_E_LOST_PARAMETER_LIST_PROCEDURE_CALL, errorIndex);

											errorIndex ++;
										}*/

										iCount ++;

										iSetSemanticStatus(statusProgram, statusProcedure, statusBegin, stStatement);
									} break;

									case kEnd: {
										/*if((resultSmtProcedure->_head_paras != NULL) || (resultSmtProcedure->_tail_paras != NULL)) {
											iAddSemanticError(tokenTemp->_line, tokenTemp->_column, smtProcedure, leftIden, SEMANTIC_E_LOST_PARAMETER_LIST_PROCEDURE_CALL, errorIndex);

											errorIndex ++;
										}*/

										iCount ++;

										iSetSemanticStatus(statusProgram, statusProcedure, statusBegin, cmmEnd);
									} break;

									default: 
										break;
								}
							} break;

							default:
								break;
						}
					}
				} else {
					iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtProcedure, (tokenTable + iCount)->_value, SEMANTIC_E_NOT_DECLARATION_PROCEDURE_CALL, errorIndex);

					errorIndex ++;

					while(!NEXT_CALL_PROCEDURE((tokenTable + iCount)->_index)) {
						iCount ++;
					}

					switch((tokenTable + iCount)->_index) {
						case kSemicolon: {
							iCount ++;

							iSetSemanticStatus(statusProgram, statusProcedure, statusBegin, stStatement);
						} break;

						case kEnd: {

							iCount ++;

							iSetSemanticStatus(statusProgram, statusProcedure, statusBegin, cmmEnd);
						} break;

						default: 
							break;
					}
				}
			} else if(READ_ASSIGN(statusCommon)) {
				leftIden = (tokenTable + iCount)->_value;

				if(iSearchSmtTable(smtTable, &resultSmtElement, leftIden) != SMT_SEARCH_NAME_FOUND) {
					iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtVariable, (tokenTable + iCount)->_value, SEMANTIC_E_IDEN_NOT_DECLARATION, errorIndex);

					errorIndex ++;
				} else if(resultSmtElement->_type == smtConst) {
					iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtConst, (tokenTable + iCount)->_value, SEMANTIC_E_CHANGE_VALUE_CONST, errorIndex);

					errorIndex ++;
				} else if(resultSmtElement->_type == smtArray) {
					tokenTemple = tokenTable + iCount;

					iCount ++;

					if((tokenTable + iCount)->_index != kLSquareParentheses) {
						iAddSemanticError(tokenTemple->_line, tokenTemple->_column, smtArray, tokenTemple->_value, SEMANTIC_E_IDEN_LOST_INDEX_ELEMENT_ARRAY, errorIndex);

						errorIndex ++;
					} else {
						iCount ++;

						int iNumParentheses = 1;

						while(iNumParentheses != 0) {
							switch((tokenTable + iCount)->_index) {
								case kDivide: {
									iAddSemanticError(tokenTemple->_line, tokenTemple->_column, smtArray, tokenTemple->_value, SEMANTIC_E_DIVIDE_CREATE_REAL_VALUE_INDEX_ARRAY, errorIndex);

									errorIndex ++;
								} break;

								case vReal: {
									iAddSemanticError(tokenTemple->_line, tokenTemple->_column, smtArray, tokenTemple->_value, SEMANTIC_E_REAL_VALUE_IN_INDEX_ARRAY, errorIndex);

									errorIndex ++;
								} break;

								case kIdentifier: {
									if(iSearchSmtTable(smtTable, &resultSmtElement, leftIden) == SMT_SEARCH_NAME_FOUND) {
										switch(resultSmtElement->_type_value) {
											case smtReal: {
												iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtVariable, (tokenTable + iCount)->_value, SEMANTIC_E_REAL_V_INDEX_ARRAY, errorIndex);

												errorIndex ++;
											} break;

											case smtChar: {
												iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtVariable, (tokenTable + iCount)->_value, SEMANTIC_E_CHARACTER_V_INDEX_ARRAY, errorIndex);

												errorIndex ++;
											} break;

											case smtStr: {
												iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtVariable, (tokenTable + iCount)->_value, SEMANTIC_E_STRING_V_INDEX_ARRAY, errorIndex);

												errorIndex ++;
											} break;

											default: 
												break;
										}
									} else {
										iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtVariable, (tokenTable + iCount)->_value, SEMANTIC_E_NOT_DECLARATION_V_INDEX_ARRAY, errorIndex);

										errorIndex ++;
									}
								} break;

								case kRSquareParentheses: iNumParentheses --;
									break;

								case kLSquareParentheses: iNumParentheses ++;
									break;

								default: {

								} break;
							}

							iCount ++;
						}
					}
				}

				int iNumCharString = 0;
				int iNumValueNumber = 0;

				while(!END_STATEMENT((tokenTable + iCount)->_index)) {
					switch((tokenTable + iCount)->_index) {
						case vReal: {
							iUpdateValueType(tokenTable, smtTable, leftIden,  smtReal);

							iNumValueNumber ++;
						} break;

						case kDivide: {
							iUpdateValueType(tokenTable, smtTable, leftIden,  smtReal);

							iNumValueNumber ++;
						} break;

						case vChar: {
							iUpdateValueType(tokenTable, smtTable, leftIden,  smtChar);

							iNumCharString ++;
						} break;

						case vString: {
							iUpdateValueType(tokenTable, smtTable, leftIden,  smtStr);

							iNumCharString ++;
						} break;

						case kIdentifier: {
							if(iSearchSmtTable(smtTable, &resultSmtElement, (tokenTable + iCount)->_value) == SMT_SEARCH_NAME_FOUND) {
								if(resultSmtElement->_type == smtArray) {
									tokenTemple = tokenTable + iCount;

									iCount ++;

									if((tokenTable + iCount)->_index != kLSquareParentheses) {
										iAddSemanticError(tokenTemple->_line, tokenTemple->_column, smtArray, tokenTemple->_value, SEMANTIC_E_IDEN_LOST_INDEX_ELEMENT_ARRAY, errorIndex);

										errorIndex ++;
									} else {
										iCount ++;

										int iParentheses = 1;

										while(iParentheses != 0) {
											switch((tokenTable + iCount)->_index) {
												case kDivide: {
													iAddSemanticError(tokenTemple->_line, tokenTemple->_column, smtArray, tokenTemple->_value, SEMANTIC_E_DIVIDE_CREATE_REAL_VALUE_INDEX_ARRAY, errorIndex);

													errorIndex ++;
												} break;

												case vReal: {
													iAddSemanticError(tokenTemple->_line, tokenTemple->_column, smtArray, tokenTemple->_value, SEMANTIC_E_REAL_VALUE_IN_INDEX_ARRAY, errorIndex);

													errorIndex ++;
												} break;

												case kIdentifier: {
													if(iSearchSmtTable(smtTable, &resultSmtElement, leftIden) == SMT_SEARCH_NAME_FOUND) {
														switch(resultSmtElement->_type_value) {
															case smtReal: {
																iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtVariable, (tokenTable + iCount)->_value, SEMANTIC_E_REAL_V_INDEX_ARRAY, errorIndex);

																errorIndex ++;
															} break;

															case smtChar: {
																iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtVariable, (tokenTable + iCount)->_value, SEMANTIC_E_CHARACTER_V_INDEX_ARRAY, errorIndex);

																errorIndex ++;
															} break;

															case smtStr: {
																iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtVariable, (tokenTable + iCount)->_value, SEMANTIC_E_STRING_V_INDEX_ARRAY, errorIndex);

																errorIndex ++;
															} break;

															default: 
																break;
														}
													} else {
														iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtVariable, (tokenTable + iCount)->_value, SEMANTIC_E_NOT_DECLARATION_V_INDEX_ARRAY, errorIndex);

														errorIndex ++;
													}
												} break;

												case kRSquareParentheses: iParentheses --;
													break;

												case kLSquareParentheses: iParentheses ++;
													break;

												default: {

												} break;
											}

											iCount ++;
										}
									}
								}

								switch(resultSmtElement->_type_value) {
									case smtReal: {
										iUpdateValueType(tokenTable, smtTable, leftIden, smtReal);

										iNumValueNumber ++;
									} break;

									case smtChar: {
										iUpdateValueType(tokenTable, smtTable, leftIden, smtChar);

										iNumCharString ++;
									} break;

									case smtStr: {
										iUpdateValueType(tokenTable, smtTable, leftIden, smtStr);

										iNumCharString ++;
									} break;

									case smtInteger: {
										iNumValueNumber ++;
									} break;

									default: 
										break;
								}
							} else {
								iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtVariable, (tokenTable + iCount)->_value, SEMANTIC_E_NOT_DECLARATION, errorIndex);

								errorIndex ++;
							} 
						} break;

						case vInteger: {
							iNumValueNumber ++;
						} break;

						default: {

						} break;
					}

					iCount ++;
				}

				if(INVALID_R_EXPRESSION(iNumCharString, iNumValueNumber)) {
					iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtVariable, leftIden, SEMANTIC_E_INVALID_R_SIDE_ASSIGN, errorIndex);

					errorIndex ++;
				}

				if((tokenTable + iCount)->_index == kSemicolon) {
					statusCommon = stStatement;

					iCount ++;
				} else if((tokenTable + iCount)->_index == kEnd) {
					statusCommon = cmmEnd;
				}
			} else if(SWITCH_AFTER_END(statusCommon)) {
				iSetSemanticStatus(statusProgram, statusProcedure, statusBegin - 1, statusCommon);
				
				if(statusBegin == 0) {
					if(statusProcedure > 0) statusProcedure --;

					iDecreaseStatusAUnit(smtTable);
					vPopSmtTable(smtTable);
					iDecreaseStatusSmtProcedure(&head_procedure, &tail_procedure);
					iDeleteSubSmtProcedure(&head_procedure, &tail_procedure);//printf("Delete.........\n");iPrintSmtTable(smtTable);
					iCount ++;//printf("End\n");iPrintSmtProcedure(&head_procedure, &tail_procedure);

					while(!AFTER_END_KEYWORD_PROCEDURE((tokenTable + iCount)->_index)) {
						iCount ++;
					}

					switch((tokenTable + iCount)->_index) {
						case kDot: {
							iSetSemanticStatus(statusProgram - 1, statusProcedure, statusBegin, stStatement);

							iCount --;
						} break;

						case kBegin: {
							iSetSemanticStatus(statusProgram, statusProcedure, statusBegin + 1, stStatement);
						} break;

						case kProcedure: {
							iSetSemanticStatus(statusProgram, statusProcedure + 1, statusBegin, cmmNameProcedure);
						} break;

						case kVar: {
							iSetSemanticStatus(statusProgram, statusProcedure, statusBegin, cmmVar);
						} break;

						case kConst: {
							iSetSemanticStatus(statusProgram, statusProcedure, statusBegin, cmmConst);
						} break;

						default: 
							break;
					}

					iCount ++;
				} else {
					while(!AFTER_END_KEYWORD_SUB_PLAIN((tokenTable + iCount)->_index)) {
						iCount ++;
					}

					if((tokenTable + iCount)->_index == kEnd) {
						statusCommon = cmmEnd;
					} else {
						iSetSemanticStatus(statusProgram, statusProcedure, statusBegin, stStatement);
					}
				}
			} else if(statusProgram == 0) {
				if(!BEGIN_END_PROGRAM(statusProgram, statusProcedure, statusBegin)) {
					iAddSemanticError((tokenTable + iCount)->_line, (tokenTable + iCount)->_column, smtParameterReference, (tokenTable + iCount)->_value, SEMANTIC_E_DUPLICATE_PROCEDURE_IDEN_DECLARATION, errorIndex);

					errorIndex ++;
				}

				break;
			} 
		}
	} else {
		current_error = SEMANTIC_NULL_NODES_PASSED_ANALYSIS;

		print_error();
	}

	return current_error;
}

int vPushSmtTable(smt_data *smtTable, smt_data smtElement) {
	(smtTable + topStack + 1)->_line = smtElement._line;
	(smtTable + topStack + 1)->_column = smtElement._column;
	(smtTable + topStack + 1)->_name = smtElement._name;
	(smtTable + topStack + 1)->_type_value = smtElement._type_value;
	(smtTable + topStack + 1)->_type = smtElement._type;
	(smtTable + topStack + 1)->_status = VALUE_INTIALIZE_STATUS;
	(smtTable + topStack + 1)->_index = smtElement._index;

	topStack ++;

	return topStack;
}

int vPopSmtTable(smt_data *smtTable) {
	for(; (topStack >= 0) && ((smtTable + topStack)->_status == VALUE_DICCUSSED_STATUS); topStack --) {
		(smtTable + topStack)->_status = END_SEMANTIC;
	}

	return topStack;
}

int iSearchSmtTable(smt_data *smtTable, smt_data **result, char *idenName) {
	int i;

	for(i = 0; (i < MAX_SMT_ELEMENTS) && ((smtTable + i)->_status != END_SEMANTIC); i ++) {
		if(strcmp(idenName, (smtTable + i)->_name) == 0) {
			*result = smtTable + i;

			return SMT_SEARCH_NAME_FOUND;
		}
	}

	return SMT_SEARCH_NAME_NOT_FOUND;
}

int iSearchValueType(smt_data *smtTable, char *idenValue) {
	int i = 0;

	while(((smtTable + i)->_index != END_SEMANTIC) && (i < MAX_SMT_ELEMENTS)) {
		if(strcmp(idenValue, (smtTable + i)->_name) == 0) {
			return (smtTable + i)->_type_value;
		}
	}

	return VALUE_TYPE_SEARCH_NOT_FOUND;
}

int iUpdateValueType(token_str *tokenTable, smt_data *smtTable, char *idenValue, int iNewValue) {
	int i = 0;
	int resultValue;
	smt_procedure *crt, *prv, *tmp;
	procedure_paras *resultPara = NULL;

	while(((smtTable + i)->_index != END_SEMANTIC) && (i < MAX_SMT_ELEMENTS)) {
		if(strcmp(idenValue, (smtTable + i)->_name) == 0) {
			(smtTable + i)->_type_value = iNewValue;

			resultValue = iNewValue;
		}

		i ++;
	}
/*
	i = 0;

	while(((tokenTable + i)->_index != END_LEXICAL_TABLE) && (i < MAX_TOKEN)) {
		if(strcmp(idenValue, (tokenTable + i)->_value) == 0) {
			switch(iNewValue) {
				case smtInteger: {
					(tokenTable + i)->_index = vInteger;

					resultValue = vInteger;
				} break;

				case smtReal: {
					(tokenTable + i)->_index = vReal;

					resultValue = vReal;
				} break;

				case smtChar: {
					(tokenTable + i)->_index = vChar;

					resultValue = vChar;
				} break;

				case smtStr: {
					(tokenTable + i)->_index = vString;

					resultValue = vString;
				} break;

				default:
					break;
			}
		}

		i ++;
	}
*/
	if(head_procedure != NULL) {
		for(crt = head_procedure, prv = NULL; crt != NULL; tmp = crt, crt = (smt_procedure *) ((u_int) (crt->_ptr) ^ (u_int) (prv)), prv = tmp) {
			iSearchProcedurePara(&(crt->_head_paras), &(crt->_tail_paras), &resultPara, idenValue);

			if(resultPara != NULL) {
				resultPara->_type_value = iNewValue;
			}

			return (resultValue = iNewValue);
		}
	} else {
		current_error = SEMANTIC_NULL_SMT_PROCEDURE_UPDATE_TYPE;

		print_error();
	}

	return UPDATE_VALUE_TYPE_FAILURE;
}

int iInserSmtError(smt_error *smtErrorTable, smt_error *errorElement) {
	static int index = 0;

	if(index >= MAX_SMT_ERRORS) {
		current_error = SEMANTIC_TOO_MUCH_ERRORS;

		print_error();
	} else {
		(smtErrorTable + index)->_line = errorElement->_line;
		(smtErrorTable + index)->_column = errorElement->_column;
		(smtErrorTable + index)->_type = errorElement->_type;
		(smtErrorTable + index)->_error = errorElement->_error;

		(smtErrorTable + index)->_name = malloc(sizeof(char) * (strlen(errorElement->_name) + 2));

		strcmp((smtErrorTable + index)->_name, errorElement->_name);

		current_error = SEMANTIC_SUCCESS;
	}

	return current_error;
}

int iIncreaseStatusAUnit(smt_data *smtTable) {
	int i;

	if(smtTable != NULL) {
		for(i = 0; (i < MAX_SMT_ELEMENTS) && ((smtTable + i)->_status != END_SEMANTIC); i ++) {
			(smtTable + i)->_status ++;
		}

		current_error = SEMANTIC_SUCCESS;
	} else {
		current_error = SEMANTIC_NULL_NODE_INCREASE_STATUS;

		print_error();
	}

	return current_error;
}

int iDecreaseStatusAUnit(smt_data *smtTable) {
	int i;

	if(smtTable != NULL) {
		for(i = 0; (i < MAX_SMT_ELEMENTS) && ((smtTable + i)->_status != END_SEMANTIC); i ++) {
			(smtTable + i)->_status --;
		}

		current_error = SEMANTIC_SUCCESS;
	} else {
		current_error = SEMANTIC_NULL_NODE_DECREASE_STATUS;

		print_error();
	}

	return current_error;
}

int iSetSemanticStatus(int newStProgram, int newStProcedure, int newStBegin, int newStCommon) {
	statusProgram = newStProgram;
	statusProcedure = newStProcedure;
	statusBegin = newStBegin;
	statusCommon = newStCommon;

	return current_error = SEMANTIC_SUCCESS;
}

int iAddSemanticError(int line, int column, int type, char *name, int error, int index) {
	if(index >= MAX_SMT_ERRORS) {
		current_error = SEMANTIC_TOO_MUCH_ERRORS;

		print_error();

		exit(1);
	} else {
		(smtError + index)->_line = line;
		(smtError + index)->_column = column;
		(smtError + index)->_type = type;
		(smtError + index)->_name = name;
		(smtError + index)->_error = error;
	}

	semanticStatus = SEMANTIC_FAILURE;

	return error;
}

int iPrintSmtTable(smt_data *smtTable) {
	int i;

	if(smtTable != NULL) {
		for(i = 0;  (i < MAX_SMT_ELEMENTS) && ((smtTable + i)->_status != END_SEMANTIC); i ++) {
			printf("Line: %d\n", (smtTable + i)->_line);
			printf("Column: %d\n", (smtTable + i)->_column);
			printf("Token table index: %d\n", (smtTable + i)->_index);
			printf("Name: %s\n", (smtTable + i)->_name);
			printf("Type of value: %d\n", (smtTable + i)->_type_value);
			printf("Type: %d\n", (smtTable + i)->_type);
			printf("Status: %d\n", (smtTable + i)->_status);

			printf("\n");
		}
	} else {
		current_error = SEMANTIC_NULL_NODE_PRINT_SMT_TABLE;

		print_error();
	}

	return current_error;
}

int iFilePrintSmtTable(char *fileName, smt_data *smtTable) {
	int i;
	FILE *file;

	if(smtTable != NULL) {
		if((fileName != NULL) && (strcmp(fileName, "") != 0)) {
			if((file = fopen(fileName, "w")) != NULL) {
				for(i = 0;  (i < MAX_SMT_ELEMENTS) && ((smtTable + i)->_status != END_SEMANTIC); i ++) {
					fprintf(file, "Line: %d\n", (smtTable + i)->_line);
					fprintf(file, "Column: %d\n", (smtTable + i)->_column);
					fprintf(file, "Token table index: %d\n", (smtTable + i)->_index);
					fprintf(file, "Name: %s\n", (smtTable + i)->_name);
					fprintf(file, "Type of value: %d\n", (smtTable + i)->_type_value);
					fprintf(file, "Type: %d\n", (smtTable + i)->_type);
					fprintf(file, "Status: %d\n", (smtTable + i)->_status);

					fprintf(file, "\n");
				}

				fclose(file);
			} else {
				current_error = SEMANTIC_COULDNT_CREATE_NEW_FILE_PRINT_SMT_TABLE;

				print_error();
			}
		} else {
			current_error = SEMANTIC_INVALID_FILE_NAME_FILE_PRINT_SMT_TABLE;

			print_error();
		}
	} else {
		current_error = SEMANTIC_NULL_NODE_FILE_PRINT_SMT_TABLE;

		print_error();
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
					((smt_procedure *) (*node))->_status = VALUE_INTIALIZE_STATUS;

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

	return current_error;
}

int iInsertProcedurePara(procedure_paras **head, procedure_paras **tail, procedure_paras **node, char *name, int order, int type_value, int type) {
	procedure_paras *tmp;

	if(*node != NULL) {
		(*node)->_order = order;
		(*node)->_type = type;
		(*node)->_type_value = type_value;
		(*node)->_name = name;

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

int iDeleteProcedurePara(procedure_paras **head, procedure_paras **tail) {
	procedure_paras *crt;

	if(*head != NULL) {
		for(crt = *head; crt != NULL; crt = *head) {
			*head = (procedure_paras *) ((u_int) ((*head)->_ptr) ^ (u_int) (NULL));
			free(crt);
		} 
	}

	return (current_error = SEMANTIC_SUCCESS);
}

int iSearchProcedurePara(procedure_paras **head, procedure_paras **tail, procedure_paras **result, char *name) {
	procedure_paras *crt, *prv, *tmp;

	if(*head != NULL) {
		for(crt = *head, prv = NULL; crt != NULL; tmp = crt, crt = (procedure_paras *) ((u_int) (crt->_ptr) ^ (u_int) (prv)), prv = tmp) {
			if(strcmp(name, crt->_name) == 0) {
				*result = crt;

				current_error = SEMANTIC_SUCCESS;

				return current_error;
			}
		}
	} else {
		current_error = SEMANTIC_NULL_NODE_SEARCH_PROCEDURE_PARAS;

		//print_error();
	}

	*result = NULL;

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

		for(crt = *head, prv = NULL; crt != NULL; tmp = crt, crt = (procedure_paras *) ((u_int) (crt->_ptr) ^ (u_int) (prv)), prv = tmp) {
			printf("\tOrder: %d\n", crt->_order);
			printf("\tType: ");

			switch(crt->_type) {
				case smtParameterValue: printf("Parameter passed by value\n");
					break;

				case smtParameterReference: printf("Parameter passed by reference\n");
					break;

				default: printf("Unknow type of parameter\n");
					break;
			}

			printf("\tName: %s\n", crt->_name);

			printf("\tType of value: %d\n", crt->_type_value);

			printf("\n");
		}

		current_error = SEMANTIC_SUCCESS;
	} else {
		current_error = SEMANTIC_NULL_NODE_PRINT_PROCEDURE_PARAS;

		print_error();
	}

	return current_error;
}

int iFilePrintProcedurePara(FILE *file, procedure_paras **head, procedure_paras **tail) {
	procedure_paras *crt, *prv, *tmp;

	if(*head != NULL) {
		if(file != NULL) {
			fprintf(file, "Number parameters: %d\n", iGetSumProcedurePara(head, tail));

			for(crt = *head, prv = NULL; crt != NULL; tmp = crt, crt = (procedure_paras *) ((u_int) (crt->_ptr) ^ (u_int) (prv)), prv = tmp) {
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

				fprintf(file, "\tName: %s\n", crt->_name);

				fprintf(file, "\tType of value: %d\n", crt->_type_value);

				fprintf(file, "\n");
			}

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

int iDeleteSubSmtProcedure(smt_procedure **head, smt_procedure **tail) {
	smt_procedure *crt, *prv, *tmp;

	if(*head != NULL) {
		crt = *head;
		prv = NULL;

		while(crt != NULL) {
			if(crt->_status == OUT_STATUS_SMT_PROCEDURE) {
				if(*tail == *head) {
					*head = *tail = NULL;

					free(crt);

					crt = NULL;
				} else if(crt == *head) {
					*head = (smt_procedure *) ((u_int) (crt->_ptr) ^ (u_int) (NULL));
					(*head)->_ptr = (smt_procedure *) ((u_int) (crt) ^ (u_int) ((*head)->_ptr));

					free(crt);

					crt = *head;
					prv = NULL;
				} else if(crt == *tail) {
					*tail = prv;

					(*tail)->_ptr = (smt_procedure *) ((u_int) ((*tail)->_ptr) ^ (u_int) (crt));

					free(crt);

					crt = NULL;
				} else {
					tmp = crt;
					crt = (smt_procedure *) ((u_int) (crt->_ptr) ^ (u_int) (prv));
					prv->_ptr = (smt_procedure *) ((u_int) (prv->_ptr) ^ (u_int) (crt) ^ (u_int) (tmp));
					crt->_ptr = (smt_procedure *) ((u_int) (crt->_ptr) ^ (u_int) (prv) ^ (u_int) (tmp));

					free(tmp);
					tmp = NULL;
				}
			} else {
				tmp = crt;
				crt = (smt_procedure *) ((u_int) (crt->_ptr) ^ (u_int) (prv));
				prv = tmp;
			}
		}

		current_error = SEMANTIC_SUCCESS;
	} else {
		current_error = SEMANTIC_NULL_NODE_DELETE_SUB_SMT_PROCEDURE;

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

		current_error = SEMANTIC_SUCCESS;
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

	if(*head != NULL) {
		printf("Number of procedure: %d\n", iGetSumSmtProcedure(head, tail));

		for(crt = *head, prv = NULL; crt != NULL; tmp = crt, crt = (smt_procedure *) ((u_int) (crt->_ptr) ^ (u_int) (prv)), prv = tmp, i ++) {
			printf("%d. Name: %s\n", i, crt->_name);

			printf("Status: %d\n", crt->_status);

			if((crt->_head_paras != NULL) && (crt->_tail_paras != NULL)) {
				iPrintProcedurePara(&(crt->_head_paras), &(crt->_head_paras));
			} else {
				printf("Emtpy parameters\n");

				printf("\n");
			}
		}

		current_error = SEMANTIC_SUCCESS;
	} else {
		current_error = SEMANTIC_NULL_NODE_PRINT_SMT_PROCEDURE;

		print_error();
	}

	return current_error;
}

int iFilePrintSmtProcedure(char *fileName, smt_procedure **head, smt_procedure **tail) {
	smt_procedure *crt, *prv, *tmp;
	FILE *file;
	int i = 1;

	if(*head != NULL) {
		if((fileName != NULL) && (strcmp(fileName, "") != 0)) {
			if((file = fopen(fileName, "w")) != NULL) {
				fprintf(file, "Number of procedure: %d\n", iGetSumSmtProcedure(head, tail));

				for(crt = *head, prv = NULL; crt != NULL; tmp = crt, crt = (smt_procedure *) ((u_int) (crt->_ptr) ^ (u_int) (prv)), prv = tmp, i ++) {
					fprintf(file, "%d. Name: %s\n", i, crt->_name);

					fprintf(file, "Status: %d\n", crt->_status);

					if((crt->_head_paras != NULL) && (crt->_tail_paras != NULL)) {
						iFilePrintProcedurePara(file, &(crt->_head_paras), &(crt->_head_paras));
					} else {
						fprintf(file, "Emtpy parameters\n");

						fprintf(file, "\n");
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

int iIncreaseStatusSmtProcedure(smt_procedure **head, smt_procedure **tail) {
	smt_procedure *crt, *prv, *tmp;

	if(*head != NULL) {
		for(crt = *head, prv = NULL; crt != NULL; tmp = crt, crt = (smt_procedure *) ((u_int) (crt->_ptr) ^ (u_int) (prv)), prv = tmp) {
			crt->_status ++;
		}

		current_error = SEMANTIC_SUCCESS;
	} else {
		current_error = SEMANTIC_NULL_NODE_INCREASE_SMT_PROCEDURE;

		print_error();
	}

	return current_error;
}

int iDecreaseStatusSmtProcedure(smt_procedure **head, smt_procedure **tail) {
	smt_procedure *crt, *prv, *tmp;

	if(*head != NULL) {
		for(crt = *head, prv = NULL; crt != NULL; tmp = crt, crt = (smt_procedure *) ((u_int) (crt->_ptr) ^ (u_int) (prv)), prv = tmp) {
			crt->_status --;
		}

		current_error = SEMANTIC_SUCCESS;
	} else {
		current_error = SEMANTIC_NULL_NODE_DECREASE_SMT_PROCEDURE;

		print_error();
	}

	return current_error;
}
