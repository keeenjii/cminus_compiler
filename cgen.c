#include "globals.h"
#include "symtab.h"
#include "cgen.h"
#include "intercodeglobals.h"
#include "ass.h"
#include "assGen.h"


int location = 1;
int tempReg = 0;
int numArgs = 0;
int labelCont = 0;

storeInfo info;

static char* cGen( TreeNode * tree, THead* intercode, int argFlag, int assignFlag);

void thrownArgOp(char *reg, char *scope, THead *intercode, int location, int eh_vetor, int vecOffset, char *varname){
  Address ad1, ad2, ad3;
  ad1 = initAddress(labelA, 0, reg, scope);
  if(eh_vetor == 0){
    ad2 = initAddress(nop, 0, NULL, NULL);
    ad3 = initAddress(nop, 0, NULL, NULL);  
  }
  else{
    int a=1;
    if(st_lookup_offset(varname, "global") != -1)
      a = 0;
    ad2 = initAddress(numA, a,  NULL, NULL);
    ad3 = initAddress(numA, vecOffset, NULL, NULL);
  }

  insereLista(intercode, ad1, ad2, ad3, argOp, location);
  location++;
}

char* genStmt(TreeNode *tree, THead* intercode, int argFlag){

  TreeNode *aux1, *aux2, *aux3;
  Address addr1, addr2, addr3;
  int savedLoc1, savedLoc2;

  switch(tree -> kind.stmt){
    case ifK:
      aux1 = tree -> child[0];
      aux2 = tree -> child[1];
      aux3 = tree -> child[2];

      char *regIf = cGen(aux1, intercode, 0, 0);

      addr1 = initAddress(labelA, 0, regIf, tree -> attr.scope);
      addr2 = initAddress(nop, 0, NULL, NULL);
      addr3 = initAddress(nop, 0, NULL, NULL);
      insereLista(intercode, addr1, addr2, addr3, iffOp, location);
      savedLoc1 = location;
      location++;

      cGen(aux2, intercode, 0, 0);

      if(aux3 == NULL){

        char *label1 = (char*)malloc(7*sizeof(char));
        sprintf(label1, "label%d", labelCont);
        labelCont++;
        pesquisaLista(intercode, savedLoc1) -> addr2 = initAddress(labelA, 0, label1, tree -> attr.scope);
        addr1 = initAddress(labelA, 0, label1, tree -> attr.scope);
        addr2 = initAddress(nop, 0, NULL, NULL);
        addr3 = initAddress(nop, 0, NULL, NULL);
        insereLista(intercode, addr1, addr2, addr3, labOp, location);
        location++;


      }else{
        addr1 = initAddress(nop, 0, NULL, NULL);
        addr2 = initAddress(nop, 0, NULL, NULL);
        addr3 = initAddress(nop, 0, NULL, NULL);
        insereLista(intercode, addr1, addr2, addr3, gotoOp, location);
        savedLoc2 = location;
        location++;

        char *label2 = (char*)malloc(7*sizeof(char));
        sprintf(label2, "label%d", labelCont);
        labelCont++;
        pesquisaLista(intercode, savedLoc1) -> addr2 = initAddress(labelA, 0, label2, tree -> attr.scope);

        addr1 = initAddress(labelA, 0, label2, tree -> attr.scope);
        addr2 = initAddress(nop, 0, NULL, NULL);
        addr3 = initAddress(nop, 0, NULL, NULL);
        insereLista(intercode, addr1, addr2, addr3, labOp, location);
        location++;

        cGen(aux3, intercode, 0, 0);
        char *label3 = (char*)malloc(7*sizeof(char));
        sprintf(label3, "label%d", labelCont);
        labelCont++;
        pesquisaLista(intercode, savedLoc2) -> addr1 = initAddress(labelA, 0, label3, tree -> attr.scope);

        addr1 = initAddress(labelA, 0, label3, tree -> attr.scope);
        addr2 = initAddress(nop, 0, NULL, NULL);
        addr3 = initAddress(nop, 0, NULL, NULL);
        insereLista(intercode, addr1, addr2, addr3, labOp, location);
        location++;
      
      }

    break;
    case whileK:
      aux1 = tree -> child[0];
      aux2 = tree -> child[1];

      addr1 = initAddress(nop, 0, NULL, NULL);
      addr2 = initAddress(nop, 0, NULL, NULL);
      addr3 = initAddress(nop, 0, NULL, NULL);
      insereLista(intercode, addr1, addr2, addr3, labOp, location);
      savedLoc1 = location;
      location++;

      char *regLab = cGen(aux1, intercode, 0, 0);

      addr1 = initAddress(labelA, 0, regLab, tree -> attr.scope);
      addr2 = initAddress(nop, 0, NULL, NULL);
      addr3 = initAddress(nop, 0, NULL, NULL);
      insereLista(intercode, addr1, addr2, addr3, iffOp, location);
      savedLoc2 = location;
      location++;

      cGen(aux2, intercode, 0, 0);

      char *label8 = (char*)malloc(7*sizeof(char));
      sprintf(label8, "label%d", labelCont);
      labelCont++;
      pesquisaLista(intercode, savedLoc1) -> addr1 = initAddress(labelA, 0, label8, tree -> attr.scope);

      addr1 = initAddress(labelA, 0, label8, tree -> attr.scope);
      addr2 = initAddress(nop, 0, NULL, NULL);
      addr3 = initAddress(nop, 0, NULL, NULL);
      insereLista(intercode, addr1, addr2, addr3, gotoOp, location);
      location++;

      char *label9 = (char*)malloc(7*sizeof(char));
      sprintf(label9, "label%d", labelCont);
      labelCont++;
      pesquisaLista(intercode, savedLoc2) -> addr2 = initAddress(labelA, 0, label9, tree -> attr.scope);

      addr1 = initAddress(labelA, 0, label9, tree -> attr.scope);
      addr2 = initAddress(nop, 0, NULL, NULL);
      addr3 = initAddress(nop, 0, NULL, NULL);
      insereLista(intercode, addr1, addr2, addr3, labOp, location);
      location++;

    break;
    case assignK:

      aux1 = tree -> child[0];
      aux2 = tree -> child[1];
      char *assign1 = cGen(aux1, intercode, 0, 1);
      char *assign2 = cGen(aux2, intercode, 0, 0);

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

  
    

    break;
    case varK: ;

      addr1 = initAddress(labelA, 0, tree -> attr.name, tree -> attr.scope);
      addr2 = initAddress(labelA, 0, tree -> attr.scope, tree -> attr.scope);
      addr3 = initAddress(numA, tree -> attr.len, NULL, NULL);

      insereLista(intercode, addr1, addr2, addr3, allocOp, location);
      location++;

      
    break;
    case funcK:
      aux1 = tree -> child[0];
      aux2 = tree -> child[1];
      char *typ = (char*)malloc(4*sizeof(char));

      if(tree -> type == integert){
        sprintf(typ, "int");
      }else {
        sprintf(typ, "void");
      }

      addr1 = initAddress(labelA, 0, typ, tree -> attr.scope);
      addr2 = initAddress(labelA, 0, tree -> attr.name, tree -> attr.scope);
      addr3 = initAddress(nop, 0, NULL, NULL);

      insereLista(intercode, addr1, addr2, addr3, funOp, location);
      location++;
      cGen(aux1, intercode, 0, 0);
      cGen(aux2, intercode, 0, 0);

      addr1 = initAddress(labelA, 0, tree -> attr.name, tree -> attr.scope);
      addr2 = initAddress(nop, 0, NULL, NULL);
      insereLista(intercode, addr1, addr2, addr2, endOp, location);
      location++;

    break;
    case callK:
      aux1 = tree -> child[0];
      cGen(aux1, intercode, 1, 0);
      
      char *rot = (char*)malloc(5*sizeof(char));
      sprintf(rot, "$t%d", tempReg);
      tempReg++;

      addr1 = initAddress(labelA, 0, rot, tree -> attr.scope);
      addr2 = initAddress(labelA, 0, tree -> attr.name, tree -> attr.scope);
      addr3 = initAddress(numA, numArgs, NULL, NULL);
      numArgs = 0;
      Opkind op;
      if(strcmp(tree -> attr.name, "output") == 0)
        op = outputOp;
      else if(strcmp(tree -> attr.name, "input") == 0)
        op = inputOp;
      else 
        op = callOp;
      insereLista(intercode, addr1, addr2, addr3, op, location);
      location++;

      if(argFlag == 1){
       thrownArgOp(rot, tree -> attr.scope, intercode, location, 0, 0, tree -> attr.name);
        numArgs++;
        }      

      return rot;

    break;
    case returnK: ;
      char* retExp;
      aux1 = tree -> child[0];
      if(aux1 == NULL){
        addr1 = initAddress(nop, 0, NULL, NULL);
        addr2 = initAddress(nop, 0, NULL, NULL);
        addr3 = initAddress(nop, 0, NULL, NULL);
        
        insereLista(intercode, addr1, addr2, addr3, retOp, location);
        location++;
      } else {
        retExp = cGen(aux1, intercode, 0, 0);
        addr1 = initAddress(labelA, 0, retExp, tree -> attr.scope);
        addr2 = initAddress(nop, 0, NULL, NULL);
        addr3 = initAddress(nop, 0, NULL, NULL);

        insereLista(intercode, addr1, addr2, addr3, retOp, location);
        location++;
      }
      
    break;
    case paramK: ;
      char *typP = (char*)malloc(4*sizeof(char));

      if(tree -> type == integert){
        sprintf(typP, "int");
      }else {
        sprintf(typP, "void");
      }
      addr1 = initAddress(labelA, 0, typP, tree -> attr.scope);
      addr2 = initAddress(labelA, 0, tree -> attr.name, tree -> attr.scope);
      addr3 = initAddress(labelA, 0, tree -> attr.scope, tree -> attr.scope);

      insereLista(intercode, addr1, addr2, addr3, paramOp, location);
      location++;


      if(argFlag == 1){
          thrownArgOp(rot, tree -> attr.scope, intercode, location, 0, 0, tree -> attr.name);
          numArgs++;
        }      

    break;
    case vectStmtK: ;

      addr1 = initAddress(labelA, 0, tree -> attr.name, tree -> attr.scope);
      addr2 = initAddress(labelA, 0, tree -> attr.scope, tree -> attr.scope);
      addr3 = initAddress(numA, tree -> attr.len, NULL, NULL);

      insereLista(intercode, addr1, addr2, addr3, allocOp, location);
      location++;
    break;
    case paramVectK: ;
      char *typPv = (char*)malloc(4*sizeof(char));
      sprintf(typPv, "vector");
      
      addr1 = initAddress(labelA, 0, typPv, tree -> attr.scope);
      addr2 = initAddress(labelA, 0, tree -> attr.name, tree -> attr.scope);
      addr3 = initAddress(labelA, 0, tree -> attr.scope, tree -> attr.scope);

      insereLista(intercode, addr1, addr2, addr3, paramOp, location);
      location++;


      if(argFlag == 1){
          thrownArgOp(rot, tree -> attr.scope, intercode, location, 0, 0, tree -> attr.name);
          numArgs++;
        }    
    break;
  }
  return NULL;
}

