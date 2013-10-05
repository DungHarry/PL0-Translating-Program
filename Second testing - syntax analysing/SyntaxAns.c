/*
	Author: Lý Việt Dũng - Hanoi University of Science and Technology
	Date created: 3rd, October 2013
	Operating System: Ubuntu 13.04 Desktop x86_64
	Compiler: GNU compiler 4.7.3
	Program: PL0 translate program
	Module: Syntax analysis

	Description: this is source code for syntax analysis process
*/

#include "SyntaxAns.h"

//Declare some extern variables here

extern token_str tokenTable[MAX_TOKEN];
extern char* lexical_keyword_parsing[MAX_LEXICAL_KEYWORD];
extern int iLineSyntaxError;
extern int iColumnSyntaxError;
/*--------Report details for each functions in syntax analysis process-------------*/

int iProcessFactor(token_str *table) {
	if(lpToken != NULL) {
		iLineSyntaxError = lpToken->_line;
		iColumnSyntaxError = lpToken->_column;

		if(IS_FACTOR(lpToken->_index)) {
			if(lpToken->_index == kIdentifier) {
				lpToken = iNextToken(table);

				if(lpToken != NULL) {
					iLineSyntaxError = lpToken->_line;
					iColumnSyntaxError = lpToken->_column;
				} else {
					current_error = SYNTAX_LOST_TOKEN_AFTER_FACTOR;
					vPrintSyntaxErrors();
				}

				if(lpToken->_index == kLSquareParentheses) {
					lpToken = iNextToken(table);

					if((lpToken != NULL) && BEGIN_INDEX_ARRAY(lpToken->_index)) {
						iProcessExpression(table);

						if(lpToken->_index == kRSquareParentheses) {
							lpToken = iNextToken(table);

							if(lpToken != NULL) {
								iLineSyntaxError = lpToken->_line;
								iColumnSyntaxError = lpToken->_column;
							} else {
								current_error = SYNTAX_LOST_TOKEN_AFTER_FACTOR;
								vPrintSyntaxErrors();
							}
						} else {
							current_error = SYNTAX_LOST_R_SQUARE_BRACKET;

							vPrintSyntaxErrors();
						}
					} else {
						current_error = SYNTAX_INVALID_AT_BEGIN_INDEX_ARRAY;

						vPrintSyntaxErrors();
					}
				}
			} else if(lpToken->_index == kLParentheses) {
				lpToken = iNextToken(table);

				if((lpToken == NULL) || !IS_FACTOR(lpToken->_index)) {
					current_error = SYNTAX_INVALID_AT_BEGIN_EXPRESSION;

					vPrintSyntaxErrors();
				}

				iProcessExpression(table);

				if(lpToken->_index != kRParentheses) {
					current_error = SYNTAX_LOST_R_PARENTHESES;

					vPrintSyntaxErrors();
				}

				lpToken = iNextToken(table);

				if(lpToken != NULL) {
					iLineSyntaxError = lpToken->_line;
					iColumnSyntaxError = lpToken->_column;
				} else {
					current_error = SYNTAX_INVALID_FACTOR_TYPE;

					vPrintSyntaxErrors();
				}
			} else {
				lpToken = iNextToken(table);

				if(lpToken != NULL) {
					iLineSyntaxError = lpToken->_line;
					iColumnSyntaxError = lpToken->_column;
				} else {
					current_error = SYNTAX_INVALID_FACTOR_TYPE;

					vPrintSyntaxErrors();
				}
			}

			current_error = SYNTAX_SUCCESS;
		} else {
			current_error = SYNTAX_INVALID_FACTOR_TYPE;

			vPrintSyntaxErrors();
		}
	} else {
		current_error = SYNTAX_LOST_FACTOR;

		vPrintSyntaxErrors();
	}

	return current_error;
}

int iProcessTerm(token_str *table) {
	iProcessFactor(table);

	while(NEXT_TERM(lpToken->_index)) {
		lpToken = iNextToken(table);

		iProcessFactor(table);
	}

	return (current_error = SYNTAX_SUCCESS);
}

