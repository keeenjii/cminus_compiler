#include "globals.h"
#include "symtab.h"
#include "cgen.h"
#include "intercodeglobals.h"

int location = 1;
int tempReg = 0;

static void cGen( TreeNode * tree, THead* intercode);

void genStmt(TreeNode *tree, THead* intercode){

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

void genExp(TreeNode *tree, THead *intercode){

  TreeNode *aux1, *aux2, *aux3;

  switch(tree -> kind.exp){
    case opK:
    break;
    case constK:
    break;
    case idK:
    break;
    case vectK:
    break;

    
    case typeK:
    aux1 = tree -> child[0];
    cGen(aux1, intercode);
    printf("Declaração de tipo\n");
    break;

  }
}

static void cGen( TreeNode * tree, THead* intercode){ 
	if (tree != NULL){ 
		switch (tree->nodekind) {
      		case statementK:
        		genStmt(tree, intercode);
        		break;
      		case expressionK:
        		genExp(tree, intercode);
        		break;
      		default:
        		break;
    }
    cGen(tree->sibling, intercode);
  }
}

void codeGen(TreeNode * syntaxTree, char * codefile)
{  char * s = malloc(strlen(codefile)+7);
   strcpy(s,"File: ");
   strcat(s,codefile);
   THead * intercodeTAD = initLista();
   cGen(syntaxTree, intercodeTAD);

}