char* genExp(TreeNode *tree, THead *intercode, int argFlag, int assignFlag){ //  a função retorna o ultimo registrador temporário utilizado ou NULL se nao utilizar

  Address addr1, addr2, addr3;
  TreeNode *aux1, *aux2, *aux3;

  switch(tree -> kind.exp){
    case opK:

    aux1 = tree -> child[0];
    aux2 = tree -> child[1];

    char * reg1 = cGen(aux1, intercode, 0, 0);
    char * reg2 = cGen(aux2, intercode, 0, 0);
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
    if(argFlag == 1){
        thrownArgOp(reg3, tree -> attr.scope, intercode, location, 0, 0, tree -> attr.name);
        numArgs++;
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
      if(argFlag == 1){
        thrownArgOp(reg4, tree -> attr.scope, intercode, location, 0, 0, tree -> attr.name);
        numArgs++;
      }

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
      if(assignFlag == 1){
        info.var = tree -> attr.name;
        info.regDesloc = "$zero";
      }
      if(argFlag == 1){
        int aux = 0;
        if(st_lookup_size(tree -> attr.name, "global") > 1 || 
            st_lookup_size(tree -> attr.name, tree -> attr.scope) > 1 ||
              strcmp(st_lookup_type(tree -> attr.name, tree -> attr.scope), "param pointer") == 0){
          aux = 1;
        }
        
        
        thrownArgOp(reg, tree -> attr.scope, intercode, location, 
          aux, 
            st_lookup_offset(tree -> attr.name, tree -> attr.scope) == -1 ? st_lookup_offset(tree -> attr.name, "global"): st_lookup_offset(tree -> attr.name, tree -> attr.scope), tree -> attr.name);
        numArgs++;
        
      }
      return reg;
    break;

    case vectK:
      aux1 = tree -> child[0];
      char *regt = cGen(aux1, intercode, 0, 0);
      char *regVec = (char*)malloc(5*sizeof(char));
      sprintf(regVec, "$t%d", tempReg);
      tempReg++;

      addr1 = initAddress(labelA, 0, regVec, tree -> attr.scope);
      addr2 = initAddress(labelA, 0, tree -> attr.name, tree -> attr.scope);
      addr3 = initAddress(labelA, 0, regt, tree -> attr.scope);

      insereLista(intercode, addr1, addr2, addr3, loadOp, location);
      location++;
      if(assignFlag == 1){
        info.var = tree -> attr.name;
        info.regDesloc = regt;
      }
      if(argFlag == 1){
        thrownArgOp(regVec, tree -> attr.scope, intercode, location, 0, 0, tree -> attr.name);
        numArgs++;
      }
      return regVec;

    break;

    case typeK:
    aux1 = tree -> child[0];
    cGen(aux1, intercode, 0, 0);
    break;
  }
  return NULL;
}

static char* cGen( TreeNode * tree, THead* intercode, int argFlag, int assignFlag){ 
	if (tree != NULL){ 
    char * lastRegt;
		switch (tree->nodekind) {
      		case statementK:
            lastRegt = genStmt(tree, intercode, argFlag);            
        		break;
      		case expressionK:
        		lastRegt = genExp(tree, intercode, argFlag, assignFlag);
        		break;
      		default:
        		break;
    }
    cGen(tree->sibling, intercode, argFlag, assignFlag);
    return lastRegt;
  }
}

void codeGen(TreeNode * syntaxTree)
{ 
  THead * intercodeTAD = initLista();
  cGen(syntaxTree, intercodeTAD, 0, 0);
  printIntercode(intercodeTAD);
  printf("\nCodigo assembly gravado em asscode.txt\n");
  assHead* assembly = initAss();
  assGen(assembly, intercodeTAD);
  printAss(assembly); 

}

