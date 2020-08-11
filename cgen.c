#include "globals.h"
#include "symtab.h"
#include "cgen.h"
#include "intercodeglobals.h"

int location = 1;
int tempReg = 0;

storeInfo info;

static char* cGen( TreeNode * tree, THead* intercode);

void genStmt(TreeNode *tree, THead* intercode){

  TreeNode *aux1, *aux2, *aux3;
  Address addr1, addr2, addr3;

  switch(tree -> kind.stmt){
    case ifK:
    break;
    case whileK:
    break;
    case assignK:

      aux1 = tree -> child[0];
      aux2 = tree -> child[1];
      char *assign1 = cGen(aux1, intercode);
      char *assign2 = cGen(aux2, intercode);

      addr1 = initAddress(labelA, 0, assign1, tree -> attr.scope);
      addr2 = initAddress(labelA, 0, assign2, tree -> attr.scope);
      addr3 = initAddress(nop, 0, NULL, NULL);

      insereLista(intercode, addr1, addr2, addr3, assignOp, location);
      location++;

      addr1 = initAddress(labelA, 0, info.var, tree -> attr.scope);
      addr2 = initAddress(labelA, 0, assign1, tree -> attr.scope);
      addr3 = initAddress(labelA, 0, info.regDesloc, tree -> attr.scope);

      insereLista(intercode, addr1, addr2, addr3, storeOp, location);
      location++;

      tempReg = 0;
    

    break;
    case varK: ;

      addr1 = initAddress(labelA, 0, tree -> attr.name, tree -> attr.scope);
      addr2 = initAddress(labelA, 0, tree -> attr.scope, tree -> attr.scope);
      addr3 = initAddress(numA, tree -> attr.len, NULL, NULL);

      insereLista(intercode, addr1, addr2, addr3, allocOp, location);
      location++;

      
    break;
    case funcK:
      aux2 = tree -> child[1];
      cGen(aux2, intercode);
    break;
    case callK:

    break;
    case returnK:
    break;
    case paramK:
    break;
    case vectStmtK: ;

      addr1 = initAddress(labelA, 0, tree -> attr.name, tree -> attr.scope);
      addr2 = initAddress(labelA, 0, tree -> attr.scope, tree -> attr.scope);
      addr3 = initAddress(numA, tree -> attr.len, NULL, NULL);

      insereLista(intercode, addr1, addr2, addr3, allocOp, location);
      location++;
    break;
    
  }

}

char* genExp(TreeNode *tree, THead *intercode){ //  a função retorna o ultimo registrador temporário utilizado ou NULL se nao utilizar

  Address addr1, addr2, addr3;
  TreeNode *aux1, *aux2, *aux3;

  switch(tree -> kind.exp){
    case opK:

    aux1 = tree -> child[0];
    aux2 = tree -> child[1];

    char * reg1 = cGen(aux1, intercode);
    char * reg2 = cGen(aux2, intercode);
    char *reg3 = (char*)malloc(5*sizeof(char));
    sprintf(reg3, "$t%d", tempReg);
    tempReg++;
    addr1 = initAddress(labelA, 0, reg3, tree -> attr.scope);
    addr2 = initAddress(labelA, 0, reg1, tree -> attr.scope);
    addr3 = initAddress(labelA, 0, reg2, tree -> attr.scope);

    switch(tree -> attr.op){
      case SOM:
      insereLista(intercode, addr1, addr2, addr3, addOp, location);
      location++;
      break;

      case SUB:
      insereLista(intercode, addr1, addr2, addr3, subOp, location);
      location++;
      break;

      case MUL:
      insereLista(intercode, addr1, addr2, addr3, multOp, location);
      location++;
      break;

      case DIV:
      insereLista(intercode, addr1, addr2, addr3, divOp, location);
      location++;
      break;

      case MENOR:
      insereLista(intercode, addr1, addr2, addr3, ltOp, location);
      location++;
      break;

      case MENORIGUAL:
      insereLista(intercode, addr1, addr2, addr3, letOp, location);
      location++;
      break;

      case MAIOR:
      insereLista(intercode, addr1, addr2, addr3, gtOp, location);
      location++;
      break;

      case MAIORIGUAL:
      insereLista(intercode, addr1, addr2, addr3, getOp, location);
      location++;
      break;

      case IGUALIGUAL:
      insereLista(intercode, addr1, addr2, addr3, eqOp, location);
      location++;
      break;

      case DIFERENTE:
      insereLista(intercode, addr1, addr2, addr3, noteqOp, location);
      location++;
      break;
    }
    return reg3;
    break;

    case constK: ;
      char *reg4 = (char*)malloc(5*sizeof(char));
      sprintf(reg4, "$t%d", tempReg);
      tempReg++;

      addr1 = initAddress(labelA, 0, reg4, tree -> attr.scope);
      addr2 = initAddress(numA, tree -> attr.val, NULL, NULL);
      addr3 = initAddress(nop, 0, NULL, NULL);

      insereLista(intercode, addr1, addr2, addr3, immedOp, location);
      location++;
      return reg4;

    break;

    case idK: ;
      char *reg = (char*)malloc(5*sizeof(char));
      sprintf(reg, "$t%d", tempReg);
      tempReg++;
      addr1 = initAddress(labelA, 0, reg, tree -> attr.scope);
      addr2 = initAddress(labelA, 0, tree -> attr.name, tree -> attr.scope);
      addr3 = initAddress(nop, 0, NULL, NULL);

      insereLista(intercode, addr1, addr2, addr3, loadOp, location);
      location++;

      info.var = tree -> attr.name;
      info.regDesloc = "$zero";

      return reg;
    break;

    case vectK:
      aux1 = tree -> child[0];
      char *regt = cGen(aux1, intercode);
      char *regVec = (char*)malloc(5*sizeof(char));
      sprintf(regVec, "$t%d", tempReg);
      tempReg++;

      addr1 = initAddress(labelA, 0, regVec, tree -> attr.scope);
      addr2 = initAddress(labelA, 0, tree -> attr.name, tree -> attr.scope);
      addr3 = initAddress(labelA, 0, regt, tree -> attr.scope);

      insereLista(intercode, addr1, addr2, addr3, loadOp, location);
      location++;

      info.var = tree -> attr.name;
      info.regDesloc = regt;

      return regVec;

    break;

    case typeK:
    aux1 = tree -> child[0];
    cGen(aux1, intercode);
    break;
  }
  return NULL;
}

static char* cGen( TreeNode * tree, THead* intercode){ 
	if (tree != NULL){ 
    char * lastRegt;
		switch (tree->nodekind) {
      		case statementK:
            genStmt(tree, intercode);            
        		break;
      		case expressionK:
        		lastRegt = genExp(tree, intercode);
        		break;
      		default:
        		break;
    }
    cGen(tree->sibling, intercode);
    return lastRegt;
  }
}

void codeGen(TreeNode * syntaxTree, char * codefile)
{  char * s = malloc(strlen(codefile)+7);
   strcpy(s,"File: ");
   strcat(s,codefile);
   THead * intercodeTAD = initLista();
   cGen(syntaxTree, intercodeTAD);
   printIntercode(intercodeTAD);

}

