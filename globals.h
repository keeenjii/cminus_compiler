//Estrutura de Dados de uma arvore para implementar a arvore de análise sintática

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef YYPARSER

#include "scanner.tab.h"

#define ENDFILE 0

#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define MAXRESERVED 6
//número de palavras reservadas

typedef int TokenType;

extern FILE* source; 
extern FILE* listing; 
extern FILE* code; 

extern int  lineno; 
typedef enum {
	statementK,expressionK
	} NodeKind;
	
typedef enum {
	ifK, whileK, assignK, varK, funcK, callK, returnK, numK, paramK, vectStmtK, paramVectK
	} StmtKind;
	
typedef enum {
	opK,constK,idK, vectK, vectIndexK,typeK,
	} ExpKind;

typedef enum {
	voidt,integert,booleant
	} ExpType;

#define MAXCHILDREN 3
//maximo de filhos

typedef struct treeNode
   { struct treeNode * child[MAXCHILDREN];
     struct treeNode * sibling;
     int lineno;
     NodeKind nodekind;
     union 	{ 
		 		StmtKind stmt; 
				ExpKind exp;
			} kind;

     struct	{	
		 		TokenType op;
				int  val;
        		int  len;
        		char* name; 
        		char* scope;	
			} attr;
     ExpType type; /* for type checking of exps */
   } TreeNode;

/**************************************************/
/***********   Flags for tracing       ************/
/**************************************************/

/* EchoSource = TRUE causes the source program to
 * be echoed to the listing file with line numbers
 * during parsing
 */
extern int EchoSource;

/* TraceScan = TRUE causes token information to be
 * printed to the listing file as each token is
 * recognized by the scanner
 */
extern int TraceScan;

/* TraceParse = TRUE causes the syntax tree to be
 * printed to the listing file in linearized form
 * (using indents for children)
 */
extern int TraceParse;

/* TraceAnalyze = TRUE causes symbol table inserts
 * and lookups to be reported to the listing file
 */
extern int TraceAnalyze;

/* TraceCode = TRUE causes comments to be written
 * to the TM code file as code is generated
 */
extern int TraceCode;

/* Error = TRUE prevents further passes if an error occurs */
int Error; 
#endif

// FINALIZADO - AUGUSTO