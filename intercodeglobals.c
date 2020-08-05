#include "intercodeglobals.h"
#include <string.h>

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

