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
static int lastArg = 0;
static int lastParam = 0;
static int em_uso[17] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // vetor binário para indentificar quais registradores temporários estao em uso
int paramCount = 0;
void empilha_temporario(assHead *head, int *em_uso){
    Operando a1, a2, a3;
    char *tempRegister;
    //printf("Empilhamento de registradores temporários: ");
    for(int i  = 0; i < 17; i++){
        if(em_uso[i] == 1){
            a1 = initOperando(reg, 0, "$sp", NULL);
            a3 = initOperando(imed, 1, NULL, NULL);
            insereAss(head, a1, a1, a3, addi, location++, 0);

            //store do valor do registrador temporario iterado
            tempRegister = (char*)malloc(4*sizeof(char));
            sprintf(tempRegister, "$t%d", i);
            //printf("%s ", tempRegister);
            a2 = initOperando(reg, 0, tempRegister, NULL);
            a3 = initOperando(imed, 0, NULL, NULL);
            insereAss(head, a1, a2, a3, sw, location++, 0);
            
        }
    }
    //printf("\n");
}

void desempilha_temporario(assHead *head, int *ultimo_uso){
    Operando a1, a2, a3;
    char *tempRegister;
    //printf("desempilhamento de registradores temporários: ");
    for(int i = 14; i >= 0; i--){
        if(em_uso[i] == 1){
            tempRegister = (char*)malloc(4*sizeof(char));
            sprintf(tempRegister, "$t%d", i);        
            a1 = initOperando(reg, 0, tempRegister, NULL);
            a2 = initOperando(reg, 0, "$sp", NULL);
            a3 = initOperando(imed, 0, NULL, NULL);
            insereAss(head, a1, a2, a3, lw, location++, 0);
            //printf("%s ", tempRegister);
            a3 = initOperando(imed, 1, NULL, NULL);
            insereAss(head, a2, a2, a3, subi, location++, 0);
        }
    }
    //printf("\n");
}

int temp_to_int(char *temp){
    if(temp[1] != 't'){ 
        return -1;  
    }
    char * newTemp = (char*)malloc(5*sizeof(char));
    char *aux = (char*)malloc(3*sizeof(char));
    int iterador = 2;
    while(temp[iterador] != '\0'){
        aux[iterador-2] = temp[iterador];
        iterador++;
    }
    return (atoi(aux)%17);
}

