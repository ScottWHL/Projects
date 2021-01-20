/* Filename: scanner.c
/* PURPOSE:
 *    SCANNER.C: Functions implementing a Lexical Analyzer (Scanner)
 *    as required for CST8152, Assignment #2
 *    scanner_init() must be called before using the scanner.
 *    The file is incomplete;
 *    Provided by: Svillen Ranev
 *    Version: 1.20.2
 *    Date: 24 May 2020
 *******************************************************************
 *    REPLACE THIS HEADER WITH YOUR HEADER
 *******************************************************************
 */

 /* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
  * to suppress the warnings about using "unsafe" functions like fopen()
  * and standard sting library functions defined in string.h.
  * The define does not have any effect in Borland compiler projects.
  */
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996) /* string copy warning due to visual studio */
#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */

  /*#define NDEBUG        to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */
#include "buffer.h"
#include "token.h"
#include "table.h"

#define DEBUG  /* for conditional processing */
#undef  DEBUG

/* Global objects - variables */
/* This buffer is used as a repository for string literals.
   It is defined in platy_st.c */
extern pBuffer str_LTBL; /*String literal table */
int line; /* current line number of the source code */
extern int scerrnum;     /* defined in platy_st.c - run-time error number */

/* Local(file) global objects - variables */
static pBuffer lex_buf;/*pointer to temporary lexeme buffer*/
static pBuffer sc_buf; /*pointer to input source buffer*/
/* No other global variable declarations/definitiond are allowed */

/* scanner.c static(local) function  prototypes */
static int char_class(char c); /* character class function */
static int get_next_state(int, char); /* state machine function */
static int iskeyword(char* kw_lexeme); /*keywords lookup functuion */


