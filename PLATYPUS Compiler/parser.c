#define _CRT_SECURE_NO_WARNINGS
#include "token.h"
#include "buffer.h"
#include "parser.h"
#include <stdlib.h>

void parser(void) {

	current = malar_next_token();
	program(); match(SEOF_T, NO_ATTR);
	gen_incode("PLATY: Source file parsed");
}

void match(int pr_token_code, int pr_token_attribute) {

	/* the attribute code is used only when the token
	is one of the following codes, else return */
	if (current.code != pr_token_code) {
		syn_eh(pr_token_code);
		return;
	}

	/* if match code with source end of file, return */
	if (current.code == SEOF_T) {
		return;
	}

	/* match the attribute code with the follow codes */
	switch (pr_token_code) {
	case KW_T:
	case LOG_OP_T:
	case ART_OP_T:
	case REL_OP_T:
		if (current.attribute.get_int == pr_token_attribute) {
			break;
		}
		else {
			syn_eh(pr_token_code);
			return;
		}
		break;
	}
	/* if match is successful, set current to token*/
	current = malar_next_token();
	/* if code equal error token calls error printing func, set current to token and increment synerrno */
	if (current.code == ERR_T) {
		syn_printe();
		current = malar_next_token();
		++synerrno;
		return;
	}
}

void syn_eh(int sync_token_code) {

	syn_printe();
	++synerrno;

	do {
		current = malar_next_token();
		if (current.code == SEOF_T) {
			if (sync_token_code == SEOF_T)
				return;
			else exit(synerrno);
		}
	} while (current.code != sync_token_code);

	current = malar_next_token();
	return;
}

void syn_printe(void) {

	Token t = current;

	printf("PLATY: Syntax error:  Line:%3d\n", line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code) {
	case  ERR_T: /* ERR_T     0   Error token */
		printf("%s\n", t.attribute.err_lex);
		break;
	case  SEOF_T: /*SEOF_T    1   Source end-of-file token */
		printf("SEOF_T\t\t%d\t\n", t.attribute.seof);
		break;
	case  AVID_T: /* AVID_T    2   Arithmetic Variable identifier token */
	case  SVID_T:/* SVID_T    3  String Variable identifier token */
		printf("%s\n", t.attribute.vid_lex);
		break;
	case  FPL_T: /* FPL_T     4  Floating point literal token */
		printf("%5.1f\n", t.attribute.flt_value);
		break;
	case INL_T: /* INL_T      5   Integer literal token */
		printf("%d\n", t.attribute.get_int);
		break;
	case STR_T:/* STR_T     6   String literal token */
		b_markc(str_LTBL, t.attribute.str_offset);
		printf("%s\n", b_location(str_LTBL,str_LTBL->markc_offset));
		break;
	case SCC_OP_T: /* 7   String concatenation operator token */
		printf("NA\n");
		break;
	case  ASS_OP_T:/* ASS_OP_T  8   Assignment operator token */
		printf("NA\n");
		break;
	case  ART_OP_T:/* ART_OP_T  9   Arithmetic operator token */
		printf("%d\n", t.attribute.get_int);
		break;
	case  REL_OP_T: /*REL_OP_T  10   Relational operator token */
		printf("%d\n", t.attribute.get_int);
		break;
	case  LOG_OP_T:/*LOG_OP_T 11  Logical operator token */
		printf("%d\n", t.attribute.get_int);
		break;
	case  LPR_T: /*LPR_T    12  Left parenthesis token */
		printf("NA\n");
		break;
	case  RPR_T: /*RPR_T    13  Right parenthesis token */
		printf("NA\n");
		break;
	case LBR_T: /*    14   Left brace token */
		printf("NA\n");
		break;
	case RBR_T: /*    15  Right brace token */
		printf("NA\n");
		break;
	case KW_T: /*     16   Keyword token */
		printf("%s\n", kw_table[t.attribute.get_int]);
		break;
	case COM_T: /* 17   Comma token */
		printf("NA\n");
		break;
	case EOS_T: /*    18  End of statement *(semi - colon) */
		printf("NA\n");
		break;
	default:
		printf("PLATY: Scanner error: invalid token code: %d\n", t.code);
	}/*end switch*/
}/* end syn_printe()*/

void gen_incode(char* string) {

	printf("%s\n", string);
}

void program(void) {

	match(KW_T, PLATYPUS);
	match(LBR_T, NO_ATTR);
	optStatements();
	match(RBR_T, NO_ATTR);
	gen_incode("PLATY: Program parsed");

}

void optStatements(void) {

	switch (current.code) {
	case AVID_T:
	case SVID_T: statements(); break;
	case KW_T:
		if (current.attribute.get_int != PLATYPUS
			&& current.attribute.get_int != ELSE
			&& current.attribute.get_int != THEN
			&& current.attribute.get_int != REPEAT) {
			statements();
			break;
		}
	default: /*empty string – optional statements*/;
		gen_incode("PLATY: Opt_statements parsed");
	}
}

void statements(void) {

	statement();
	statements_p();
}

