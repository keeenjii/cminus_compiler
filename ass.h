typedef enum {
    add, sub, mult, divv, sra, sla, and, or, xor, eq, cmp, not, 
    addi, subi, lw, sw, andi, ori, xori, cmpi, jr, beq, bne, 
    j, jal, 
    breakk, out, in,
    notInst

}Instrucao;

typedef enum{
    reg, imed, nothing
}tipoOperando;

typedef struct {
    tipoOperando kind;
    union{
        int imediato;
        struct {
            char* name;
            char* scope;
        }var;
    }contents;

}Operando;

typedef struct assItem *assApontador;

typedef struct assItem{
    int islabel;

    Instrucao inst;

    Operando op1, op2, op3;
    int location;

    assApontador prox, ant;

}assCelula;

typedef struct{
    assApontador head;
    int tamanho;
}assHead;

assHead *initAss();

Operando initOperando(tipoOperando kind, int imediato, char* name, char* scope);

void insereAss(assHead *a, Operando op1, Operando op2, Operando op3, Instrucao inst, int location, int islabel);

char * printOp(Operando op);

int nDigitos(long a);

void printAss(assHead *head);