/*Initializes scanner */
int scanner_init(pBuffer psc_buf) {
	if (b_isempty(psc_buf)) return EXIT_FAILURE;/*1*/
	/* in case the buffer has been read previously  */
	b_rewind(psc_buf);
	b_clear(str_LTBL);
	line = 1;
	sc_buf = psc_buf;
	return EXIT_SUCCESS;/*0*/
/*   scerrnum = 0;  *//*no need - global ANSI C */
}
/*****************************************************************************************
* Function Header:  malar_next_token(void)
* Purpose			The function returns t after processing part 1(Implementation of token driven scanner)
					and part 2(Implementation of Finite State Machine (DFA)
					or Transition Table driven Scanner);
* Author			Andy Yang    WenHao Li
* Student ID#		040947248    040941575
* Version			1.0
* Called functions	b_getc(),b_markoffset(),b_compact(),b_reset(),b_alloc(),b_allocate(),b_addc(),aa_table[]()
* Parameters		NONE
* Return value		return token struct
* Algorithm			--get a char;
					--go through token driven scanner, if able to return, return token;
					--go through Finite State Machine;
					  or Transition Table driven Scanner), if able to return, return token;
*****************************************************************************************/
Token malar_next_token(void)
{
	Token t = { 0 }; /* token to return after pattern recognition. Set all structure members to 0 */
	unsigned char c; /* input symbol */
	int state = 0; /* initial state of the FSM */
	short lexstart;  /*start offset of a lexeme in the input char buffer (array) */
	short lexend;    /*end   offset of a lexeme in the input char buffer (array)*/


	char temp = 0;
	//DECLARE YOUR LOCAL VARIABLES HERE IF NEEDED   
	int next = NOAS;

	while (1) {
		c = b_getc(sc_buf);/*get one char each time*/
		if (c == SEOF) { /*if reach the end of sourcee file, set seof_t token and set attribute as SEOF_EOF, return token*/
			t.code = SEOF_T;
			t.attribute.seof = SEOF_EOF;
			return t;
		}
		else if (c == '\0') {/*if find \0, set seof_t token and set attribute as SEOF_0, return token*/
			t.code = SEOF_T;
			t.attribute.seof = SEOF_EOF;
			return t;
		}
		switch (c)/*swich to handle any other valid character*/
		{
		case ' ':
		case '\t':
		case '\v':
		case '\f':
		case '\r':
			continue;/*continue if met any space*/
		case '\n':
			line++;
			continue;/*continue and increase the line number when meet \n*/

		case '{': t.code = LBR_T; return t;/*if met left brace, set token as left brace and return token*/
		case '}': t.code = RBR_T; return t;/*if met right brace, set token as right brace and return token*/
		case '(': t.code = LPR_T; return t;/*if met left braket, set token as left braket and return token*/
		case ')': t.code = RPR_T; return t;/*if met left braket, set token as right braket and return token*/

		case '+':
			t.code = ART_OP_T; t.attribute.arr_op = PLUS; return t;/*if met plus operator, set token as arithmetic operator, set token attribute of operators as plus and return token*/
		case '-':
			t.code = ART_OP_T; t.attribute.arr_op = MINUS; return t;/*if met minus operator, set token as arithmetic operator, set token attribute of operators as minus and return token*/
		case '*':
			t.code = ART_OP_T; t.attribute.arr_op = MULT; return t;/*if met times operator, set token as arithmetic operator, set token attribute of operators as multiple and return token*/
		case '/':
			t.code = ART_OP_T; t.attribute.arr_op = DIV; return t;/*if met division operator, set token as arithmetic operator, set token attribute of operators as division and return token*/

		case ';':
			t.code = EOS_T;  return t;/*if met semicolon, set token as semicolon and return token*/
		case ',':
			t.code = COM_T; return t;/*if met comma, set token as comma and return token*/
		case '>':									/*if met greater than sign, set token as raltional operator, set token attribute as greater than sign and return token*/
			t.code = REL_OP_T; t.attribute.rel_op = GT; return t;

		case '<':
			temp = b_getc(sc_buf);
			if (temp == '>') {	/*if met less than sign, and the char after it is greater than sign,set token as raltional operator, set token attribute as not equal and return token*/
				t.code = REL_OP_T;
				t.attribute.rel_op = NE;
				return t;
			}
			if (temp == '<') {	/*if met less than sign,and the char after it is greater than sign, set token as raltional operator, set token attribute as string concatination sign and return token*/
				t.code = SCC_OP_T;
				return t;
			}
			t.code = REL_OP_T;	/*if met less than sign,and the char after it doesn't match any special case, set token as raltional operator, set token attribute as less than sign and return token*/
			t.attribute.rel_op = LT;
			b_retract(sc_buf);
			return t;
		case '=':
			if (b_getc(sc_buf) == '=') {/*if met equal sign,and the char after it is equal than sign, set token as raltional operator, set token attribute as euqal sign and return token*/
				t.code = REL_OP_T;
				t.attribute.rel_op = EQ;
				return t;
			}
			b_retract(sc_buf);/*if only met equal sign, set token as equal sign and return token*/
			t.code = ASS_OP_T;
			return t;
		case '.':
			b_markc(sc_buf, b_getcoffset(sc_buf));/*mark in case needs to retract after*/
			temp = b_getc(sc_buf);
			if (temp == 'A') {/*if met dot,and the char after it is 'A'*/
				if (b_getc(sc_buf) == 'N' && b_getc(sc_buf) == 'D' && b_getc(sc_buf) == '.') {/*if it's and operator, set token attribute as logical operator sign and return token*/
					t.code = LOG_OP_T;
					t.attribute.log_op = AND;
					return t;
				}
			}
			else if (temp == 'O') {/*if met dot,and the char after it is 'O'*/
				if (b_getc(sc_buf) == 'R' && b_getc(sc_buf) == '.') {/*if it's or operator, set token attribute as logical operator sign and return token*/
					t.code = LOG_OP_T;
					t.attribute.log_op = OR;
					return t;
				}
			}
			t.code = ERR_T;
			b_reset(sc_buf);
			t.attribute.err_lex[0] = '.';
			t.attribute.err_lex[1] = '\0';
			return t;/*otherwise, recorded error lexime, set token attribute as error token and return token*/
		case '!':/*if met exclaimation mark*/
			temp = b_getc(sc_buf);/*get the one char followed by it*/
			if (temp != '!' && temp != SEOF && temp != '\0' && temp != '\n') {/*if the following char is not exclaimation mark and it's not SEOF,not \0 and '\n,
																			  recorded first 3 characters error lexime, set token attribute as error token*/
				t.code = ERR_T;
				t.attribute.err_lex[0] = '!';
				t.attribute.err_lex[1] = temp;
				t.attribute.err_lex[2] = '\0';
				while (c != '\n') {/*read the remainning characters untill reach \n*/
					c = b_getc(sc_buf);
					if (c == SEOF || c == '\0')/*if it's SEOF or \0, break through the loop*/
					{
						b_retract(sc_buf);
						break;
					}
				}
				return t;/*return token*/
			}
			else if (temp != '!') {/*if the following char is not exclaimation mark and might be one of SEOF,\0 and '\n, set error tokan, ,
								   recorded first character and follow by '\0' as error lexime, set token attribute as error token*/
				t.code = ERR_T;
				t.attribute.err_lex[0] = '!';
				t.attribute.err_lex[1] = '\0';
			}
			c = temp;
			while (c != '\n') {/*read the remainning characters untill reach \n*/
				c = b_getc(sc_buf);
				if (c == SEOF || c == '\0')/*if it's SEOF or \0, return seof_t token*/
				{
					t.code = SEOF_T;
					return t;
				}
			}
			if (c == '\n') {/*if reach the end of the line, increase line count by 1*/
				line++;
				continue;
			}
			return t;		/*return token*/
		default:
			break;/*other situtation will get rid of switch*/
		}

		/*Finite machine, automaton*/
		if (isalnum(c) != 0 || c == '"' || c == '\0') {/*if c is space,double quotation mark, \0,
													   digits or letter */
			lexstart = b_markc(sc_buf, b_getcoffset(sc_buf) - 1);/*set lexstart to start at location of
																current char*/

			state = get_next_state(state, c);/*get next state based on the char*/
			while (1) {				/*loop through*/
				c = b_getc(sc_buf);		/*get the next char and state*/
				state = get_next_state(state, c);
				next = as_table[state];/*look through as_table based on the state and assign it to next*/
				if (next != NOAS)/*if next is not accepting state*/
					break;		/*break trough the loop*/
			}
			if (next == ASWR) {/*if next is accepting state with retract, retract one char*/
				b_retract(sc_buf);
			}
			lexend = b_getcoffset(sc_buf);/*set the end of lex*/
			lex_buf = b_allocate(lexend - lexstart, 0, 'f');
			if (lex_buf == NULL) {/*if failed to allocate a buffer for lexime, set error token
								  and record error message as runtime error,
								  set run time error flag which is scerrum, return token*/
				t.code = ERR_T;
				scerrnum = 1;
				strcpy(t.attribute.err_lex, "RUN TIME ERROR: ");
				return t;
			}

			b_reset(sc_buf);/*reset sc_buf in order to add characters to lexime buffer*/
			while (lexstart < lexend) {/*loop to add characters to lexime buffer*/
				c = b_getc(sc_buf);
				b_addc(lex_buf, c);
				lexstart++;
			}

			b_compact(lex_buf, '\0');/*compact, to add one '\0' at the end of lexime buffer*/
			b_reset(lex_buf);/*reset lexime buffer*/
			t = aa_table[state](b_location(lex_buf, lex_buf->markc_offset));/*initialize function pointer and run it, the return token will
													 be assign to t*/
			b_free(lex_buf);/*free lexime buffer*/
			return t;
		}
		t.code = ERR_T;/*if other than accept characters, will set error token and record error message, return t*/
		t.attribute.err_lex[0] = c;
		t.attribute.err_lex[1] = '\0';
		return t;
	}//end while(1)
}
int get_next_state(int state, char c)
{

	int col;
	int next;
	col = char_class(c);
	next = st_table[state][col];
#ifdef DEBUG
	printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
#endif

	assert(next != IS);

#ifdef DEBUG
	if (next == IS)
	{
		printf("Scanner Error: Illegal state:\n");
		printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
		exit(1);
	}
#endif
	return next;

}
int char_class(char c)
{
	int value;
	switch (c)
	{
	case '0':
		value = 1;
		break;
	case '.':
		value = 3;
		break;
	case '#':
		value = 4;
		break;
	case '"':
		value = 6;
		break;
	case '\0':
	case SEOF:
		value = 7;
		break;
	default:
		if (isdigit(c) ) {
			value = 2;
			break;
		}
		else if (isalpha(c)) {
			value = 0;
			break;
		}
		else {
			value = 5;
			break;
		}
	}
	return value;
}

