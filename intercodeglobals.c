#include "intercodeglobals.h"
#include <string.h>
#include <math.h>

/// Implementar uma rotina para liberar a memória utilizada.
THead* initLista(){
    THead* p = (THead*)malloc(sizeof(THead));
    p -> tamanho = 0;
    TApontador a = (TApontador)malloc(sizeof(TItem));
    a -> prox = a;
    a -> ant = a;
    a -> location = 0;
    p -> head = a;
    return p;
}

Address initAddress(Addrkind kind, int val, char *name, char *scope){
    Address p;
    p.akind = kind;
    if(kind == numA){
        p.contents.val = val;
    } else if(kind == labelA){
        p.contents.var.name = name;
        p.contents.var.scope = scope;
    }
    return p;
}

void insereLista(THead* a, Address addr1, Address addr2, Address addr3, Opkind op, int location){
    TApontador p = (TApontador)malloc(sizeof(TItem));
    p -> addr1 = addr1;
    p -> addr2 = addr2;
    p -> addr3 = addr3;
    p -> op = op;
    p -> location = location;

    a -> head -> ant -> prox = p;
    p -> ant = a -> head -> ant;
    p -> prox = a -> head;
    a -> head -> ant = p;
}

TApontador pesquisaLista(THead* a, int location){

    TApontador x = a -> head -> prox;
    TApontador item;

    while(x -> location != 0){
        if (x -> location == location)
            return x;
        x = x -> prox;
    }
    return NULL;
}

int nDigitos(long a){
    int dig = 0;
    while(a/(pow(10, dig)) >= 1){
        dig++;
    }
    return dig;
}

char* printAddress(Address addr){
    if (addr.akind == labelA){
        return addr.contents.var.name;
    } else if (addr.akind == numA){
        char * num = (char*)malloc(sizeof(char)*nDigitos(addr.contents.val));
        sprintf(num, "%d", addr.contents.val);
        return num;
    } else {
        return "--";
    }
}

void printIntercode(THead * head){
    TApontador iterador = head -> head;
    iterador = iterador -> prox;
        while(iterador -> location != 0){
            char op[10];
            switch(iterador -> op){
                case addOp:
                strcpy(op, "add");
                break;
                case subOp:
                strcpy(op, "sub");
                break;
                case multOp:
                strcpy(op, "mult");
                break;
                case divOp:
                strcpy(op, "div");
                break;
                case ltOp:
                strcpy(op, "lt");
                break;
                case eqOp:
                strcpy(op, "eq");
                break;
                case letOp:
                strcpy(op, "let");
                break;
                case gtOp:
                strcpy(op, "gt");
                break;
                case getOp:
                strcpy(op, "get");
                break;
                case andOp:
                strcpy(op, "and");
                break;
                case orOp:
                strcpy(op, "or");
                break;
                case assignOp:
                strcpy(op, "assign");
                break;
                case allocOp:
                strcpy(op, "alloc");
                break;
                case immedOp:
                strcpy(op, "immed");
                break;
                case loadOp:
                strcpy(op, "load");
                break;
                case storeOp:
                strcpy(op, "store");
                break;
                case gotoOp:
                strcpy(op, "goto");
                break;
                case iffOp:
                strcpy(op, "iff");
                break;
                case retOp:
                strcpy(op, "ret");
                break;
                case funOp:
                strcpy(op, "fun");
                break;
                case endOp:
                strcpy(op, "end");
                break;
                case paramOp:
                strcpy(op, "param");
                break;
                case callOp:
                strcpy(op, "call");
                break;
                case argOp:
                strcpy(op, "arg");
                break;
                case labOp:
                strcpy(op, "lab");
                break;
                case hltOp:
                strcpy(op, "hlt");
                break;
                case inputOp:
                strcpy(op, "input");
                break;
                case outputOp:
                strcpy(op, "output");
                break;
                default:
                strcpy(op, "ERRO");
            }
            char *pos1 = printAddress(iterador -> addr1);
            char *pos2 = printAddress(iterador -> addr2);
            char *pos3 = printAddress(iterador -> addr3);

            printf("<%s: %s, %s, %s>\n", op, pos1, pos2, pos3);
            iterador = iterador -> prox;
        }
}


/* addOp, subOp, multOp, divOp, ltOp, eqOp,       // TEM UMA INSTRUÇÃO A MAIS:
    letOp, gtOp, getOp, andOp, orOp, assignOp,      //iGUALDADE CONDICIONAL == 
    allocOp, immedOp, loadOp, storeOp, vecOp,
    gotoOp, iffOp, retOp, funOp, endOp, paramOp,
    callOp, argOp, labOp, hltOp */