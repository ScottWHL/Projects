/* Filename: table.h
 * Transition Table and function declarations necessary for the scanner implementation
 * as required for CST8152 - Assignment #2.
 * Version: 1.20.2
 * Date: 24 May 2020
 * Provided by: Svillen Ranev
 * The file is incomplete. You are to complete it.
 ***************************************************
 * REPLACE THIS HEADER WITH YOUR HEADER
 ***************************************************
 */

#ifndef  TABLE_H_
#define  TABLE_H_ 

#ifndef BUFFER_H_
#include "buffer.h"
#endif


#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

 /*   Source end-of-file (SEOF) sentinel symbol
  *    '\0' or one of 255,0xFF,EOF
  */
#define SEOF 0xFF /* \0 */
#define EOL 0x0a  /* new line */
#define DOT 0x2e /* . period */
#define ZERO 0 /* zero */
#define ONE 1
#define TWO 2
#define THREE 3
  /*  Special case tokens processed separately one by one
   *  in the token-driven part of the scanner
   *  '=' , ' ' , '(' , ')' , '{' , '}' , == , <> , '>' , '<' , ';',
   *  white space
   *  !!comment , ',' , ';' , '-' , '+' , '*' , '/', ## ,
   *  .AND., .OR. , SEOF,
   */

   //PLACE YOUR CONSTANT DEFINITIONS HERE IF YOU NEED ANY

   //REPLACE *ESN* and *ESR* WITH YOUR ERROR STATE NUMBER 
#define ES  11 /* Error state  with no retract */
#define ER  12 /* Error state  with retract */
#define IS -1    /* Inavalid state */

/* State transition table definition */

//REPLACE *CN* WITH YOUR COLUMN NUMBER  

#define TABLE_COLUMNS 8
/*transition table - type of states defined in separate table */
int  st_table[][TABLE_COLUMNS] = {
	/* State 0 */  {  1,  6,  4,  ES, ES, ES, 9,  ES,}, /* NOAS - not accepting state */

	/* State 1 */  {  1,  1,  1,  2,  3,  2,  2,   2,}, /* NOAS - not accepting state */

	/* State 2 */  {  IS, IS, IS, IS, IS, IS, IS, IS}, /* AVID/KW ASWR - arithmetic variable identifiers - accepting state with retract */

	/* State 3 */  {  IS, IS, IS, IS, IS, IS, IS, IS}, /* SVID ASNR - string variable identifiers - accepting state no retract */

	/* State 4 */  {  ES, 4,  4,  7,  5,  5,  5,   5}, /* NOAS - not accepting state */

	/* State 5 */  {  IS, IS, IS, IS, IS, IS, IS, IS}, /* DIL ASWR - decimal literal - accepting state with retract*/

	/* State 6 */  {  ES, 6,  ES, 7,  ES,  5, 5,   5}, /* NOAS - not accepting state */

	/* State 7 */  {  8,  7,  7,  8,  8,  8,  8,   8}, /* FPL ASWR - floating-point literal - accepting state with retract */

	/* State 8 */  {  IS, IS, IS, IS, IS, IS, IS,  IS}, /* NOAS - not accepting state */

	/* State 9 */  {  9,  9,  9,  9,  9,  9,  10,  ES},  /* SL - string literal */

	/* State 10 */ {  IS, IS, IS, IS, IS, IS, IS,  IS}, /* ES ASNR - Error state - accepting state no retract*/

	/* State 11 */ {  IS, IS, IS, IS, IS, IS, IS,  IS}, /* ER ASWR - Error state wiht retract */

	/* State 12 */ {  IS, IS, IS, IS, IS, IS, IS,  IS}  /* NOAS - not accepting state */
}; //YOUR TABLE INITIALIZATION HERE



/* Accepting state table definition */
//REPLACE *N1*, *N2*, and *N3* WITH YOUR NUMBERS
#define ASWR     0  /* accepting state with retract */
#define ASNR     1  /* accepting state with no retract */
#define NOAS     2  /* not accepting state */

int as_table[] = {
	/*state 0 */	NOAS, /* Not Accepting State */
	/*state 1 */	NOAS, /* Not Accepting State */
	/*state 2 */	ASWR, /* Accepting State With Retract */
	/*state 3 */	ASNR, /* Accepting State No Retract */
	/*state 4 */	NOAS, /* Not Accepting State */
	/*state 5 */	ASWR, /* Accepting State With Retract */
	/*state 6 */	NOAS, /* Not Accepting State */
	/*state 7 */	NOAS, /* Not Accepting State */
	/*state 8 */	ASWR, /* Accepting State With Retract */
	/*state 9 */	NOAS, /* Not Accepting State */
	/*state 10 */	ASNR, /* Accepting State No Retract */
	/*state 11 */	ASNR, /* Accepting State No Retract */
	/*state 12 */	ASWR  /* Accepting State With Retract */
};

/* Accepting action function declarations */
Token fc02(char lexeme[]); /* AVID/KW - ASWR */
Token fc03(char lexeme[]); /* SVID - ASNR */
Token fc05(char lexeme[]); /* DIL - ASWR */
Token fc08(char lexeme[]); /* FPL - ASWR */
Token fc10(char lexeme[]); /* ASNR */
Token fc11(char lexeme[]); /* ES - Error state no retract */

/*FOR EACH OF YOUR ACCEPTING STATES YOU MUST PROVIDE
ONE FUNCTION PROTOTYPE. THEY ALL RETURN Token AND TAKE
ONE ARGUMENT: A string REPRESENTING A TOKEN LEXEME. */

//Token aa_funcXX(char *lexeme); 
//
//Replace XX with the number of the accepting state: 02, 03 and so on.

/* defining a new type: pointer to function (of one char * argument)
   returning Token
*/

typedef Token(*PTR_AAF)(char* lexeme);


/* Accepting function (action) callback table (array) definition */
/* If you do not want to use the typedef, the equvalent declaration is:
 * Token (*aa_table[])(char lexeme[]) = {
 */

PTR_AAF aa_table[] = {
						NULL,	/* state 0 */
						NULL,	/* state 1 */
						fc02,	/* state 2 */
						fc03,	/* state 3 */
						NULL,	/* state 4 */
						fc05,	/* state 5 */
						NULL,	/* state 6 */
						NULL,	/* state 7 */
						fc08,	/* state 8 */
						NULL,	/* state 9 */
						fc10,	/* state 10 */
						fc11,	/* state 11 */
						fc11,	/* state 12 */
};

/* Keyword lookup table (.AND. and .OR. are not keywords) */

#define KWT_SIZE  10

char* kw_table[] =
{
"ELSE",
"FALSE",
"IF",
"PLATYPUS",
"READ",
"REPEAT",
"THEN",
"TRUE",
"WHILE",
"WRITE"
};

#endif
