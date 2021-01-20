#ifdef PARSER_H
#define PARSER_H_
#endif

#define DEBUG /* for debuggnig*/
#undef DEBUG

#include "token.h"
#include "buffer.h"

#define NO_ATTR -1
#define ELSE 0
#define	FALSE 1
#define IF 2
#define	PLATYPUS 3
#define READ 4
#define REPEAT 5
#define	THEN 6
#define	TRUE 7
#define	WHILE 8
#define WRITE 9

static Token current;
int synerrno;
extern Token malar_next_token();
extern int line;
extern char* kw_table[];
extern Buffer* str_LTBL;
void parser(void);
void match(int pr_token_code, int pr_token_attribute);
void syn_eh(int sync_token_code);
void syn_printe(void);
void gen_incode(char* string);

void program(void);
void optStatements(void);
void statements(void);
void statement(void);
void statements_p(void);
void assignmentStatement(void);
void assignmentExpression(void);
void selectionStatement(void);
void iterationStatement(void);
void preCondition(void);
void inputStatement(void);
void variableList(void);
void variableList_p(void);
void variableIdentifier(void);
void outputStatement(void);
void outputList(void);
void arithmeticExpression(void);
void unaryArithmeticExpression(void);
void additiveArithmeticExpression(void);
void additiveArithmeticExpression_p(void);
void multiplicativeArithmeticExpression(void);
void multiplicativeArithmeticExpression_p(void);
void primaryArithmeticExpression(void);
void stringExpression(void);
void stringExpression_p(void);
void primaryStringExpression(void);
void conditionalExpression(void);
void ORlogicalExpression(void);
void ORlogicalExpression_p(void);
void ANDlogicalExpression(void);
void ANDlogicalExpression_p(void);
void relationalExpression(void);
void aprimaryRelationalExpression(void);
void sprimaryRelationalExpression(void);
void relationalOperator(void);