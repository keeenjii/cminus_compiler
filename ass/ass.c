#include "ass.h"
#include <stdio.h>
#include <stdlib.h>

assHead *initAss(){
    assHead* p = (assHead*)malloc(sizeof(assHead));
    p -> tamanho = 0;
    assApontador a = (assApontador)malloc(sizeof(assCelula));
    a -> prox = a;
    a -> ant = a;
    a -> location = 0;
    p -> head = a;
    return p;
}

Operando initOperando(tipoOperando kind, int imediato, char* name, char* scope){
    Operando p;
    p.kind = kind;
    if(kind == imed){
        p.contents.imediato = imediato;
    } else if(kind == reg){
        p.contents.var.name = name;
        p.contents.var.scope = scope;
    }
    return p;

}

void insereAss(assHead *a, Operando op1, Operando op2, Operando op3, Inst inst, int location){


}