int iProcessExpression(token_str *table) {
	if(NEXT_EXPRESSION(lpToken->_index)) {
		lpToken = iNextToken(table);
	}

	iProcessTerm(table);

	while(NEXT_EXPRESSION(lpToken->_index)) {
		lpToken = iNextToken(table);

		iProcessTerm(table);
	}

	return (current_error = SYNTAX_SUCCESS);
}

int iProcessCondition(token_str *table) {
	iProcessExpression(table);

	if(IS_CONDITION_OPERATOR(lpToken->_index)) {
		lpToken = iNextToken(table);

		if(lpToken == NULL) {
			current_error = SYNTAX_LOST_R_EXPRESSION_IN_CONDITION;

			vPrintSyntaxErrors();
		}

		iProcessExpression(table);
	} else {
		current_error = SYNTAX_LOST_COMPARISION_OPER;

		vPrintSyntaxErrors();
	}

	return (current_error = SYNTAX_SUCCESS);
}

int iProcessStatement(token_str *table) {
	if(lpToken == NULL) {
		current_error = SYNTAX_LOST_WORD_AT_BEGIN_STATEMENT;

		vPrintSyntaxErrors();
	}

	switch(lpToken->_index) {
		case kIdentifier: {
			lpToken = iNextToken(table);

			if((lpToken != NULL) && NEXT_ASSIGN(lpToken->_index)) {
				iLineSyntaxError = lpToken->_line;
				iColumnSyntaxError = lpToken->_column;

				if(lpToken->_index == kLSquareParentheses) {
					lpToken = iNextToken(table);

					if((lpToken != NULL) && BEGIN_INDEX_ARRAY(lpToken->_index)) {
						iProcessExpression(table);

						if(lpToken->_index == kRSquareParentheses) {
							lpToken = iNextToken(table);

							if(lpToken != NULL) {
								iLineSyntaxError = lpToken->_line;
								iColumnSyntaxError = lpToken->_column;
							}
						} else {
							current_error = SYNTAX_LOST_R_SQUARE_BRACKET;

							vPrintSyntaxErrors();
						}
					} else {
						current_error = SYNTAX_INVALID_AT_BEGIN_INDEX_ARRAY;

						vPrintSyntaxErrors();
					}
				}

				if((lpToken == NULL) || (lpToken->_index != kAssign)) {
					current_error = SYNTAX_LOST_ASSIGN_OPERATOR;

					vPrintSyntaxErrors();
				}

				lpToken = iNextToken(table);

				if((lpToken == NULL) || !IS_FACTOR(lpToken->_index)) {
					current_error = SYNTAX_INVALID_AT_BEGIN_EXPRESSION;

					vPrintSyntaxErrors();
				}

				iProcessExpression(table);
			} else {
				current_error = SYNTAX_LOST_KEYWORD_ASSIGN_OPERATOR;

				vPrintSyntaxErrors();
			}
		} break;

		case kBegin: {
			lpToken = iNextToken(table);

			if(lpToken == NULL) {
				current_error = SYNTAX_LOST_EXPRESSION_AFTER_BEGIN_KEYWORD;

				vPrintSyntaxErrors();
			} else {
				iLineSyntaxError = lpToken->_line;
				iColumnSyntaxError = lpToken->_column;
			}

			iProcessStatement(table);

			while(lpToken->_index == kSemicolon) {
				lpToken = iNextToken(table);

				if(lpToken == NULL) {
					current_error = SYNTAX_LOST_EXPRESSION_AFTER_BEGIN_KEYWORD;

					vPrintSyntaxErrors();
				} else {
					iLineSyntaxError = lpToken->_line;
					iColumnSyntaxError = lpToken->_column;
				}

				if(lpToken->_index == kEnd) {
					current_error = SYNTAX_EXCESS_SEMICOLON_BEFORE_END_BEGIN_KEYWORK;

					vPrintSyntaxErrors();
				}

				iProcessStatement(table);
			}

			if(lpToken->_index != kEnd) {
				current_error = SYNTAX_LOST_END_KEYWORD_STATEMENT;

				vPrintSyntaxErrors();
			}

			lpToken = iNextToken(table);

			if(lpToken == NULL) {
				current_error = SYNTAX_LOST_EXPRESSION_AFTER_BEGIN_KEYWORD;

				vPrintSyntaxErrors();
			} else {
				iLineSyntaxError = lpToken->_line;
				iColumnSyntaxError = lpToken->_column;
			}
		} break;

		case kCall: {
			lpToken = iNextToken(table);

			if((lpToken == NULL) || (lpToken->_index != kIdentifier)) {
				current_error = SYNTAX_LOST_IDENTIFIER_CALL;

				vPrintSyntaxErrors();
			} else {
				iLineSyntaxError = lpToken->_line;
				iColumnSyntaxError = lpToken->_column;
			}

			lpToken = iNextToken(table);

			if(lpToken == NULL) {
				current_error = SYNTAX_INVALID_WORD_CALL;

				vPrintSyntaxErrors();
			} else {
				iLineSyntaxError = lpToken->_line;
				iColumnSyntaxError = lpToken->_column;
			}

			if(lpToken->_index == kLParentheses) {
				lpToken = iNextToken(table);

				if((lpToken == NULL) || !BEGIN_EXPRESSION(lpToken->_index)) {
					current_error = SYNTAX_INVALID_AT_BEGIN_EXPRESSION;

					vPrintSyntaxErrors();
				} else {
					iLineSyntaxError = lpToken->_line;
					iColumnSyntaxError = lpToken->_column;
				}

				iProcessExpression(table);

				while(lpToken->_index == kCommas) {
					lpToken = iNextToken(table);

					if((lpToken == NULL) || !BEGIN_EXPRESSION(lpToken->_index)) {
						current_error = SYNTAX_INVALID_AT_BEGIN_EXPRESSION;

						vPrintSyntaxErrors();
					} else {
						iLineSyntaxError = lpToken->_line;
						iColumnSyntaxError = lpToken->_column;
					}

					iProcessExpression(table);
				}

				if(lpToken->_index != kRParentheses) {
					current_error = SYNTAX_LOST_R_PARENTHESES;

					vPrintSyntaxErrors();
				} else {
					iLineSyntaxError = lpToken->_line;
					iColumnSyntaxError = lpToken->_column;

					lpToken = iNextToken(table);

					if(lpToken == NULL) {
						current_error = SYNTAX_INVALID_WORD_CALL;

						vPrintSyntaxErrors();
					} else {
						iLineSyntaxError = lpToken->_line;
						iColumnSyntaxError = lpToken->_column;
					}
				}
			}
		} break;

		case kIf: {
			lpToken = iNextToken(table);

			if(lpToken == NULL) {
				current_error = SYNTAX_LOST_CONDITION_IF;

				vPrintSyntaxErrors();
			} else {
				iLineSyntaxError = lpToken->_line;
				iColumnSyntaxError = lpToken->_column;
			}

			iProcessCondition(table);

			if(lpToken->_index != kThen) {
				current_error = SYNTAX_LOST_KEYWORD_THEN_IF;

				vPrintSyntaxErrors();
			} else {
				iLineSyntaxError = lpToken->_line;
				iColumnSyntaxError = lpToken->_column;
			}

			lpToken = iNextToken(table);

			if(lpToken == NULL) {
				current_error = SYNTAX_LOST_STATEMENT_IF;

				vPrintSyntaxErrors();
			} else {
				iLineSyntaxError = lpToken->_line;
				iColumnSyntaxError = lpToken->_column;
			}

			iProcessStatement(table);

			if(lpToken->_index == kElse) {
				lpToken = iNextToken(table);

				if(lpToken == NULL) {
					current_error = SYNTAX_LOST_STATEMENT_IF;

					vPrintSyntaxErrors();
				} else {
					iLineSyntaxError = lpToken->_line;
					iColumnSyntaxError = lpToken->_column;
				}

				iProcessStatement(table);
			}
		} break;

		case kWhile: {
			lpToken = iNextToken(table);

			if(lpToken == NULL) {
				current_error = SYNTAX_LOST_CONDITION_WHILE;

				vPrintSyntaxErrors();
			} else {
				iLineSyntaxError = lpToken->_line;
				iColumnSyntaxError = lpToken->_column;
			}

			iProcessCondition(table);

			if(lpToken->_index != kDo) {
				current_error = SYNTAX_LOST_KEYWORD_DO_WHILE;

				vPrintSyntaxErrors();
			} else {
				iLineSyntaxError = lpToken->_line;
				iColumnSyntaxError = lpToken->_column;
			}

			lpToken = iNextToken(table);

			if(lpToken == NULL) {
				current_error = SYNTAX_LOST_STATEMENT_WHILE;

				vPrintSyntaxErrors();
			} else {
				iLineSyntaxError = lpToken->_line;
				iColumnSyntaxError = lpToken->_column;
			}

			iProcessStatement(table);
		} break;

		case kFor: {
			lpToken = iNextToken(table);

			if((lpToken == NULL) || (lpToken->_index != kIdentifier)) {
				current_error = SYNTAX_LOST_IDENTIFIER_FOR;

				vPrintSyntaxErrors();
			} else {
				iLineSyntaxError = lpToken->_line;
				iColumnSyntaxError = lpToken->_column;
			}

			lpToken = iNextToken(table);

			if((lpToken == NULL) || (lpToken->_index != kAssign)) {
				current_error = SYNTAX_LOST_ASSIGN_OPERATOR_FOR;

				vPrintSyntaxErrors();
			} else {
				iLineSyntaxError = lpToken->_line;
				iColumnSyntaxError = lpToken->_column;
			}

			lpToken = iNextToken(table);

			if(lpToken == NULL) {
				current_error = SYNTAX_LOST_STATEMENT_FOR;

				vPrintSyntaxErrors();
			} else {
				iLineSyntaxError = lpToken->_line;
				iColumnSyntaxError = lpToken->_column;
			}

			iProcessExpression(table);

			if((lpToken == NULL) || (lpToken->_index != kTo)) {
				current_error = SYNTAX_LOST_KEYWORD_TO_FOR;

				vPrintSyntaxErrors();
			} else {
				iLineSyntaxError = lpToken->_line;
				iColumnSyntaxError = lpToken->_column;
			}

			lpToken = iNextToken(table);

			if(lpToken == NULL) {
				current_error = SYNTAX_LOST_STATEMENT_FOR;

				vPrintSyntaxErrors();
			} else {
				iLineSyntaxError = lpToken->_line;
				iColumnSyntaxError = lpToken->_column;
			}

			iProcessExpression(table);

			if(lpToken->_index != kDo) {
				current_error = SYNTAX_LOST_KEYWORD_DO_FOR;

				vPrintSyntaxErrors();
			} else {
				iLineSyntaxError = lpToken->_line;
				iColumnSyntaxError = lpToken->_column;
			}

			lpToken = iNextToken(table);

			if(lpToken == NULL) {
				current_error = SYNTAX_LOST_STATEMENT_FOR;

				vPrintSyntaxErrors();
			} else {
				iLineSyntaxError = lpToken->_line;
				iColumnSyntaxError = lpToken->_column;
			}

			iProcessStatement(table);
		} break;

		default: {
			current_error = SYNTAX_INVALID_BEGIN_STATEMENT;

			vPrintSyntaxErrors();
		} break;
	}

	return (current_error = SYNTAX_SUCCESS);
}

