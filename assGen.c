#include "intercodeglobals.h"
#include "ass.h"
#include "assGen.h"
static int globalOffset = 0;
static int location = 1;
///Formato da instrução: Operação Rd, r1, r2 (R) Operação rd, r1, imediato (I)
//Corrigir a Ordem quando for passa p binario r1, r2, rd ou r1, rd

void lineGen(assHead* head, TApontador node){
    Operando a1, a2, a3;

    switch(node -> op){
        case addOp:
            a1 = initOperando(reg, 0, node -> addr1.contents.var.name, node -> addr1.contents.var.scope);
            a2 = initOperando(reg, 0, node -> addr2.contents.var.name, node -> addr2.contents.var.scope);
            a3 = initOperando(reg, 0, node -> addr3.contents.var.name, node -> addr3.contents.var.scope);

            insereAss(head, a1, a2, a3, add, location++, 0);

        break;
        case subOp:

        break;
        case multOp:

        break;
        case divOp:

        break;
        case ltOp:

        break;
        case eqOp:

        break;
        case letOp:

        break;
        case gtOp:
        
        break;
        case getOp:

        break;
        case noteqOp:

        break;
        case andOp:

        break;
        case orOp:

        break;
        case assignOp:
        
        break;
        case allocOp:

        break;
        case immedOp:

        break;
        case loadOp:

        break;
        case storeOp:

        break;
        case vecOp:

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
//    printf("%d", globalOffset);

    aux = intercode -> head -> prox;
    while(aux -> location != 0){
        lineGen(assHead, aux);
        aux = aux -> prox;
    }
}
