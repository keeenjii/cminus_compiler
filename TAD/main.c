#include <stdio.h>
#include <stdlib.h>

#include "intercodeglobals.h"

int main(){
    THead* A = initLista();

    Address p = initAddress(numA, 10, NULL, NULL);
    Address o = initAddress(labelA, -1, "teste", "teste2");
    Address d = initAddress(numA, 20, NULL, NULL);

    insereLista(A, p, o, d, callOp, 1);
    printf("%s\n", A -> head -> prox -> addr2.contents.var.name);

    Address x = initAddress(nop, -1, NULL, NULL);
    Address y = initAddress(numA, 50, NULL, NULL);
    Address z = initAddress(labelA, -1, "kenjola", "kenjila");

    insereLista(A, x, y, z, gotoOp, 2);

    insereLista(A, x, y, z, gotoOp, 3);

    insereLista(A, p, o, d, gotoOp, 4);

    printf("%s, %d\n", A -> head -> prox -> prox -> addr3.contents.var.scope, A -> head -> prox -> prox -> addr2.contents.val);
    
    TApontador a = pesquisaLista(A, 4);

    printf("%s\n, %d\n", a -> addr2.contents.var.scope, a->location);

    int w = nDigitos(10232323232323);
    printf("%d\n", w);

    char* coco = printAddress(x);

    char cu[10] = "teste";
    printf("%s\n", cu);

    printf("%s\n", coco);

    printIntercode(A);

    return 0;
}