void statement(void) {

	switch (current.code)
	{
	case AVID_T:
	case SVID_T:
		assignmentStatement(); break;
	case KW_T:
		switch (current.attribute.get_int)
		{
		case IF: selectionStatement(); break;
		case WHILE: iterationStatement(); break;
		case READ: inputStatement(); break;
		case WRITE: outputStatement(); break;
		} break;
	default: syn_printe(); break;
	}
}

void statements_p(void) {

	switch (current.code) {
	case AVID_T:
	case SVID_T: statement(); statements_p(); break;
	case KW_T:
		switch (current.attribute.kwt_idx) {
		case IF:
		case WHILE:
		case READ:
		case WRITE: statement(); statements_p(); break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void assignmentStatement(void) {

	assignmentExpression();
	match(EOS_T, NO_ATTR);
	gen_incode("PLATY: Assignment statement parsed");
}

void assignmentExpression(void) {

	switch (current.code)
	{
	case AVID_T:
		match(AVID_T, NO_ATTR);
		match(ASS_OP_T, EQ); //
		arithmeticExpression();
		gen_incode("PLATY: Assignment expression (arithmetic) parsed");
		break;
	case SVID_T:
		match(SVID_T, NO_ATTR);
		match(ASS_OP_T, EQ); //
		stringExpression();
		gen_incode("PLATY: Assignment expression (string) parsed");
		break;
	default:
		syn_printe();
		break;
	}
}

void selectionStatement(void) {

	match(KW_T, IF);
	preCondition();
	match(LPR_T, NO_ATTR);
	conditionalExpression();
	match(RPR_T, NO_ATTR);
	match(KW_T, THEN);
	match(LBR_T, NO_ATTR);
	optStatements();
	match(RBR_T, NO_ATTR);
	match(KW_T, ELSE);
	match(LBR_T, NO_ATTR);
	optStatements();
	match(RBR_T, NO_ATTR);
	match(EOS_T, NO_ATTR);
	gen_incode("PLATY: Selection statement parsed");
}

void iterationStatement(void) {

	match(KW_T, WHILE);
	preCondition();
	match(LPR_T, NO_ATTR);
	conditionalExpression();
	match(RPR_T, NO_ATTR);
	match(KW_T, REPEAT);
	match(LBR_T, NO_ATTR);
	statements();
	match(RBR_T, NO_ATTR);
	match(EOS_T, NO_ATTR);
	gen_incode("PLATY: Iteration statement parsed");
}

void preCondition(void) {

	switch (current.code)
	{
	case KW_T:
		switch (current.attribute.kwt_idx)
		{
		case TRUE:	match(KW_T, TRUE); break;
		case FALSE:	match(KW_T, FALSE); break;
		default:
			syn_printe();
			break;
		}
		break;
	default:
		syn_printe();
		break;
	}
}

void inputStatement(void) {

	match(KW_T, READ);
	match(LPR_T, NO_ATTR);
	variableList();
	match(RPR_T, NO_ATTR);
	match(EOS_T, NO_ATTR);
	gen_incode("PLATY: Input statement parsed");
}
void variableList(void) {

	variableIdentifier();
	variableList_p();
	gen_incode("PLATY: Variable list parsed");
}
void variableList_p(void) {

	switch (current.code)
	{
	case COM_T:
		match(COM_T, NO_ATTR);
		variableIdentifier();
		variableList_p();
		break;
	default:
		break;
	}
}

void variableIdentifier(void) {

	switch (current.code) {
	case AVID_T: match(AVID_T, NO_ATTR); break; // match Arithmetic Variable identifier token
	case SVID_T: match(SVID_T, NO_ATTR); break; // match String Variable identifier token
	default: syn_printe();
		break;
	}
}

void outputStatement(void) {

	match(KW_T, WRITE);
	match(LPR_T, NO_ATTR);
	outputList();
	match(RPR_T, NO_ATTR);
	match(EOS_T, NO_ATTR);
	gen_incode("PLATY: Output statement parsed");
}
void outputList(void) {

	switch (current.code) {
	case AVID_T:
	case SVID_T: variableList(); break;
	case STR_T: match(STR_T, NO_ATTR);
		gen_incode("PLATY: Output list (string literal) parsed");
		break;
	default: gen_incode("PLATY: Output list (empty) parsed");
		break;
	}
}
void arithmeticExpression(void) {

	switch (current.code) {
	case ART_OP_T:
		switch (current.attribute.arr_op) {
		case PLUS:
		case MINUS: unaryArithmeticExpression(); break;
		default: syn_printe(); break;
		}
		break;
	case AVID_T:
	case FPL_T:
	case INL_T:
	case LPR_T:
		additiveArithmeticExpression(); break;
	default: syn_printe();
		break;
	}
	gen_incode("PLATY: Arithmetic expression parsed");
}

void unaryArithmeticExpression(void) {

	switch (current.code) {
	case ART_OP_T:
		switch (current.attribute.get_int) {
		case PLUS:
		case MINUS:
			match(ART_OP_T, current.attribute.get_int);
			primaryArithmeticExpression();
			break;
		default: syn_printe();
			break;
		}
	}
	gen_incode("PLATY: Unary arithmetic expression parsed");
}

void additiveArithmeticExpression(void) {

	multiplicativeArithmeticExpression();
	additiveArithmeticExpression_p();
}

void additiveArithmeticExpression_p(void) {

	switch (current.code) {
	case ART_OP_T:
		switch (current.attribute.get_int)
		{
		case PLUS:
		case MINUS:
			match(ART_OP_T, current.attribute.arr_op);
			multiplicativeArithmeticExpression();
			additiveArithmeticExpression_p();
			gen_incode("PLATY: Additive arithmetic expression parsed");
			break;
		default:
			break;
		}
	default:
		break;
	}
}
void multiplicativeArithmeticExpression(void) {

	primaryArithmeticExpression();
	multiplicativeArithmeticExpression_p();
}

void multiplicativeArithmeticExpression_p(void) {

	switch (current.code) {
	case ART_OP_T:
		switch (current.attribute.get_int) {
		case MULT:
		case DIV:
			match(ART_OP_T, current.attribute.get_int);
			primaryArithmeticExpression();
			multiplicativeArithmeticExpression_p();
			gen_incode("PLATY: Multiplicative arithmetic expression parsed");
			break;
		default:
			break;
		}
	}
}

void primaryArithmeticExpression(void) {

	switch (current.code) {
	case AVID_T:
	case FPL_T:
	case INL_T:
		match(current.code, NO_ATTR); break;
	case LPR_T:
		match(LPR_T, NO_ATTR);
		arithmeticExpression();
		match(RPR_T, NO_ATTR); break;
	default: syn_printe();
		break;
	}
	gen_incode("PLATY: Primary arithmetic expression parsed");
}

void stringExpression(void) {
	primaryStringExpression();
	stringExpression_p();
	gen_incode("PLATY: String expression parsed");
}

void stringExpression_p(void) {

	switch (current.code) {
	case SCC_OP_T:
		match(SCC_OP_T, NO_ATTR);
		primaryStringExpression();
		stringExpression_p();
		break;
	default:
		break;
	}
}


void primaryStringExpression(void) {
	switch (current.code)
	{
	case SVID_T:
		match(SVID_T, NO_ATTR);
		break;
	case STR_T:
		match(STR_T, NO_ATTR);
		break;
	default:
		syn_printe();
		break;
	}
	gen_incode("PLATY: Primary string expression parsed");
}

void conditionalExpression(void) {
	ORlogicalExpression();
	gen_incode("PLATY: Conditional expression parsed");
}

void ORlogicalExpression(void) {
	ANDlogicalExpression();
	ORlogicalExpression_p();
}

void ORlogicalExpression_p(void) {
	switch (current.code)
	{
	case LOG_OP_T:
		switch (current.attribute.get_int)
		{
		case OR:
			match(LOG_OP_T, OR);
			ANDlogicalExpression();
			ORlogicalExpression_p();
			gen_incode("PLATY: Logical OR expression parsed");
		default:
			break;
		}
		break;
	default:
		//	syn_printe();
		break;
	}
}

void ANDlogicalExpression(void) {
	relationalExpression();
	ANDlogicalExpression_p();
}

void ANDlogicalExpression_p(void) {
	switch (current.code)
	{
	case LOG_OP_T:
		switch (current.attribute.get_int)
		{
		case AND:
			match(LOG_OP_T, AND);
			relationalExpression();
			ANDlogicalExpression_p();
			gen_incode("PLATY: Logical AND expression parsed");
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

/**************************************************************************************
* Function Header:	stringExpression()
* Purpose			Grammer production function: <string expression> nonterminal
*
* Author			Sophoan Meas
* Version			1.0
* Called Function
* Parameters		NONE
* Return value		NONE
* Algorithm			NONE
****************************************************************************************/
void relationalExpression(void) {
	switch (current.code)
	{
	case AVID_T:case FPL_T:case INL_T:
		aprimaryRelationalExpression();
		relationalOperator();
		aprimaryRelationalExpression();

		break;
	case STR_T:case SVID_T:
		sprimaryRelationalExpression();
		relationalOperator();
		sprimaryRelationalExpression();
		break;
	default:
		syn_printe();
		break;
	}
	gen_incode("PLATY: relational expression parsed");
}
void aprimaryRelationalExpression(void) {
	switch (current.code)
	{
	case AVID_T:case FPL_T:case INL_T:
		match(current.code, NO_ATTR);
		break;
	default:
		syn_printe();
		break;
	}
	gen_incode("PLATY: Primary a_relational expression parsed");
}
void relationalOperator(void) {
	switch (current.code)
	{
	case REL_OP_T:
		switch (current.attribute.get_int)
		{
		case NE:case EQ:case GT:case LT:
			match(REL_OP_T, current.attribute.get_int);
			break;
		default:
			syn_printe();
			break;
		}
		break;
	default:
		syn_printe();
		break;
	}
}
void sprimaryRelationalExpression(void) {
	primaryStringExpression();
	gen_incode("PLATY: Primary s_relational expression parsed");
}