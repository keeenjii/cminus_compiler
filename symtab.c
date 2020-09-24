#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"
#define SIZE 211
#define SHIFT 4


static int hash (char* name, char* Escopo)
{ 
    int temp = 0;
    int i = 0;
    while (name[i] != '\0')
    { 
        temp = ((temp << SHIFT) + name[i]) % SIZE;
        ++i;
    }
    i = 0;
    while (Escopo[i] != '\0')
    { 
        temp = ((temp << SHIFT) + Escopo[i]) % SIZE;
        ++i;
    }
    return temp;
}

typedef struct LineListRec
{ 
    int lineno;
    struct LineListRec * next;
    
} * LineList;

typedef struct BucketListRec
{ 
     char* name;
     LineList lines;
     int memloc ;
     int size; 
	 char* Escopo;
	 char* typeID;
	 char* typeData; 
     struct BucketListRec * next;
     
 } * BucketList;

static BucketList hashTable[SIZE];

void st_insert( char * name, int lineno, int loc, char* Escopo, 
		char* typeID, char* typeData, int size)
{ 
 
    int h = hash(name, Escopo);
    BucketList l =  hashTable[h];
    while ((l != NULL) && (strcmp(name,l->name) != 0) && (strcmp(Escopo,l->Escopo) != 0)) 
            l = l->next;
    if (l == NULL) /* variable not yet in table */
    { 
        l = (BucketList) malloc(sizeof(struct BucketListRec));
        l->name = name;
        l->lines = (LineList) malloc(sizeof(struct LineListRec));
        l->lines->lineno = lineno;
        l->memloc = loc;
        l -> size = size;
        l->lines->next = NULL;
        l->Escopo = Escopo;
        l->typeID = typeID;
        l->typeData = typeData;
        l->next = hashTable[h];
        hashTable[h] = l; 
    }
    else
    {
        LineList t = l->lines;
        while (t->next != NULL) 
            t = t->next;
        t->next = (LineList) malloc(sizeof(struct LineListRec));
        t->next->lineno = lineno;
        t->next->next = NULL;  
    }
} 

int st_lookup (char* name, char* Escopo, char* typeID)
{ 
  int h = hash(name, Escopo);	
  BucketList l =  hashTable[h];
  
  while ((l != NULL) && (strcmp(name,l->name) != 0) && (strcmp(Escopo,l->Escopo) != 0)){
    
        l = l->next;
  }

  if (l == NULL){ 
      return -1;
  }else{
      
      return l->memloc;
  }
}

/*char* st_lookup_typeID (char* name, char* Escopo){
  int h = hash(name, Escopo);	
  BucketList l =  hashTable[h];
  
  while ((l != NULL) && (strcmp(name,l->name) != 0)){
    
        l = l->next;
  }

  if (l == NULL){ 
      return NULL;
  }else{
      
      return l->typeID;
  }
}*/

char* st_lookup_type (char* name, char* Escopo)
{ 
  int h = hash(name, Escopo);	
  BucketList l =  hashTable[h];
  while ((l != NULL) && (strcmp(name,l->name) != 0) && (strcmp(Escopo,l->Escopo) != 0)){
        l = l->next;
  }
  if (l == NULL) 
      return "null";
  else 
      return l->typeData;
}

int st_lookup_offset (char* name, char* Escopo)
{ 
  int h = hash(name, Escopo);	
  BucketList l =  hashTable[h];
  while ((l != NULL) && (strcmp(name,l->name) != 0) && (strcmp(Escopo,l->Escopo) != 0)){
        l = l->next;
  }
  if (l == NULL) 
      return -1;
  else 
      return l->memloc;
}

int st_lookup_size(char *name, char *Escopo){
  int h = hash(name, Escopo);	
  BucketList l =  hashTable[h];
  while ((l != NULL) && (strcmp(name,l->name) != 0) && (strcmp(Escopo,l->Escopo) != 0)){
        l = l->next;
					
  }
  if (l == NULL) 
      return -1;
  else 
      return l->size;

}

void printSymTab(FILE * listing)
{ int i;
  fprintf(listing,"Offset         Size           Name           Escopo         TypeID         TypeData       Line Numbers\n");
  fprintf(listing,"-------------  -------------  -------------  -------------  -------------  -------------  -------------\n");
  for (i=0;i<SIZE;++i)
  { if (hashTable[i] != NULL)
    { BucketList l = hashTable[i];
      while (l != NULL)
      { LineList t = l->lines;
        fprintf(listing,"%-14d ",l->memloc);
        fprintf(listing,"%-14d ", l->size);
        fprintf(listing,"%-13s  ",l->name);
		fprintf(listing,"%-13s  ",l->Escopo);
        fprintf(listing,"%-13s  ",l->typeID);
		fprintf(listing,"%-11s  ",l->typeData);	
        while (t != NULL)
        { fprintf(listing,"%4d ",t->lineno);
          t = t->next;
        }
        fprintf(listing,"\n");
        l = l->next;
      }
    }
  }
} 