char *nextTemp(char *temp){
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
    int correcao = atoi(aux)%17;
    correcao++;
    sprintf(newTemp, "$t%d", correcao);
    return newTemp;
}

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
    int correcao = atoi(aux)%17;
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

            em_uso[temp_to_int(node -> addr1.contents.var.name)] = 1;
            em_uso[temp_to_int(node -> addr2.contents.var.name)] = 0;
            em_uso[temp_to_int(node -> addr3.contents.var.name)] = 0;

        break;
        case subOp:
            a1 = initOperando(reg, 0, corrigeTemp(node -> addr1.contents.var.name), node -> addr1.contents.var.scope);
            a2 = initOperando(reg, 0, corrigeTemp(node -> addr2.contents.var.name), node -> addr2.contents.var.scope);
            a3 = initOperando(reg, 0, corrigeTemp(node -> addr3.contents.var.name), node -> addr3.contents.var.scope);

            insereAss(head, a1, a2, a3, sub, location++, 0);

            em_uso[temp_to_int(node -> addr1.contents.var.name)] = 1;
            em_uso[temp_to_int(node -> addr2.contents.var.name)] = 0;
            em_uso[temp_to_int(node -> addr3.contents.var.name)] = 0;

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

            em_uso[temp_to_int(node -> addr1.contents.var.name)] = 1;
            em_uso[temp_to_int(node -> addr2.contents.var.name)] = 0;
            em_uso[temp_to_int(node -> addr3.contents.var.name)] = 0;

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

            em_uso[temp_to_int(node -> addr1.contents.var.name)] = 1;
            em_uso[temp_to_int(node -> addr2.contents.var.name)] = 0;
            em_uso[temp_to_int(node -> addr3.contents.var.name)] = 0;

        break;
        case ltOp:
            a1 = initOperando(reg, 0, corrigeTemp(node -> addr1.contents.var.name), node -> addr1.contents.var.scope);
            a2 = initOperando(reg, 0, corrigeTemp(node -> addr2.contents.var.name), node -> addr2.contents.var.scope);
            a3 = initOperando(reg, 0, corrigeTemp(node -> addr3.contents.var.name), node -> addr3.contents.var.scope);

            insereAss(head, a1, a2, a3, cmp, location++, 0);

            a2 = a1;
            a3 = initOperando(reg, 0, "$zero", node -> addr1.contents.var.scope);

            insereAss(head, a1, a2, a3, eq, location++, 0);    

            em_uso[temp_to_int(node -> addr1.contents.var.name)] = 1;
            em_uso[temp_to_int(node -> addr2.contents.var.name)] = 0;
            em_uso[temp_to_int(node -> addr3.contents.var.name)] = 0;        

        break;
        case eqOp:
            a1 = initOperando(reg, 0, corrigeTemp(node -> addr1.contents.var.name), node -> addr1.contents.var.scope);
            a2 = initOperando(reg, 0, corrigeTemp(node -> addr2.contents.var.name), node -> addr2.contents.var.scope);
            a3 = initOperando(reg, 0, corrigeTemp(node -> addr3.contents.var.name), node -> addr3.contents.var.scope);

            insereAss(head, a1, a2, a3, eq, location++, 0);

            em_uso[temp_to_int(node -> addr1.contents.var.name)] = 1;
            em_uso[temp_to_int(node -> addr2.contents.var.name)] = 0;
            em_uso[temp_to_int(node -> addr3.contents.var.name)] = 0;

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

            em_uso[temp_to_int(node -> addr1.contents.var.name)] = 1;
            em_uso[temp_to_int(node -> addr2.contents.var.name)] = 0;
            em_uso[temp_to_int(node -> addr3.contents.var.name)] = 0;

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

            em_uso[temp_to_int(node -> addr1.contents.var.name)] = 1;
            em_uso[temp_to_int(node -> addr2.contents.var.name)] = 0;
            em_uso[temp_to_int(node -> addr3.contents.var.name)] = 0;
        
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

            em_uso[temp_to_int(node -> addr1.contents.var.name)] = 1;
            em_uso[temp_to_int(node -> addr2.contents.var.name)] = 0;
            em_uso[temp_to_int(node -> addr3.contents.var.name)] = 0;

        break;
        case noteqOp:
            a1 = initOperando(reg, 0, corrigeTemp(node -> addr1.contents.var.name), node -> addr1.contents.var.scope);
            a2 = initOperando(reg, 0, corrigeTemp(node -> addr2.contents.var.name), node -> addr2.contents.var.scope);
            a3 = initOperando(reg, 0, corrigeTemp(node -> addr3.contents.var.name), node -> addr3.contents.var.scope);

            insereAss(head, a1, a2, a3, eq, location++, 0);

            a3 = initOperando(nothing, 0, NULL, NULL);

            insereAss(head, a1, a1, a3, not, location++, 0);

            em_uso[temp_to_int(node -> addr1.contents.var.name)] = 1;
            em_uso[temp_to_int(node -> addr2.contents.var.name)] = 0;
            em_uso[temp_to_int(node -> addr3.contents.var.name)] = 0;

        break;
        case andOp:
            a1 = initOperando(reg, 0, corrigeTemp(node -> addr1.contents.var.name), node -> addr1.contents.var.scope);
            a2 = initOperando(reg, 0, corrigeTemp(node -> addr2.contents.var.name), node -> addr2.contents.var.scope);
            a3 = initOperando(reg, 0, corrigeTemp(node -> addr3.contents.var.name), node -> addr3.contents.var.scope);

            insereAss(head, a1, a2, a3, and, location++, 0);

            em_uso[temp_to_int(node -> addr1.contents.var.name)] = 1;
            em_uso[temp_to_int(node -> addr2.contents.var.name)] = 0;
            em_uso[temp_to_int(node -> addr3.contents.var.name)] = 0;

        break;
        case orOp:
            a1 = initOperando(reg, 0, corrigeTemp(node -> addr1.contents.var.name), node -> addr1.contents.var.scope);
            a2 = initOperando(reg, 0, corrigeTemp(node -> addr2.contents.var.name), node -> addr2.contents.var.scope);
            a3 = initOperando(reg, 0, corrigeTemp(node -> addr3.contents.var.name), node -> addr3.contents.var.scope);

            insereAss(head, a1, a2, a3, or, location++, 0);

            em_uso[temp_to_int(node -> addr1.contents.var.name)] = 1;
            em_uso[temp_to_int(node -> addr2.contents.var.name)] = 0;
            em_uso[temp_to_int(node -> addr3.contents.var.name)] = 0;

        break;
        case assignOp:
            a1 = initOperando(reg, 0, corrigeTemp(node -> addr1.contents.var.name), node -> addr1.contents.var.scope);
            a2 = initOperando(reg, 0, corrigeTemp(node -> addr2.contents.var.name), node -> addr2.contents.var.scope);
            a3 = initOperando(reg, 0, "$zero", node -> addr2.contents.var.scope);

            insereAss(head, a1, a2, a3, add, location++, 0);

            em_uso[temp_to_int(node -> addr1.contents.var.name)] = 1;
            em_uso[temp_to_int(node -> addr2.contents.var.name)] = 0;
            
        break;
        case allocOp:
            if(strcmp(node -> addr2.contents.var.name, "global")!=0){
            currentSp+= node -> addr3.contents.val;
            paramCount += node -> addr3.contents.val;

            a1 = initOperando(reg, 0, "$sp", "global");
            a2 = initOperando(reg, 0, "$sp", "global");
            a3 = initOperando(imed, node -> addr3.contents.val, NULL, NULL);
            insereAss(head, a1, a2, a3, addi, location++, 0);
            }
            
        break;
        case immedOp:
            a1 = initOperando(reg, 0, corrigeTemp(node -> addr1.contents.var.name), node -> addr1.contents.var.scope);
            a2 = initOperando(reg, 0, "$zero", node -> addr1.contents.var.scope);
            a3 = initOperando(imed, node->addr3.contents.val, NULL, node -> addr1.contents.var.scope);

            insereAss(head, a1, a2, a3, addi, location++, 0);

            em_uso[temp_to_int(node -> addr1.contents.var.name)] = 1;
        break;
        case loadOp: ;
            if(node -> prox -> op == argOp && node -> prox -> addr2.akind == numA
                && strcmp(st_lookup_type(node -> addr2.contents.var.name, node -> addr2.contents.var.scope), "param pointer") != 0)
               break;
            else{
                int offset;
                int fixedLoc = currentSp - lastSp - 1;
                int memloc;
                
                if(strcmp(st_lookup_type(node -> addr2.contents.var.name, node -> addr2.contents.var.scope), "param pointer") == 0
                && (node -> prox -> op != argOp || node -> prox -> addr2.akind != numA) ){
                    offset = st_lookup_offset(node -> addr2.contents.var.name, node -> addr2.contents.var.scope);
                    memloc = fixedLoc-offset;
                    a1 = initOperando(reg, 0, corrigeTemp(node -> addr1.contents.var.name), node -> addr1.contents.var.scope);
                    a2 = initOperando(reg, 0, "$sp", node -> addr1.contents.var.scope);
                    a3 = initOperando(imed, memloc, NULL, NULL);
                    insereAss(head, a1, a2, a3, subi, location++, 0);
                    
                    a3 = initOperando(imed, 0, NULL, NULL);
                    insereAss(head, a1, a1, a3, lw, location++, 0);

                    a3 = initOperando(reg, 0, corrigeTemp(node -> addr3.contents.var.name), node -> addr3.contents.var.scope);
                    insereAss(head, a1, a1, a3, add, location++, 0);

                    a3 = initOperando(imed, 0, NULL, NULL);
                    insereAss(head, a1, a1, a3, lw, location++, 0);
                }else{
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
                }

                em_uso[temp_to_int(node -> addr1.contents.var.name)] = 1;

                break;
            }
        case storeOp: ; //ficar atento as particularidades do store. sintaxe: sw r1, rd, imed note que
        //o endereço de memoria a ser slavado o dado é a soma de r1 com imed, e rd contem o dado a ser salvado.
            int offsetS;
            int fixedLocS = currentSp - lastSp - 1;
            int memlocS;
            if(strcmp(st_lookup_type(node -> addr1.contents.var.name, node -> addr1.contents.var.scope), "param pointer") == 0){
                offsetS = st_lookup_offset(node -> addr1.contents.var.name, node -> addr1.contents.var.scope);
                memlocS = fixedLocS - offsetS;
                char * auxTemp = nextTemp(node -> addr2.contents.var.name);

                a1 = initOperando(reg, 0, auxTemp, node -> addr3.contents.var.scope);
                a2 = initOperando(reg, 0, "$sp", node -> addr3.contents.var.scope);
                a3 = initOperando(imed, memlocS, NULL, NULL);
                insereAss(head, a1, a2, a3, subi, location++, 0);

                a3 = initOperando(imed, 0, NULL, NULL);
                insereAss(head, a1, a1, a3, lw, location++, 0);

                a2 = initOperando(reg, 0, corrigeTemp(node -> addr3.contents.var.name), node -> addr3.contents.var.scope);
                insereAss(head, a1, a1, a2, add, location++, 0);

                a2 = initOperando(reg, 0, corrigeTemp(node -> addr2.contents.var.name), node -> addr2.contents.var.scope);
                insereAss(head, a1, a2, a3, sw, location++, 0);

            }else{
                
                offsetS = st_lookup_offset(node -> addr1.contents.var.name, "global");
                if(offsetS != -1){
                    if(strcmp(node -> addr3.contents.var.name, "$zero") == 0){
                        a1 = initOperando(reg, 0, "$gp", node -> addr1.contents.var.scope);
                        a2 = initOperando(reg, 0, corrigeTemp(node -> addr2.contents.var.name), node -> addr2.contents.var.scope);
                        a3 = initOperando(imed, offsetS, NULL, NULL);
                        insereAss(head, a1, a2, a3, sw, location++, 0);
                    }else{
                        a1 = initOperando(reg, 0, corrigeTemp(node -> addr3.contents.var.name), node -> addr3.contents.var.scope);
                        a2 = initOperando(reg, 0, "$gp", node -> addr1.contents.var.scope);
                        a3 = initOperando(reg, 0, corrigeTemp(node -> addr3.contents.var.name), node -> addr3.contents.var.scope);
                        insereAss(head, a1, a2, a3, add, location++, 0);

                        a3 = initOperando(imed, offsetS, NULL, NULL);
                        insereAss(head, a1, a2, a3, sw, location++, 0);

                    }
                }else{
                    offsetS = st_lookup_offset(node -> addr1.contents.var.name, node -> addr1.contents.var.scope);

                    memlocS = fixedLocS - offsetS;
                    a1 = initOperando(reg, 0, "$sp", node -> addr1.contents.var.scope);
                    a2 = initOperando(reg, 0, "$sp", node -> addr1.contents.var.scope);
                    a3 = initOperando(imed, memlocS, NULL, NULL);
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
                        insereAss(head, a1, a2, a3, sw, location++, 0);
                    }
                    a1 = initOperando(reg, 0, "$sp", node -> addr1.contents.var.scope);
                    a2 = initOperando(reg, 0, "$sp", node -> addr1.contents.var.scope);
                    a3 = initOperando(imed, memlocS, NULL, NULL);
                    insereAss(head, a1, a2, a3, addi, location++, 0);
                }
            }
            em_uso[temp_to_int(node -> addr2.contents.var.name)] = 0;

        break;
        case gotoOp:
            a1 = initOperando(reg, 0, node -> addr1.contents.var.name, node -> addr1.contents.var.scope);
            a2 = initOperando(notInst, 0, NULL, NULL);
            insereAss(head, a1, a2, a2, j, location++, 0);

        break;
        case iffOp:
            a1 = initOperando(reg, 0, corrigeTemp(node -> addr1.contents.var.name), node -> addr1.contents.var.scope);
            a2 = initOperando(reg, 0, "$zero", node -> addr1.contents.var.scope);
            a3 = initOperando(reg, 0, node -> addr2.contents.var.name, node -> addr2.contents.var.scope);

            insereAss(head, a1, a2, a3, beq, location++, 0);
            em_uso[temp_to_int(node -> addr1.contents.var.name)] = 0;
        break;

        //Retorno de função carregado para o registardor específico
        case retOp:
            if(node -> addr1.akind != nop){
                a1 = initOperando(reg, 0, "$ret", node -> addr1.contents.var.scope);
                a2 = initOperando(reg, 0, corrigeTemp(node -> addr1.contents.var.name), node -> addr1.contents.var.scope);
                a3 = initOperando(imed, 0, NULL, NULL);
                insereAss(head, a1, a2, a3, addi, location++, 0);
            }

        break;
        case funOp:
            a1 = initOperando(reg, 0, node -> addr2.contents.var.name, node -> addr2.contents.var.scope);
            a2 = initOperando(notInst, 0, NULL, NULL);
            insereAss(head, a1, a2, a2, notInst, location++, 1);

            lastSp = currentSp;
           
            for(int i = 0; i < 17; i++)
                em_uso[i] = 0;
    
        break;
        case endOp:
            a1 = initOperando(reg, 0, "$sp", NULL);
            a2 = initOperando(imed, paramCount, NULL, NULL);
            insereAss(head, a1, a1, a2, subi, location++, 0);
            paramCount = 0;
            lastParam = 0;
            a1 = initOperando(notInst, 0, NULL, NULL);
            insereAss(head, a1, a1, a1, jr, location++, 0);
        break;
        case paramOp:
            a1 = initOperando(reg, 0, "$sp", node -> addr3.contents.var.name);
            a2 = initOperando(imed, 1, NULL, NULL);
            insereAss(head, a1, a1, a2, addi, location++, 0);
            currentSp++;

            paramCount++;

            char *paramRegister = (char*)malloc(4*sizeof(char));
            sprintf(paramRegister, "$a%d", lastParam%7); // verificar tratamento de estoro de registradores de argumento depois
            lastParam++;

            a2 = initOperando(reg, 0, paramRegister, node -> addr3.contents.var.name);
            a3 = initOperando(imed, 0, NULL, NULL);
            insereAss(head, a1, a2, a3, sw, location++, 0);

        break;
        case callOp:

            empilha_temporario(head, em_uso);
            //empilha RA
            a1 = initOperando(reg, 0, "$sp", NULL);
            a2 = initOperando(imed, 1, NULL, NULL);
            insereAss(head, a1, a1, a2, addi, location++, 0);

            a2 = initOperando(reg, 0, "$ra", NULL);
            a3 = initOperando(imed, 0, NULL, NULL);
            insereAss(head, a1, a2, a3, sw, location++, 0);
            ////
            a1 = initOperando(reg, 0, node -> addr2.contents.var.name, node -> addr2.contents.var.scope);
            a2 = initOperando(notInst, 0, NULL, NULL);
            
            insereAss(head, a1, a2, a2, jal, location++, 0);
            //desempilha RA
            a1 = initOperando(reg, 0, "$ra", NULL);
            a2 = initOperando(reg, 0, "$sp", NULL);
            insereAss(head, a1, a2, a3, lw, location++, 0);

            a3 = initOperando(imed, 1, NULL, NULL);
            insereAss(head, a2, a2, a3, subi, location++, 0);
            ///
            desempilha_temporario(head, em_uso);

            //atriguição de retorno de função
            if(strcmp(st_lookup_type(node -> addr2.contents.var.name, "global"), "void") != 0){
                a1 = initOperando(reg, 0, corrigeTemp(node -> addr1.contents.var.name),node -> addr1.contents.var.scope);
                a2 = initOperando(reg, 0, "$ret", NULL);
                a3 = initOperando(reg, 0, "$zero", NULL);
                insereAss(head, a1, a2, a3, add, location++, 0);

                em_uso[temp_to_int(node -> addr1.contents.var.name)] = 1;
            }
            lastArg = 0;
        // Empilha todos os registradores usados.
        //tratar e entender como funciona o registrador de endereço.
        //tratar o retorno da função, transferindo o valor do registrador de retorno pro registrador temporario
       

        break;
        case argOp: ;
        //    printf("%s\n", node -> addr1.contents.var.scope);
        // se for vetor, realizar um tratamento diferente
            char *argRegister = (char*)malloc(4*sizeof(char));
            sprintf(argRegister, "$a%d", lastArg%7); // verificar tratamento de estoro de registradores de argumento depois
            lastArg++;
            if(node -> addr2.akind == numA &&
                    strcmp(st_lookup_type(node -> ant -> addr2.contents.var.name, node -> addr1.contents.var.scope), "param pointer") != 0){// se node -> addr2.akind for 0, o escopo da variável a ser
                int vecLoc;                 // passada como parametro é o global, e se for 1, o escopo é o atual
                if(node -> addr2.contents.val == 0){// encontrar um jeito melhor de reconhecer o escopo global
                    vecLoc = node -> addr3.contents.val;
                    
                } else {
                    vecLoc = lastSp + node -> addr3.contents.val+1;                   
                }
                a1 = initOperando(reg, 0, argRegister, node -> addr1.contents.var.scope);
                a2 = initOperando(reg, 0, "$zero", node -> addr1.contents.var.scope);
                a3 = initOperando(imed, vecLoc, NULL, NULL);
                insereAss(head, a1, a2, a3, addi, location++, 0);

            }else {
                a1 = initOperando(reg, 0, argRegister, node -> addr1.contents.var.scope);
                a2 = initOperando(reg, 0, corrigeTemp(node -> addr1.contents.var.name), node -> addr1.contents.var.scope);
                a3 = initOperando(imed, 0, NULL, NULL);
                insereAss(head, a1, a2, a3, addi, location++, 0);
            }

        break;
        case labOp:
            a1 = initOperando(reg, 0, node -> addr1.contents.var.name, node -> addr2.contents.var.scope);
            a2 = initOperando(nothing, 0, NULL, NULL);
            a3 = a2;

            insereAss(head, a1, a2, a3, notInst, location++, 1);

        break;
        case inputOp:

            a1 = initOperando(notInst, 0, NULL, NULL);
            insereAss(head, a1, a1, a1, in, location++, 0);

            a1 = initOperando(reg, 0, corrigeTemp(node -> addr1.contents.var.name), node -> addr1.contents.var.scope);
            a2 = initOperando(reg, 0, "$RE", node -> addr1.contents.var.scope);
            a3 = initOperando(imed, 0, NULL, NULL);

            insereAss(head, a1, a2, a3, addi, location++, 0);

            em_uso[temp_to_int(node -> addr1.contents.var.name)] = 1;

        break;
        case outputOp:
            a1 = initOperando(reg, 0, "$a0", NULL);
            a2 = initOperando(reg, 0, "$zero", NULL);
            a3 = initOperando(notInst, 0, NULL, NULL);
            insereAss(head, a1, a2, a3, out, location++, 0);
            lastArg = 0;

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

    aux1 = initOperando(reg, 0, "main", NULL);
    aux2 = initOperando(notInst, 0, NULL, NULL);
    insereAss(assHead, aux1, aux2, aux2, jal, location++, 0);

    aux1 = initOperando(notInst, 0, NULL, NULL);
    insereAss(assHead, aux1, aux1, aux1, breakk, location++, 0);


    aux = intercode -> head -> prox;
    while(aux -> location != 0){
        lineGen(assHead, aux);
        aux = aux -> prox;
    }
}
