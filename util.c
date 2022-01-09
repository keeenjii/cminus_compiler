#include "globals.h"
#include "util.h"

/* Procedure printToken prints a token 
 * and its lexeme to the listing file
 */
void printToken( TokenType token, const char* tokenString )
{ switch (token){ 
    case IF:            fprintf(listing, "Palavra reservada: IF, Lexema: %s\n",tokenString); break;
    case ELSE:          fprintf(listing, "Palavra reservada: ELSE, Lexema: %s\n",tokenString); break;
    case RETURN:        fprintf(listing, "Palavra reservada: RETURN, Lexema: %s\n",tokenString); break;
    case INT:		        fprintf(listing, "Palavra reservada: INT, Lexema: %s\n",tokenString); break;
    case VOID:	        fprintf(listing, "Palavra reservada: VOID, Lexema: %s\n",tokenString); break;
    case WHILE:         fprintf(listing, "Palavra reservada: WHILE, Lexema: %s\n",tokenString); break;
    case IGL:           fprintf(listing, "=\n"); break;
	  case PEV:           fprintf(listing, ";\n"); break;
	  case APR:           fprintf(listing, "(\n"); break;
	  case FPR:           fprintf(listing, ")\n"); break;
	  case MENOR:         fprintf(listing, "<\n"); break;
    case MENORIGUAL:    fprintf(listing, "<=\n"); break;
    case MAIOR:         fprintf(listing, ">\n"); break;
    case MAIORIGUAL:    fprintf(listing, ">=\n"); break;
    case IGUALIGUAL:    fprintf(listing, "==\n"); break;
	  case DIFERENTE:     fprintf(listing, "!=\n"); break;
	  case VIRGULA:       fprintf(listing, ",\n"); break;
	  case COLCHETEABRE:  fprintf(listing, "[\n"); break;
	  case COLCHETEFECHA: fprintf(listing, "]\n"); break;
	  case CHAVESABRE:    fprintf(listing, "{\n"); break;
	  case CHAVESFECHA:   fprintf(listing, "}\n"); break;
	  case SOM:           fprintf(listing, "+\n"); break;
	  case SUB:           fprintf(listing, "-\n"); break;
	  case MUL:           fprintf(listing, "*\n"); break;
	  case DIV:           fprintf(listing, "/\n"); break;
    case FIM:           fprintf(listing, "EOF\n"); break;
    case NUM:           fprintf(listing, "NUM, val= %s\n",tokenString); break;
    case ID:            fprintf(listing, "ID, name= %s\n",tokenString); break;
    case ERR:           fprintf(listing, "Lexema: %s\n",tokenString); break;
    default: /* should never happen */
      fprintf(listing,"Unknown token: %d\n",token);
  }
}

void aggScope(TreeNode* t, char* scope)
{
	int i;
	while(t != NULL)
	{
		for(i = 0; i < MAXCHILDREN; ++i)
		{
			t->attr.scope = scope;
			aggScope(t->child[i], scope);
		}
		t = t->sibling; 
	}
}



/* Function newStmtNode creates a new statement
 * node for syntax tree construction
 */
TreeNode * newStmtNode(StmtKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i=0;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) 
      t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = statementK;
    t->kind.stmt = kind;
    t->lineno = lineno;
    t->attr.scope = "global";

	// Preencher a variável type toda vez que essa função ser chamada
  }
  return t;
}

/* Function newExpNode creates a new expression 
 * node for syntax tree construction
 */
TreeNode * newExpNode(ExpKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  
  int i=0;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++){ 
      t->child[i] = NULL;
    }
    t->sibling = NULL;
    t->nodekind = expressionK;
    t->kind.exp = kind;
    t->lineno = lineno;
    t->type = VOID;
    t->attr.scope = "global";
    
  }

  return t;
}

/* Function copyString allocates and makes a new
 * copy of an existing string
 */
char * copyString(char * s)
{ int n;
  char * t;
  if (s==NULL) 
    return NULL;
  
  n = strlen(s)+1;
  t = malloc(n);
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else 
    strcpy(t,s);
  return t;
}

/* Variable indentno is used by printTree to
 * store current number of spaces to indent
 */
static int indentno = 0;

/* macros to increase/decrease indentation */
#define INDENT indentno+=2
#define UNINDENT indentno-=2

/* printSpaces indents by printing spaces */
static void printSpaces(void)
{ int i;
  for (i=0;i<indentno;i++)
    fprintf(listing," ");
}

/* procedure printTree prints a syntax tree to the 
 * listing file using indentation to indicate subtrees
 */
void printTree( TreeNode * tree )
{ 
  int i;
  INDENT;
  while (tree != NULL) {
    printSpaces();
    if (tree->nodekind==statementK)
    { switch (tree->kind.stmt) {
        case ifK:     fprintf(listing,"If\n"); break;
        case whileK:  fprintf(listing,"While\n"); break;
		case paramK:  fprintf(listing,"Parametro: %s\n", tree -> attr.name); break;
        case assignK: fprintf(listing,"Atribuição\n"); break;
        case varK:    fprintf(listing,"Variável: %s\n",tree->attr.name); break;
        case funcK:   fprintf(listing,"Função %s\n", tree->attr.name); break;
		case callK:   fprintf(listing,"Chamada de Função %s \n", tree->attr.name); break;
		case returnK: fprintf(listing,"Retorno\n"); break;
    case vectStmtK: fprintf(listing, "Variável vetor: %s\n", tree -> attr.name); break;
        default:      fprintf(listing,"Unknown ExpNode kind 1\n"); break;

      }
    }
    else if (tree->nodekind==expressionK)
    { 

      switch (tree->kind.exp) {
        case opK:         fprintf(listing,"Op: ");
                          printToken(tree->attr.op,"\0");
                          break;
        case constK:      fprintf(listing,"Const: %d\n",tree->attr.val);
                          break;
        case idK:         fprintf(listing,"Id: %s\n",tree->attr.name);
                          break;
		    case vectK:       fprintf(listing,"Vetor: %s\n",tree->attr.name);
                          break;
		    case vectIndexK:  fprintf(listing,"Indíce: [%d]\n",tree->attr.val);
                          break;
        case typeK:       fprintf(listing,"Type %s\n",tree->attr.name);
                          break;
        default:          fprintf(listing,"Unknown ExpNode kind 2\n");
                          break;
      }
    }
    else 
      fprintf(listing,"Unknown node kind 3\n");

    for (i=0;i<MAXCHILDREN;i++){
      //printf("\n\n%d\n\n",i);
      if(tree->child[i] != NULL)
        printTree(tree->child[i]);
    }
    
    tree = tree->sibling;
  }
  UNINDENT;
}