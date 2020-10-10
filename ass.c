#include "ass.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

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

void insereAss(assHead *a, Operando op1, Operando op2, Operando op3, Instrucao inst, int location, int islabel){
    assApontador p = (assApontador)malloc(sizeof(assCelula));
    p -> op1 = op1;
    p -> op2 = op2;
    p -> op3 = op3;
    p -> inst = inst;
    p -> location = location;
    p -> islabel = islabel;

    a -> head -> ant -> prox = p;
    p -> ant = a -> head -> ant;
    p -> prox = a -> head;
    a -> head -> ant = p;
}


char * printOp(Operando op){
    if(op.kind == reg){
        return op.contents.var.name;
    } else if(op.kind == imed){
        char * num = (char*)malloc(sizeof(char)*nDigitos(op.contents.imediato));
        sprintf(num, "%d", op.contents.imediato);
        return num;
    } else return " ";
}

void printAss(assHead *head){
    assApontador iterador = head -> head;
    iterador = iterador -> prox;
    FILE * code = fopen("asscode.txt", "w");
    while(iterador -> location != 0){
        if(iterador -> islabel == 1){
            fprintf(code, "%s\n", iterador -> op1.contents.var.name);
        }else{
            char op[7];
            switch(iterador -> inst){
                case add:
                strcpy(op, "add");
                break;
                case sub:
                strcpy(op, "sub");
                break;
                case mult:
                strcpy(op, "mult");
                break;
                case divv:
                strcpy(op, "div");
                break;
                case sra:
                strcpy(op, "sra");
                break;
                case sla:
                strcpy(op, "sla");
                break;
                case and:
                strcpy(op, "and");
                break;
                case or:
                strcpy(op, "or");
                break;
                case xor:
                strcpy(op, "xor");
                break;
                case eq:
                strcpy(op, "eq");
                break;
                case cmp:
                strcpy(op, "cmp");
                break;
                case not:
                strcpy(op, "not");
                break;
                case addi:
                strcpy(op, "addi");
                break;
                case subi:
                strcpy(op, "subi");
                break;
                case lw:
                strcpy(op, "lw");
                break;
                case sw:
                strcpy(op, "sw");
                break;
                case andi:
                strcpy(op, "andi");
                break;
                case ori:
                strcpy(op, "ori");
                break;
                case xori:
                strcpy(op, "xori");
                break;
                case cmpi:
                strcpy(op, "cmpi");
                break;
                case jr:
                strcpy(op, "jr");
                break;
                case beq:
                strcpy(op, "beq");
                break;
                case bne:
                strcpy(op, "bne");
                break;
                case j:
                strcpy(op, "j");
                break;
                case jal:
                strcpy(op, "jal");
                break;
                case breakk:
                strcpy(op, "break");
                break;
                case out:
                strcpy(op, "out");
                break;
                case in:
                strcpy(op, "in");
                break;
                default:
                break;
            }
            char *pos1 = printOp(iterador -> op1);
            char *pos2 = printOp(iterador -> op2);
            char *pos3 = printOp(iterador -> op3);

            fprintf(code,"\t%s %s %s %s\n", op, pos1, pos2, pos3);
        }
      	iterador = iterador -> prox;
    }
    fclose(code);
}
