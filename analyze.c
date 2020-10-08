#include "globals.h"
#include "symtab.h"
#include "analyze.h"

static int location = 0;
static int globalLoc = 0;
char *lastScope = "global";
int verificaGlobal;

static void typeError(TreeNode * t, char * message)
{ fprintf(listing,"Erro de tipo em %s na linha %d: %s.\n",t->attr.name, t->lineno,message);
  Error = TRUE;
}


static void insertNode( TreeNode * t)
{ 
	verificaGlobal = strcmp(t -> attr.scope, "global");
	if(strcmp(t -> attr.scope, lastScope) != 0){
		location = 0;
		lastScope = t -> attr.scope;
	}
	switch (t->nodekind)
    { 

		case statementK:
      	switch (t->kind.stmt)
        { 	case paramK:

          		if (st_lookup(t->attr.name, t->attr.scope,"variable") == -1 && st_lookup(t->attr.name, "global","variable") == -1)
            		st_insert(t->attr.name,t->lineno, verificaGlobal == 0 ? globalLoc++ : location++, t->attr.scope, "variable", (t->type ==1) ? "integer":"void", 1);
          		else
            		typeError(t,"Declaração Inválida. Parâmetro já declarado.");	
            break;
			case paramVectK:
				if (st_lookup(t->attr.name, t->attr.scope,"vector") == -1 && st_lookup(t->attr.name, "global","vector") == -1)
            		st_insert(t->attr.name,t->lineno,verificaGlobal == 0 ? globalLoc++ : location++, t->attr.scope, "vector", "param pointer", 1);
          		else
            		typeError(t,"Declaração Inválida. Parâmetro já declarado.");	
			break;
			case vectStmtK:
			if (st_lookup(t->attr.name, t->attr.scope,"vector") == -1 && st_lookup(t->attr.name, "global","vector") == -1){
				if(t -> attr.len != 0){
            		st_insert(t->attr.name,t->lineno,verificaGlobal == 0 ? globalLoc : location, t->attr.scope, "vector", "pointer", t -> attr.len);		
					if(verificaGlobal == 0){
						globalLoc += t -> attr.len;
					}else {
						location += t -> attr.len;
					}
				} else {
					st_insert(t -> attr.name, t -> lineno, verificaGlobal == 0 ? globalLoc++ : location++, t -> attr.scope, "vector", "pointer", 1);
				}
			}
      		else
        		typeError(t,"Declaração Inválida. Vetor já declarado.");	
			if (t -> type == voidt)
				typeError(t, "Declaração Inválida. Tentativa de declaração de variável do tipo VOID");
			break;	
			case varK:
          		if (st_lookup(t->attr.name, t->attr.scope,"variable") == -1 && st_lookup(t->attr.name, "global","variable") == -1){
				
            		st_insert(t->attr.name,t->lineno,verificaGlobal == 0 ? globalLoc++ : location++, t->attr.scope, "variable", (t->type ==1) ? "integer":"void", 1);
				  }
          		else 
            		typeError(t,"Declaração Inválida. Variável já declarada.");	
				if (t -> type == voidt)
					typeError(t, "Declaração Inválida. Tentativa de declaração de variável do tipo VOID");
            break;
			case funcK:
				if (st_lookup(t->attr.name, t->attr.scope,"function") == -1 && st_lookup(t->attr.name, "global","function") == -1)
				{
					if(t->type == integert){
                        st_insert(t->attr.name,t->lineno,-9, t->attr.scope,"function", "integer", 1);
						
					}
					else{
                        st_insert(t->attr.name,t->lineno,-9, t->attr.scope,"function", "void", 1);
						
					}
				}
          		else
            		typeError(t,"Declaração Inválida. Variável já declarada.");	
			break;
			case callK:
				if(strcmp(t -> attr.name, "input") != 0 && strcmp(t -> attr.name, "output") != 0 && strcmp(t -> attr.name, "main") != 0){
					if (st_lookup(t->attr.name, t->attr.scope,"function") == -1 && st_lookup(t->attr.name, "global","function") == -1)
						typeError(t,"Chamada inválida. Função não declarada.");	
					else
						st_insert(t->attr.name,t->lineno,-9, t->attr.scope, "call", "-", 1);
				}
          	break;
			case returnK:
				
			break;
        	default:
          break;
      }
      break;
      case expressionK:
      switch (t->kind.exp)
      { 
		  case idK:
			if (st_lookup(t->attr.name, t->attr.scope,"variable") == -1 && st_lookup(t->attr.name, "global","variable") == -1)
				 typeError(t,"Ainda não declarado.");
			else{
            //	st_insert(t->attr.name,t->lineno,0, t->attr.scope, "variable", st_lookup_type(t->attr.name, t->attr.scope), 1);	
				
			}
		  break;
	      case vectK:
	      	if (st_lookup(t->attr.name, t->attr.scope,"vector") == -1 && st_lookup(t->attr.name, "global","vector") == -1)
				 typeError(t,"Ainda não declarado.");
			else{
				if(strcmp(st_lookup_type(t -> attr.name, t -> attr.scope), "pointer")!= 0 &&
						strcmp(st_lookup_type(t -> attr.name, t -> attr.scope), "param pointer")!= 0 &&
						strcmp(st_lookup_type(t -> attr.name, "global"), "pointer")!= 0 )
				typeError(t, "Ainda não declarado");	
		
        	//   	st_insert(t->attr.name,t->lineno,0, t->attr.scope, "vector", "pointer", 
			//	st_lookup_size(t -> attr.name, t -> attr.scope) == -1 ? st_lookup_size(t -> attr.name, "global") : st_lookup_size(t -> attr.name, t -> attr.scope));

			}
			break;
		  case vectIndexK:
		  	if (st_lookup(t->attr.name, t->attr.scope,"vector index") == -1 && st_lookup(t->attr.name, "global","vector index") == -1)
				 typeError(t,"Ainda não declarado.");
			else
            	st_insert(t->attr.name,t->lineno,0, t->attr.scope, "vector index", "integer", 1);	
          break;
        default:
        break;
      }
      break;
    default:
      break;
  }
}