/*****************************************************************************************
* Function Header:  iskeyword()
* Purpose			The function check the lexeme against all the keywords
* Author			Andy Yang    WenHao Li
* Student ID#		040947248    040941575
* Called functions	strncmp(), strlen()
* Parameters		char* kw_lexeme
* Return value		return int - return -1 if fail
* Algorithm			if keyword lexeme does not fail, else return -1
					compare both strings to see if they are equal, if so,
					return the keyword
*****************************************************************************************/
int iskeyword(char* kw_lexeme)
{
	/* check if the keyword_lexeme does not fail, return -1 */
	if (!kw_lexeme) {
		return RT_FAIL_1; /* return -1 */
	}

	/* loop through and compare both strings, if equal return 0 */
	for (int i = 0; i < KWT_SIZE; i++) {
		if (strcmp(kw_table[i], kw_lexeme) == ZERO)
			return i; /* return the index number of keyword */
	}
	return RT_FAIL_1;
}
/*****************************************************************************************
* Function Header:  fc02() : ACCEPTING FUNCTION (VID - AVID/KW)
* Purpose			check if the lexeme is a keyword, set the token code, attributes
					and return the token
* Author			Andy Yang    WenHao Li
* Student ID#		040947248    040941575
* Called functions	iskeyword(), strncpy()
* Parameters		char lexeme[]
* Return value		Token t
* Algorithm			-create a new token, set keywordIndex to the lexeme ID
					-first check if the lexeme is a keyword, if so return the set the token
					code and atrributes and return the token
					-Set the AVID_T Token if the lexeme is longer than VID_LEN (8)
					then copy the lexeme with the length to the token attributes and add \0,
					return the token
*****************************************************************************************/
Token fc02(char lexeme[])
{
	Token t = { 0 };
	int keywordIndex = iskeyword(lexeme); /* store the ID of the lexeme*/

	if (keywordIndex != RT_FAIL_1) { /* check the lexeme against they keyword */
		t.code = KW_T;
		t.attribute.kwt_idx = keywordIndex;
		return t; /* return the token if fail */
	}
	else
	{
		t.code = AVID_T;
	}

	if (strlen(lexeme) > VID_LEN)
	{
		for (int i = 0; i < VID_LEN; i++)
			t.attribute.vid_lex[i] = lexeme[i];

		t.attribute.vid_lex[VID_LEN] = '\0';
	}
	else
		strcpy(t.attribute.vid_lex, lexeme);

	return t;
}

