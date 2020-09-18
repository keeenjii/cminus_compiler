#include "ass.h"
#include <stdlib.h>
#include <stdio.h>

int main(){
    assHead *a = initAss();
    Operando aux1 = initOperando(reg, 0, "$t1", "global");
    Operando aux2 = initOperando(reg, 0, "$t1", "global");
    Operando aux3 = initOperando(imed, 12, "$t1", "global");
    Operando aux4 = initOperando(label, 0, "teste", "global");
    if(aux4.kind == label)
        printf("cu");

    insereAss(a, aux1, aux2, aux1, addi, 1);
    insereAss(a, aux1, aux2, aux3, addi, 2);
    insereAss(a, aux4, aux2, aux1, addi, 3);
    insereAss(a, aux1, aux2, aux3, addi, 4);
    insereAss(a, aux4, aux1, aux1, nop, 5);

    printAss(a);
    return 0;
}