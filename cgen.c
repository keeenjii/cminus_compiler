#include "globals.h"
#include "symtab.h"
#include "cgen.h"

void genStmt(TreeNode *tree){

  switch(tree -> kind.stmt){
    case ifK:
    break;
    case whileK:
    break;
    case assignK:
    break;
    case varK:
    break;
    case funcK:
    break;
    case callK:
    break;
    case returnK:
    break;
    case numK:
    break;
    case paramK:
    break;
    case vectStmtK:
    break;
    
  }

}

void genExp(TreeNode *tree){

  switch(tree -> kind.exp){
    case opK:
    break;
    case constK:
    break;
    case idK:
    break;
    case vectK:
    break;
    case vectIndexK:
    break;
    case typeK:
    printf("OLA CARAIO");
    break;

  }
}

static void cGen( TreeNode * tree){ 
	if (tree != NULL){ 
		switch (tree->nodekind) {
      		case statementK:
        		genStmt(tree);
        		break;
      		case expressionK:
        		genExp(tree);
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
   cGen(syntaxTree);

}