/*****************************************************************************************
* Function Header:  fc03() : ACCEPTING FUNCTION (VID - SVID)
* Purpose			the character array and generate a new string variable identifier token
* Author			Andy Yang    WenHao Li
* Student ID#		040947248    040941575
* Called functions	strlen(); strncpy()
* Parameters		char lexeme[]
* Return value		Token t
* Algorithm			-set the token code to SVID, if the lexeme is greater than VID_LEN (8)
					copy the first VID_LEN -1 characters and store it into the attribute array
					vid_lex[]
					-add a @ character to the end of the string and add \0
					-if the lexeme length is less then VID_LEN, copy it into vid_lex
*****************************************************************************************/
Token fc03(char lexeme[])
{
	Token t = { 0 };
	t.code = SVID_T; /* set token code to SVID_T */

	/* if the length of lexeme is greater than variable identifier, we trim the last char
	set it to vid_lex[] */
	if (strlen(lexeme) > VID_LEN) {
		for (int i = 0; i < VID_LEN - ONE; i++)
			t.attribute.vid_lex[i] = lexeme[i];
		t.attribute.vid_lex[VID_LEN - ONE] = '#'; /* add @ to VID_LEN - 1*/
		t.attribute.vid_lex[VID_LEN] = '\0'; /* add \0 to the end */
		return t;
	}
	else
	{
		strncpy(t.attribute.vid_lex, lexeme, strlen(lexeme));
		t.attribute.vid_lex[strlen(lexeme)] = '\0';
	}
	return t;
}
/*****************************************************************************************
* Function Header:  fc05() : ACCEPTING FUNCTION integer literal decimal constant (DIL)
* Purpose			convert the lexeme representing a decimal constant to a decimal
					integer value. Return the token, else return an error token
* Author			Andy Yang    WenHao Li
* Student ID#		040947248    040941575
* Called functions	atol();
* Parameters		char lexeme[]
* Return value		Token t
* Algorithm			convert the lexeme to and store it as a long variable
					if the lexeme is within range, return the location
*****************************************************************************************/
Token fc05(char lexeme[])
{
	Token t = { 0 };
	long number = atol(lexeme); /* convert lexeme decimal integer value */

	/* check if length of lexeme is within the range */
	if (number < SHRT_MIN || number > SHRT_MAX) { /* less then 2 bytes (-32768) or greater 2 bytes */
		t = aa_table[ES](lexeme); /* error token */
		return t;
	}
	/* code and attribute for token */
	t.code = INL_T;
	t.attribute.int_value = (short)number;

	return t;
}