int iProcessBlock(token_str *table) {
	if(lpToken == NULL) {
		current_error = SYNTAX_INVALID_BEGIN_BLOCK;

		vPrintSyntaxErrors();
	}

	if(!BEGIN_BLOCK(lpToken->_index)) {
		current_error = SYNTAX_INVALID_BEGIN_BLOCK;

		vPrintSyntaxErrors();
	}

	while(BEGIN_BLOCK(lpToken->_index)) {
		switch(lpToken->_index) {
			case kConst: {
				LoopingConst:

					lpToken = iNextToken(table);

					if((lpToken == NULL) || (lpToken->_index != kIdentifier)) {
						current_error = SYNTAX_INVALID_IDENTIFIER_CONST;

						vPrintSyntaxErrors();
					} else {
						iLineSyntaxError = lpToken->_line;
						iColumnSyntaxError = lpToken->_column;
					}

					lpToken = iNextToken(table);

					if((lpToken == NULL) || (lpToken->_index != kEqualCpr)) {
						current_error = SYNTAX_LOST_EQUAL_CONST;

						vPrintSyntaxErrors();
					} else {
						iLineSyntaxError = lpToken->_line;
						iColumnSyntaxError = lpToken->_column;
					}

					lpToken = iNextToken(table);

					if((lpToken == NULL) || !VALUE_CONST(lpToken->_index)) {
						current_error = SYNTAX_INVALID_VALUE_CONST;

						vPrintSyntaxErrors();
					} else {
						iLineSyntaxError = lpToken->_line;
						iColumnSyntaxError = lpToken->_column;
					}

					lpToken = iNextToken(table);

					if((lpToken == NULL) || !NEXT_CONST(lpToken->_index)) {
						current_error = SYNTAX_INVALID_END_CONST;

						vPrintSyntaxErrors();
					} else {
						iLineSyntaxError = lpToken->_line;
						iColumnSyntaxError = lpToken->_column;
					}

					if(lpToken->_index == kCommas) {
						goto LoopingConst;
					}

					lpToken = iNextToken(table);

					if(lpToken == NULL) {
						current_error = SYNTAX_INVALID_THE_END_CONST;

						vPrintSyntaxErrors();
					} else {
						iLineSyntaxError = lpToken->_line;
						iColumnSyntaxError = lpToken->_column;
					}
			} break;

			case kVar: {
				LoopingVar:

					lpToken = iNextToken(table);

					if((lpToken == NULL) || (lpToken->_index != kIdentifier)) {
						current_error = SYNTAX_INVALID_IDENTIFIER_VAR;

						vPrintSyntaxErrors();
					} else {
						iLineSyntaxError = lpToken->_line;
						iColumnSyntaxError = lpToken->_column;
					}

					lpToken = iNextToken(table);

					if((lpToken == NULL) || !NEXT_VAR(lpToken->_index)) {
						current_error = SYNTAX_INVALID_AFTER_IDENTIFIER_VAR;

						vPrintSyntaxErrors();
					} else {
						iLineSyntaxError = lpToken->_line;
						iColumnSyntaxError = lpToken->_column;
					}

					if(lpToken->_index == kLSquareParentheses) {
						lpToken = iNextToken(table);

						if((lpToken == NULL) || (lpToken->_index != vInteger)) {
							current_error = SYNTAX_INVALID_INTEGER_INDEX_VAR;

							vPrintSyntaxErrors();
						} else {
							iLineSyntaxError = lpToken->_line;
							iColumnSyntaxError = lpToken->_column;
						}

						lpToken = iNextToken(table);

						if((lpToken == NULL) || (lpToken->_index != kRSquareParentheses)) {
							current_error = SYNTAX_LOST_R_SQUARE_PARENTHESES_VAR;

							vPrintSyntaxErrors();
						} else {
							iLineSyntaxError = lpToken->_line;
							iColumnSyntaxError = lpToken->_column;
						}

						lpToken = iNextToken(table);

						if((lpToken == NULL) || !NEXT_VAR_SIMPLE(lpToken->_index)) {
							current_error = SYNTAX_INVALID_NEXT_VAR;

							vPrintSyntaxErrors();
						} else {
							iLineSyntaxError = lpToken->_line;
							iColumnSyntaxError = lpToken->_column;
						}
					}

					if(lpToken->_index == kCommas) {
						goto LoopingVar;
					}

					lpToken = iNextToken(table);

					if(lpToken == NULL) {
						current_error = SYNTAX_INVALID_THE_END_VAR;

						vPrintSyntaxErrors();
					} else {
						iLineSyntaxError = lpToken->_line;
						iColumnSyntaxError = lpToken->_column;
					}
			} break;

			case kProcedure: {
				lpToken = iNextToken(table);

				if((lpToken == NULL) || (lpToken->_index != kIdentifier)) {
					current_error = SYNTAX_LOST_IDENTIFIER_AFTER_KEYWORD_PROCEDURE;

					vPrintSyntaxErrors();
				} else {
					iLineSyntaxError = lpToken->_line;
					iColumnSyntaxError = lpToken->_column;
				}

				lpToken = iNextToken(table);

				if((lpToken == NULL) || !NEXT_PROCEDURE(lpToken->_index)) {
					current_error = SYNTAX_INVALID_TOKEN_AFTER_IDENTIFIER_PROCEDURE;

					vPrintSyntaxErrors();
				} else {
					iLineSyntaxError = lpToken->_line;
					iColumnSyntaxError = lpToken->_column;
				}

				if(lpToken->_index == kLParentheses) {
					LoopingParameterProcedure:

						lpToken = iNextToken(table);

						if((lpToken == NULL) || !BEGIN_PARAMETER_PROCEDURE(lpToken->_index)) {
							current_error = SYNTAX_INVALID_BEGIN_PARAMETER_PROCEDURE;

							vPrintSyntaxErrors();
						} else {
							iLineSyntaxError = lpToken->_line;
							iColumnSyntaxError = lpToken->_column;
						}

						if(lpToken->_index == kVar) {
							lpToken = iNextToken(table);

							if(lpToken == NULL) {
								current_error = SYNTAX_LOST_IDENTIFIER_PARAMETER_PROCEDURE;

								vPrintSyntaxErrors();
							} else {
								iLineSyntaxError = lpToken->_line;
								iColumnSyntaxError = lpToken->_column;
							}
						}

						if(lpToken->_index != kIdentifier) {
							current_error = SYNTAX_LOST_IDENTIFIER_PARAMETER_PROCEDURE;

							vPrintSyntaxErrors();
						} else {
							iLineSyntaxError = lpToken->_line;
							iColumnSyntaxError = lpToken->_column;
						}

						lpToken = iNextToken(table);

						if((lpToken == NULL) || !NEXT_PARAMETER_PRODURE(lpToken->_index)) {
							current_error = SYNTAX_INVALID_NEXT_PARAMETER_PROCEDURE;

							vPrintSyntaxErrors();
						} else {
							iLineSyntaxError = lpToken->_line;
							iColumnSyntaxError = lpToken->_column;
						}

						if(lpToken->_index == kSemicolon) {
							goto LoopingParameterProcedure;
						}

						lpToken = iNextToken(table);

						if(lpToken == NULL) {
							current_error = SYNTAX_LOST_SEMICOLON_PROCEDURE;

							vPrintSyntaxErrors();
						} else {
							iLineSyntaxError = lpToken->_line;
							iColumnSyntaxError = lpToken->_column;
						} 
				}

				if(lpToken->_index != kSemicolon) {
					current_error = SYNTAX_LOST_SEMICOLON_PROCEDURE;

					vPrintSyntaxErrors();
				} else {
					iLineSyntaxError = lpToken->_line;
					iColumnSyntaxError = lpToken->_column;
				}

				lpToken = iNextToken(table);

				if(lpToken == NULL) {
					current_error = SYNTAX_LOST_STATEMENT_PROCEDURE;

					vPrintSyntaxErrors();
				} else {
					iLineSyntaxError = lpToken->_line;
					iColumnSyntaxError = lpToken->_column;
				}

				iProcessBlock(table);

				if(lpToken->_index != kSemicolon) {
					current_error = SYNTAX_LOST_SEMICOLON_PROCEDURE;

					vPrintSyntaxErrors();
				} else {
					iLineSyntaxError = lpToken->_line;
					iColumnSyntaxError = lpToken->_column;
				}

				lpToken = iNextToken(table);

				if(lpToken == NULL) {
					current_error = SYNTAX_INVALID_THE_END_PROCEDURE;

					vPrintSyntaxErrors();
				} else {
					iLineSyntaxError = lpToken->_line;
					iColumnSyntaxError = lpToken->_column;
				}
			} break;

			case kBegin: {
				LoopingBegin: 

					lpToken = iNextToken(table);

					if(lpToken == NULL) {
						current_error = SYNTAX_INVALID_NEXT_BEGIN;

						vPrintSyntaxErrors();
					} else {
						iLineSyntaxError = lpToken->_line;
						iColumnSyntaxError = lpToken->_column;
					}

					iProcessStatement(table);

					if(!NEXT_BEGIN(lpToken->_index)) {
						current_error = SYNTAX_INVALID_AFTER_STATEMENT_BEGIN;

						vPrintSyntaxErrors();
					} else {
						iLineSyntaxError = lpToken->_line;
						iColumnSyntaxError = lpToken->_column;
					}

					if(lpToken->_index == kSemicolon) {
						goto LoopingBegin;	
					}

					if(lpToken->_index != kEnd) {
						current_error = SYNTAX_LOST_END_BEGIN;

						vPrintSyntaxErrors();
					} else {
						iLineSyntaxError = lpToken->_line;
						iColumnSyntaxError = lpToken->_column;
					}

					lpToken = iNextToken(table);

					if(lpToken == NULL) {
						current_error = SYNTAX_INVALID_THE_END_BEGIN;

						vPrintSyntaxErrors();
					} else {
						iLineSyntaxError = lpToken->_line;
						iColumnSyntaxError = lpToken->_column;
					}

					goto EndBlock;
			} break;

			default: {
				current_error = SYNTAX_INVALID_BEGIN_BLOCK;

				vPrintSyntaxErrors();
			} break;
		}
	}

	EndBlock:

	return (current_error = SYNTAX_SUCCESS);
}

