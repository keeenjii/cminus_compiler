#include "globals.h"

typedef enum {
    numA, labelA, nop
} Addrkind;

typedef enum {
    addOp, subOp, multOp, divOp, ltOp, eqOp,       // TEM UMA INSTRUÇÃO A MAIS:
    ledOp, gtOp, getOp, andOp, orOp, assignOp,      //iGUALDADE CONDICIONAL == 
    allocOp, immedOp, loadOp, storeOp, vecOp,
    gotoOp, iffOp, retOp, funOp, endOp, paramOp,
    callOp, argOp, labOp, hltOp, nopOp
} Opkind;