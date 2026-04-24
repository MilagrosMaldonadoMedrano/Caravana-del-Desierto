#ifndef COLA_H_INCLUDED
#define COLA_H_INCLUDED
#include <stddef.h>

#define TODO_OK 1
#define COLA_LLENA 2
#define COLA_VACIA 3
#define COLA_NO_LLENA 4



#define minimo(x,y)((x)<(y)?(x):(y))

typedef struct sNodo
{
    void* info;
    unsigned tamInfo;
    struct sNodo* sig;
}tNodo;

typedef struct
{
    tNodo* pri;
    tNodo* ult;
}tCola;

void crearCola(tCola* pc);
int ponerEnCola(tCola* pc,const void* dato,size_t tam);
int sacarDeCola(tCola* pc,void* dato,size_t tam);
int frenteDeCola(const  tCola* pc,void* dato,size_t tam);
int colaLLena(const tCola* pc,size_t tam);
int colaVacia(const tCola* pc);
void vaciarCola(tCola* pc);

#endif // COLA_H_INCLUDED