int iProccessSyntax(token_str *table) {
	lpToken = iNextToken(table);

	if((lpToken == NULL) || (lpToken->_index != kProgram)) {
		current_error = SYNTAX_LOST_KEYWORD_PROGRAM_MAIN;

		vPrintSyntaxErrors();
	} else {
		iLineSyntaxError = lpToken->_line;
		iColumnSyntaxError = lpToken->_column;
	}

	lpToken = iNextToken(table);

	if((lpToken == NULL) || (lpToken->_index != kIdentifier)) {
		current_error = SYNTAX_LOST_IDENTIFIER_MAIN;

		vPrintSyntaxErrors();
	} else {
		iLineSyntaxError = lpToken->_line;
		iColumnSyntaxError = lpToken->_column;
	}

	lpToken = iNextToken(table);

	if((lpToken == NULL) || (lpToken->_index != kSemicolon)) {
		current_error = SYNTAX_LOST_SEMICOLON_MAIN;

		vPrintSyntaxErrors();
	} else {
		iLineSyntaxError = lpToken->_line;
		iColumnSyntaxError = lpToken->_column;
	}

	lpToken = iNextToken(table);

	if(lpToken == NULL) {
		current_error = SYNTAX_INVALID_TOKEN_MAIN;

		vPrintSyntaxErrors();
	} else {
		iLineSyntaxError = lpToken->_line;
		iColumnSyntaxError = lpToken->_column;
	}

	iProcessBlock(table);

	if(lpToken->_index != kDot) {
		current_error = SYNTAX_LOST_DOT_MAIN;

		vPrintSyntaxErrors();
	} else {
		printf("Syntax analysing successfully!\n");
	}

	return (current_error = SYNTAX_SUCCESS);
}
