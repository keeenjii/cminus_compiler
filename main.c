#include "globals.h"
#include "util.h"
#include "parse.h"
#include "scan.h"
#include "symtab.h"
#include "analyze.h"
#include "cgen.h"



/* set NO_PARSE to TRUE to get a scanner-only compiler */
#define NO_PARSE FALSE
/* set NO_ANALYZE to TRUE to get a parser-only compiler */
#define NO_ANALYZE FALSE


/* allocate global variables */
int  lineno = 0;
FILE * source;
FILE * listing;

/* allocate and set tracing flags */
int TraceScan = TRUE;
int TraceParse = TRUE;
int TraceAnalyze = TRUE;
int TraceCode   = TRUE;
int Error = FALSE;


int main( int argc, char * argv[] ){ 

    TreeNode * syntaxTree;
    char pgm[120]; /* source code file name */
    
    if (argc != 2){ 
        fprintf(stderr,"Arquivo: %s <filename>\n",argv[0]);
        exit(1);
    }
    
    strcpy(pgm,argv[1]) ;
    
    source = fopen(pgm,"r");
    
    if (source==NULL){ 
        fprintf(stderr,"Arquivo %s não encontrado!\n",pgm);
        exit(1);
    }
 
    listing = stdout; /* send listing to screen */
    
    fprintf(listing,"\nCOMPILAÇÃO C-: %s\n",pgm);
    
    if(NO_PARSE){
        while (getToken()!=ENDFILE);
    }
    else{
        
	printf("\nErro: %d",Error);

	if(!Error){
		syntaxTree = parse();

        	if (TraceParse && !Error) {
            		fprintf(listing,"\nÁrvore Sintática:\n");
            		printTree(syntaxTree);
        	}
    	
        if(!NO_ANALYZE){

            if (! Error){ 
                if (TraceAnalyze){
                    fprintf(listing,"\nConstruindo Tabela de Símbolos...\n");
                    buildSymtab(syntaxTree);
                }

                if (TraceAnalyze){ 
                    fprintf(listing,"\nChecagem de Tipos...\n");
                    typeCheck(syntaxTree);
                }

                if (TraceAnalyze && !Error) 
                    fprintf(listing,"\nChecagem de Tipos Finalizada!\n");
                }
            }
        }

    }
    if(!Error){    
        codeGen(syntaxTree);
        fprintf(listing,"\nCompilação Finalizada\n");
    }

    fclose(source);
   
    return 0;

}