static void checkNode(TreeNode * t)
{ switch (t->nodekind)
  { 
   case expressionK:
      switch (t->kind.exp)
      { 
        case opK:
        break;
        default:
        break;
      }
      break;
    case statementK:
      switch (t->kind.stmt)
      { 
        case ifK:
          if (t->child[0]->type != booleant)
           typeError(t->child[0],"Argumento do IF não booleano.");
        break;
		case whileK:
          if (t->child[0]->type != booleant)
           typeError(t->child[0],"Argumento do WHILE não booleano.");
        break;
        case assignK:

		//printf("\n\n\n\n\n\n -- name: %s  ",t->child[1]->attr.name);

		//if (strcmp(st_lookup_type(t->child[0]->attr.name, t->child[0]->attr.scope),"void") == 0 || strcmp(st_lookup_type(t->child[1]->attr.name, t->child[1]->attr.scope),"void") == 0)
          if (t->child[0]->type == voidt || t->child[1]->type == voidt)
            typeError(t->child[0],"Atribuição de um valor não inteiro");
		  
		  else if(t->child[1]->kind.stmt == callK)
		  {

			//printf("\n\n-----------------------\n\n");
			//printf("name: %s\n",t->child[1]->attr.name);
			//printf("escopo: %s\n",t->child[1]->attr.scope);
			//printf("tipo pelo lookup: %s\n",st_lookup_type(t->child[1]->attr.name, "global"));

		    if(strcmp(st_lookup_type(t->child[1]->attr.name, "global"), "integer") != 0 && strcmp(t->child[1]->attr.name, "input") != 0 ){
					typeError(t->child[1],"Atribuição de retorno vazio");
			}
			}
        break;
        default:
        break;
      }
    break;
    default:
    break;
  }
}


/* Procedure traverse is a generic recursive 
 * syntax tree traversal routine:
 * it applies preProc in preorder and postProc 
 * in postorder to tree pointed to by t
 */
static void traverse( TreeNode * t,
               void (* preProc) (TreeNode *),
               void (* postProc) (TreeNode *) )
{ 

	if (t != NULL)
 	{ 
		preProc(t);
    	{ 
	  		int i;
	        for (i=0; i < MAXCHILDREN; i++)
        		traverse(t->child[i],preProc,postProc);
    	}
    	postProc(t);
    	traverse(t->sibling,preProc,postProc);
  	}
}

/* nullProc is a do-nothing procedure to 
 * generate preorder-only or postorder-only
 * traversals from traverse
 */
static void nullProc(TreeNode * t)
{ 
	if (t==NULL) 
		return;
  	else 
		return;
}

void buildSymtab(TreeNode * syntaxTree)
{ 
   
  traverse(syntaxTree,insertNode,nullProc); 
  if(st_lookup("main", "global","function") == -1)
   {
   		fprintf(listing,"Função main não declarada\n");
   		Error = TRUE; 
   }
	
  if (TraceAnalyze && !Error)
  { 
    fprintf(listing,"\nSymbol table:\n\n");
    printSymTab(listing);
  }
}

void typeCheck(TreeNode * syntaxTree)
{ 
    traverse(syntaxTree,nullProc,checkNode);
}
