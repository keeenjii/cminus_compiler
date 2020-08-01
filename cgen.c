#include "globals.h"
#include "symtab.h"
#include "code.h"
#include "cgen.h"


static void cGen( TreeNode * tree){ 
	if (tree != NULL){ 
		switch (tree->nodekind) {
      		case statementK:
        		//genStmt(tree);
        		break;
      		case expressionK:
        		//genExp(tree);
        		break;
      		default:
        		break;
    }
    cGen(tree->sibling);
  }
}

void codeGen(TreeNode * syntaxTree, char * codefile)
{  char * s = malloc(strlen(codefile)+7);
   strcpy(s,"File: ");
   strcat(s,codefile);
   emitComment("TINY Compilation to TM Code");
   emitComment(s);
   /* generate standard prelude */
   emitComment("Standard prelude:");
   emitRM("LD",mp,0,ac,"load maxaddress from location 0");
   emitRM("ST",ac,0,ac,"clear location 0");
   emitComment("End of standard prelude.");
   /* generate code for TINY program */
   cGen(syntaxTree);
   /* finish */
   emitComment("End of execution.");
   
}