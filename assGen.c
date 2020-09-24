#include "intercodeglobals.h"
#include "ass.h"
#include "assGen.h"
#include "symtab.h"


static int globalOffset = -1;
static int location = 1;
///Formato da instrução: Operação Rd, r1, r2 (R) Operação rd, r1, imediato (I)
//Corrigir a Ordem quando for passa p binario r1, r2, rd ou r1, rd
static int lastSp;
static int currentSp;

char *corrigeTemp(char * temp){
    if(temp[1] != 't'){ 
        return temp;  
    }
    char * newTemp = (char*)malloc(5*sizeof(char));
    char *aux = (char*)malloc(3*sizeof(char));
    int iterador = 2;
    while(temp[iterador] != '\0'){
        aux[iterador-2] = temp[iterador];
        iterador++;
    }
    int correcao = atoi(aux)%15;
    sprintf(newTemp, "$t%d", correcao);
    return newTemp;
}

void lineGen(assHead* head, TApontador node){
    Operando a1, a2, a3;

    switch(node -> op){
        case addOp:
            a1 = initOperando(reg, 0, corrigeTemp(node -> addr1.contents.var.name), node -> addr1.contents.var.scope);
            a2 = initOperando(reg, 0, corrigeTemp(node -> addr2.contents.var.name), node -> addr2.contents.var.scope);
            a3 = initOperando(reg, 0, corrigeTemp(node -> addr3.contents.var.name), node -> addr3.contents.var.scope);

            insereAss(head, a1, a2, a3, add, location++, 0);

        break;
        case subOp:
            a1 = initOperando(reg, 0, corrigeTemp(node -> addr1.contents.var.name), node -> addr1.contents.var.scope);
            a2 = initOperando(reg, 0, corrigeTemp(node -> addr2.contents.var.name), node -> addr2.contents.var.scope);
            a3 = initOperando(reg, 0, corrigeTemp(node -> addr3.contents.var.name), node -> addr3.contents.var.scope);

            insereAss(head, a1, a2, a3, sub, location++, 0);

        break;
        case multOp:
            a1 = initOperando(nothing, 0, NULL, NULL);
            a2 = initOperando(reg, 0, corrigeTemp(node -> addr2.contents.var.name), node -> addr2.contents.var.scope);
            a3 = initOperando(reg, 0, corrigeTemp(node -> addr3.contents.var.name), node -> addr3.contents.var.scope);
            insereAss(head, a1, a2, a3, mult, location++, 0);

            a1 = initOperando(reg, 0,  corrigeTemp(node -> addr1.contents.var.name), node -> addr1.contents.var.scope);
            a2 = initOperando(reg, 0, "$LO", node -> addr1.contents.var.scope);
            a3 = initOperando(reg, 0, "$zero", node -> addr1.contents.var.scope);
            insereAss(head, a1, a2, a3, add, location++, 0);

        break;
        case divOp:
            a1 = initOperando(nothing, 0, NULL, NULL);
            a2 = initOperando(reg, 0, corrigeTemp(node -> addr2.contents.var.name), node -> addr2.contents.var.scope);
            a3 = initOperando(reg, 0, corrigeTemp(node -> addr3.contents.var.name), node -> addr3.contents.var.scope);
            insereAss(head, a1, a2, a3, divv, location++, 0);

            a1 = initOperando(reg, 0,  corrigeTemp(node -> addr1.contents.var.name), node -> addr1.contents.var.scope);
            a2 = initOperando(reg, 0, "$LO", node -> addr1.contents.var.scope);
            a3 = initOperando(reg, 0, "$zero", node -> addr1.contents.var.scope);
            insereAss(head, a1, a2, a3, add, location++, 0);

        break;
        case ltOp:
            a1 = initOperando(reg, 0, corrigeTemp(node -> addr1.contents.var.name), node -> addr1.contents.var.scope);
            a2 = initOperando(reg, 0, corrigeTemp(node -> addr2.contents.var.name), node -> addr2.contents.var.scope);
            a3 = initOperando(reg, 0, corrigeTemp(node -> addr3.contents.var.name), node -> addr3.contents.var.scope);

            insereAss(head, a1, a2, a3, cmp, location++, 0);

            a2 = a1;
            a3 = initOperando(reg, 0, "$zero", node -> addr1.contents.var.scope);

            insereAss(head, a1, a2, a3, eq, location++, 0);            

        break;
        case eqOp:
            a1 = initOperando(reg, 0, corrigeTemp(node -> addr1.contents.var.name), node -> addr1.contents.var.scope);
            a2 = initOperando(reg, 0, corrigeTemp(node -> addr2.contents.var.name), node -> addr2.contents.var.scope);
            a3 = initOperando(reg, 0, corrigeTemp(node -> addr3.contents.var.name), node -> addr3.contents.var.scope);

            insereAss(head, a1, a2, a3, eq, location++, 0);

        break;
        case letOp:
            a1 = initOperando(reg, 0, corrigeTemp(node -> addr1.contents.var.name), node -> addr1.contents.var.scope);
            a2 = initOperando(reg, 0, corrigeTemp(node -> addr2.contents.var.name), node -> addr2.contents.var.scope);
            a3 = initOperando(reg, 0, corrigeTemp(node -> addr3.contents.var.name), node -> addr3.contents.var.scope);

            insereAss(head, a1, a2, a3, cmp, location++, 0);

            a1 = initOperando(reg, 0, corrigeTemp(node -> addr2.contents.var.name), node -> addr2.contents.var.scope);
            a2 = initOperando(reg, 0, "$zero", node -> addr2.contents.var.scope);
            a3 = initOperando(imed, 2, NULL, NULL);

            insereAss(head, a1, a2, a3, addi, location++, 0);

            a3 = a1;
            a1 = initOperando(reg, 0, corrigeTemp(node -> addr1.contents.var.name), node -> addr1.contents.var.scope);
            a2 = a1;

            insereAss(head, a1, a2, a3, eq, location++, 0);

            a3 = initOperando(nothing, 0, NULL, NULL);

            insereAss(head, a1, a1, a3, not, location++, 0);

        break;
        case gtOp:

            a1 = initOperando(reg, 0, corrigeTemp(node -> addr1.contents.var.name), node -> addr1.contents.var.scope);
            a2 = initOperando(reg, 0, corrigeTemp(node -> addr2.contents.var.name), node -> addr2.contents.var.scope);
            a3 = initOperando(reg, 0, corrigeTemp(node -> addr3.contents.var.name), node -> addr3.contents.var.scope);

            insereAss(head, a1, a2, a3, cmp, location++, 0);

            a1 = initOperando(reg, 0, corrigeTemp(node -> addr2.contents.var.name), node -> addr2.contents.var.scope);
            a2 = initOperando(reg, 0, "$zero", node -> addr2.contents.var.scope);
            a3 = initOperando(imed, 2, NULL, NULL);

            insereAss(head, a1, a2, a3, addi, location++, 0);

            a3 = a1;
            a1 = initOperando(reg, 0, corrigeTemp(node -> addr1.contents.var.name), node -> addr1.contents.var.scope);
            a2 = a1;

            insereAss(head, a1, a2, a3, eq, location++, 0);
        
        break;
        case getOp:
            a1 = initOperando(reg, 0, corrigeTemp(node -> addr1.contents.var.name), node -> addr1.contents.var.scope);
            a2 = initOperando(reg, 0, corrigeTemp(node -> addr2.contents.var.name), node -> addr2.contents.var.scope);
            a3 = initOperando(reg, 0, corrigeTemp(node -> addr3.contents.var.name), node -> addr3.contents.var.scope);

            insereAss(head, a1, a2, a3, cmp, location++, 0);

            a2 = a1;
            a3 = initOperando(reg, 0, "$zero", node -> addr1.contents.var.scope);

            insereAss(head, a1, a2, a3, eq, location++, 0); 

            a3 = initOperando(nothing, 0, NULL, NULL);

            insereAss(head, a1, a1, a3, not, location++, 0);

        break;
        case noteqOp:
            a1 = initOperando(reg, 0, corrigeTemp(node -> addr1.contents.var.name), node -> addr1.contents.var.scope);
            a2 = initOperando(reg, 0, corrigeTemp(node -> addr2.contents.var.name), node -> addr2.contents.var.scope);
            a3 = initOperando(reg, 0, corrigeTemp(node -> addr3.contents.var.name), node -> addr3.contents.var.scope);

            insereAss(head, a1, a2, a3, eq, location++, 0);

            a3 = initOperando(nothing, 0, NULL, NULL);

            insereAss(head, a1, a1, a3, not, location++, 0);

        break;
        case andOp:
            a1 = initOperando(reg, 0, corrigeTemp(node -> addr1.contents.var.name), node -> addr1.contents.var.scope);
            a2 = initOperando(reg, 0, corrigeTemp(node -> addr2.contents.var.name), node -> addr2.contents.var.scope);
            a3 = initOperando(reg, 0, corrigeTemp(node -> addr3.contents.var.name), node -> addr3.contents.var.scope);

            insereAss(head, a1, a2, a3, and, location++, 0);

        break;
        case orOp:
            a1 = initOperando(reg, 0, corrigeTemp(node -> addr1.contents.var.name), node -> addr1.contents.var.scope);
            a2 = initOperando(reg, 0, corrigeTemp(node -> addr2.contents.var.name), node -> addr2.contents.var.scope);
            a3 = initOperando(reg, 0, corrigeTemp(node -> addr3.contents.var.name), node -> addr3.contents.var.scope);

            insereAss(head, a1, a2, a3, or, location++, 0);

        break;
        case assignOp:
            a1 = initOperando(reg, 0, corrigeTemp(node -> addr1.contents.var.name), node -> addr1.contents.var.scope);
            a2 = initOperando(reg, 0, corrigeTemp(node -> addr2.contents.var.name), node -> addr2.contents.var.scope);
            a3 = initOperando(reg, 0, "$zero", node -> addr2.contents.var.scope);

            insereAss(head, a1, a2, a3, add, location++, 0);
            
        break;
        case allocOp:
            if(strcmp(node -> addr2.contents.var.name, "global")!=0){
            currentSp+= node -> addr3.contents.val;
            a1 = initOperando(reg, 0, "$sp", "global");
            a2 = initOperando(reg, 0, "$zero", "global");
            a3 = initOperando(imed, node -> addr3.contents.val, NULL, NULL);
            insereAss(head, a1, a2, a3, addi, location++, 0);
            }
            
        break;
        case immedOp:
            a1 = initOperando(reg, 0, corrigeTemp(node -> addr1.contents.var.name), node -> addr1.contents.var.scope);
            a2 = initOperando(reg, 0, "$zero", node -> addr1.contents.var.scope);
            a3 = initOperando(imed, node->addr3.contents.val, NULL, node -> addr1.contents.var.scope);

            insereAss(head, a1, a2, a3, addi, location++, 0);

        break;
        case loadOp: ;
            int offset;
            int fixedLoc = currentSp - lastSp - 1;
            int memloc;
            offset = st_lookup_offset(node -> addr2.contents.var.name, "global");
            if(offset != -1){
                if(node -> addr3.akind == nop){
                    a1 = initOperando(reg, 0, corrigeTemp(node -> addr1.contents.var.name), node -> addr1.contents.var.scope);
                    a2 = initOperando(reg, 0, "$gp", node -> addr1.contents.var.scope);
                    a3 = initOperando(imed, offset, NULL, NULL);
                    insereAss(head, a1, a2, a3, lw, location++, 0);
                }else{
                    a1 = initOperando(reg, 0, corrigeTemp(node -> addr1.contents.var.name), node -> addr1.contents.var.scope);
                    a2 = initOperando(reg, 0, "$gp", node -> addr1.contents.var.scope);
                    a3 = initOperando(reg, 0, corrigeTemp(node -> addr3.contents.var.name), node -> addr3.contents.var.scope);
                    insereAss(head, a1, a2, a3, add, location++, 0);

                    a3 = initOperando(imed, offset, NULL, NULL);
                    insereAss(head, a1, a1, a3, lw, location++, 0);

                }
            }else{
                offset = st_lookup_offset(node -> addr2.contents.var.name, node -> addr2.contents.var.scope);

                memloc = fixedLoc - offset;
                a1 = initOperando(reg, 0, corrigeTemp(node -> addr1.contents.var.name), node -> addr1.contents.var.scope);
                a2 = initOperando(reg, 0, "$sp", node -> addr1.contents.var.scope);
                a3 = initOperando(imed, memloc, NULL, NULL);
                insereAss(head, a1, a2, a3, subi, location++, 0);
            
                if(node -> addr3.akind == nop){
                    a3 = initOperando(imed, 0, NULL, NULL);
                    insereAss(head, a1, a1, a3, lw, location++, 0);

                }else{
                    a1 = initOperando(reg, 0, corrigeTemp(node -> addr1.contents.var.name), node -> addr1.contents.var.scope);
                    a2 = a1;
                    a3 = initOperando(reg, 0, corrigeTemp(node -> addr3.contents.var.name), node -> addr3.contents.var.scope);
                    insereAss(head, a1, a2, a3, add, location++, 0);
                    
                    a3 = initOperando(imed, 0, NULL, NULL);
                    insereAss(head, a1, a1, a3, lw, location++, 0);
                }
            }

        break;
        case storeOp: ; //ficar atento as particularidades do store. sintaxe: sw r1, rd, imed note que
        //o endereço de memoria a ser slavado o dado é a soma de r1 com imed, e rd contem o dado a ser salvado.
            int offsetS;
            int fixedLocS = currentSp - lastSp - 1;
            int memlocS;
            offsetS = st_lookup_offset(node -> addr1.contents.var.name, "global");
            if(offsetS != -1){
                if(strcmp(node -> addr3.contents.var.name, "$zero") == 0){
                    a1 = initOperando(reg, 0, "$gp", node -> addr1.contents.var.scope);
                    a2 = initOperando(reg, 0, corrigeTemp(node -> addr2.contents.var.name), node -> addr2.contents.var.scope);
                    a3 = initOperando(imed, offset, NULL, NULL);
                    insereAss(head, a1, a2, a3, sw, location++, 0);
                }else{
                    a1 = initOperando(reg, 0, corrigeTemp(node -> addr3.contents.var.name), node -> addr3.contents.var.scope);
                    a2 = initOperando(reg, 0, "$gp", node -> addr1.contents.var.scope);
                    a3 = initOperando(reg, 0, corrigeTemp(node -> addr3.contents.var.name), node -> addr3.contents.var.scope);
                    insereAss(head, a1, a2, a3, add, location++, 0);

                    a3 = initOperando(imed, offset, NULL, NULL);
                    insereAss(head, a1, a2, a3, sw, location++, 0);

                }
            }else{
                offsetS = st_lookup_offset(node -> addr1.contents.var.name, node -> addr1.contents.var.scope);

                memlocS = fixedLocS - offsetS;
                a1 = initOperando(reg, 0, "$sp", node -> addr1.contents.var.scope);
                a2 = initOperando(reg, 0, "$sp", node -> addr1.contents.var.scope);
                a3 = initOperando(imed, memloc, NULL, NULL);
                insereAss(head, a1, a2, a3, subi, location++, 0);   
            
                if(strcmp(node -> addr3.contents.var.name, "$zero") == 0){
                    a3 = initOperando(imed, 0, NULL, NULL);
                    a2 = initOperando(reg, 0, corrigeTemp(node -> addr2.contents.var.name), node -> addr2.contents.var.scope);
                    insereAss(head, a1, a2, a3, sw, location++, 0);



                }else{
                    a1 = initOperando(reg, 0, corrigeTemp(node -> addr3.contents.var.name), node -> addr1.contents.var.scope);
                    a2 = a1;
                    a3 = initOperando(reg, 0, "$sp", node -> addr3.contents.var.scope);
                    insereAss(head, a1, a2, a3, add, location++, 0);
                    
                    a3 = initOperando(imed, 0, NULL, NULL);
                    a2 = initOperando(reg, 0, corrigeTemp(node -> addr2.contents.var.name), node -> addr2.contents.var.scope);
                    insereAss(head, a1, a1, a3, sw, location++, 0);
                }
                a1 = initOperando(reg, 0, "$sp", node -> addr1.contents.var.scope);
                a2 = initOperando(reg, 0, "$sp", node -> addr1.contents.var.scope);
                a3 = initOperando(imed, memloc, NULL, NULL);
                insereAss(head, a1, a2, a3, addi, location++, 0);
            }

        break;
        case gotoOp:

        break;
        case iffOp:
            a1 = initOperando(reg, 0, corrigeTemp(node -> addr1.contents.var.name), node -> addr1.contents.var.scope);
            a2 = initOperando(reg, 0, "$zer0", node -> addr1.contents.var.scope);
            a3 = initOperando(reg, 0, node -> addr2.contents.var.name, node -> addr2.contents.var.scope);

            insereAss(head, a1, a2, a3, beq, location++, 0);

        break;
        case retOp:

        break;
        case funOp:

        break;
        case endOp:

        break;
        case paramOp:

        break;
        case callOp:
        // Empilha todos os registradores usados.
        lastSp = currentSp;

        break;
        case argOp:

        break;
        case labOp:
            a1 = initOperando(reg, 0, node -> addr1.contents.var.name, node -> addr2.contents.var.scope);
            a2 = initOperando(nothing, 0, NULL, NULL);
            a3 = a2;

            insereAss(head, a1, a2, a3, notInst, location++, 1);

        break;
        case hltOp:

        break;
        case inputOp:

        break;
        case outputOp:

        break;
        default:
        break;
    }

}

void assGen(assHead* assHead, THead * intercode){

    Operando aux1, aux2, aux3;
// Alocando as variáveis globais
    TApontador aux = intercode -> head -> prox;
    while(aux -> location != 0){
        if(aux -> op == allocOp && strcmp(aux -> addr2.contents.var.name, "global") == 0){
            globalOffset += aux -> addr3.contents.val;
        }
        
        aux = aux -> prox;
    }
    aux1 = initOperando(reg, 0, "$sp", "global");
    aux2 = initOperando(reg, 0, "$zero", "global");
    aux3 = initOperando(imed, globalOffset, NULL, NULL);
    insereAss(assHead, aux1, aux2, aux3, addi, location++, 0);

    currentSp = globalOffset;
    lastSp = currentSp; 

    aux1 = initOperando(reg, 0, "$gp", "global");
    aux2 = initOperando(reg, 0, "$zero", "global");
    aux3 = initOperando(imed, 0, NULL, NULL);
    insereAss(assHead, aux1, aux2, aux3, addi, location++, 0);


    aux = intercode -> head -> prox;
    while(aux -> location != 0){
        lineGen(assHead, aux);
        aux = aux -> prox;
    }
}