/*****************************************************************************************
* Function Header:  fc08() : ACCEPTING FUNCTION FOR  floating-point literal
* Purpose			convert the lexeme to a floating point value
* Author			Andy Yang    WenHao Li
* Student ID#		040947248    040941575
* Called functions	atol();
* Parameters		char lexeme[]
* Return value		Token t
* Algorithm			-convert the lexeme to a double and store it as temp
					-check if temp is within range (4 byte float)
					-if temp is within range, set the attribute and token, store it
					to flt_value, return the token
					-else return error token, error token attributes of the lexeme
*****************************************************************************************/
Token fc08(char lexeme[])
{
	Token t = { 0 };
	double temp = atof(lexeme); /* convert the lexeme and store it as a double */

	/* if the temp is >= FLT_MIN and <= FLT_MAX or when temp == 0
	set token code and attribute and convert temp to a float and store it to flt_value */
	if (temp >= FLT_MIN && temp <= FLT_MAX || temp == ZERO) {
		/* set code and attribute */
		t.code = FPL_T;
		t.attribute.flt_value = (float)temp;
		return t; /* return the token */
	}
	t = aa_table[ES](lexeme); /* else, set an error token */

	return t;
}
/**************************************************************************************
* Function Header:  fc10() : Accpeting function for the SL
* Purpose			This function is to store the lexeme into the string literal table
* Author			Andy Yang    WenHao Li
* Student ID#		040947248    040941575
* Called Function	strlen(),strcpy(), strncpy(),b_limit(), b_addc()
* Parameters		char lexeme[]
* Return value		Token t
* Algorithm			-set the str_offset by using b_limit()
					-store lexeme on the str_LTBL until lexeme - 1
					-add \0
****************************************************************************************/
Token fc10(char lexeme[]) {

	Token t = { 0 };
	t.attribute.str_offset = b_limit(str_LTBL);
	for (int i = 0; i < strlen(lexeme); i++) {
		if (lexeme[i] != '"')
			b_addc(str_LTBL, lexeme[i]);
		if (lexeme[i] == '\n')
			line++;
	}
	b_addc(str_LTBL, '\0');
	t.code = STR_T;
	return t;
}
/**************************************************************************************
* Function Header:  fc11() : ACCEPTING FUNCTION FOR THE ERROR TOKEN
* Purpose			To store the error lexeme into the err_lex
* Author			Andy Yang    WenHao Li
* Student ID#		040947248    040941575
* Called Function	strlen(),strcpy(), strncpy()
* Parameters		char lexeme[]
* Return value		Token t
* Algorithm			-get the length of lexeme,
					-if less than max err_len, simply copy the lexeme to err_lex and add \0
****************************************************************************************/
Token fc11(char lexeme[]) {

	Token t = { 0 };
	t.code = ERR_T;
	int lexLenght = strlen(lexeme);

	if (lexLenght < ERR_LEN) {


		/* when the loop is finish until the before string length of lexeme,
		store line terminator */
		strncpy(t.attribute.err_lex, lexeme, lexLenght);
		t.attribute.err_lex[lexLenght] = '\0';
	}
	else if (lexLenght >= ERR_LEN)
	{
		/* copy all strings into err_lex until 17 length */
		/* then print out three dots, and line terminator  */
		strncpy(t.attribute.err_lex, lexeme, (ERR_LEN - THREE));
		t.attribute.err_lex[ERR_LEN - THREE] = DOT;
		t.attribute.err_lex[ERR_LEN - TWO] = DOT;
		t.attribute.err_lex[ERR_LEN - ONE] = DOT;
		t.attribute.err_lex[ERR_LEN] = '\0';
	}
	for (int i = 0; i < lexLenght; i++) {
		if (lexeme[i] == '\n')
			line++;
	}
	return t;
}
Token aa_func12(char lexeme[])
{
	Token t = { 0 }; /* Token to be returned */
	t = aa_table[ES](lexeme);
	return t;
}