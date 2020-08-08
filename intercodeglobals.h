#include "globals.h"

typedef enum {
    numA, labelA, nop
} Addrkind;

typedef enum {
    addOp, subOp, multOp, divOp, ltOp, eqOp,       // TEM UMA INSTRUÇÃO A MAIS:
    letOp, gtOp, getOp, andOp, orOp, assignOp,      //iGUALDADE CONDICIONAL == 
    allocOp, immedOp, loadOp, storeOp, vecOp,
    gotoOp, iffOp, retOp, funOp, endOp, paramOp,
    callOp, argOp, labOp, hltOp, inputOp, outputOp
} Opkind;

typedef struct {
    Addrkind akind;
    union{
        int val;
        struct {
            char *name;
            char *scope;
        } var;
    }contents;
} Address;

typedef struct item* TApontador;

typedef struct item{
    TApontador prox, ant;
    Address addr1, addr2, addr3;
    Opkind op;
    int location;

}TItem;

typedef struct {
    TApontador head;
    int tamanho;
}THead;

THead *initLista();

Address initAddress(Addrkind akind, int val, char *name, char *scope);

void insereLista(THead* a, Address addr1, Address addr2, Address addr3, Opkind op, int location);

TApontador pesquisaLista(THead* a, int location);

int nDigitos(long a);

char* printAddress(Address addr);

void printIntercode(THead * head);