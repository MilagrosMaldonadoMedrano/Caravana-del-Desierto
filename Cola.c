#include "Cola.h"



void crearCola(tCola* pc)
{
    pc->pri=NULL;
    pc->ult=NULL;
}
int ponerEnCola(tCola* pc,const void* dato,size_t tam)
{
    tNodo* nuevo=malloc(sizeof(tNodo));

    if(!nuevo)
        return COLA_LLENA;

    nuevo->info=malloc(tam);

    if(!nuevo->info)
    {
        free(nuevo);
        return COLA_LLENA;
    }
    nuevo->tamInfo=tam;
    memcpy(nuevo->info,dato,nuevo->tamInfo);
    nuevo->sig=NULL;



    if(pc->ult)
        pc->ult->sig=nuevo;
    else
        pc->pri=nuevo;

    pc->ult=nuevo;

    return TODO_OK;
}
int sacarDeCola(tCola* pc,void* dato,size_t tam)
{
    tNodo* eliminar=pc->pri;
    if(!eliminar)
        return COLA_VACIA;

    memcpy(dato,eliminar->info,minimo(tam,eliminar->tamInfo));
    pc->pri=eliminar->sig;

    if(!pc->pri)
        pc->ult=NULL;
    free(eliminar->info);
    free(eliminar);
    return TODO_OK;
}
int frenteDeCola(const  tCola* pc,void* dato,size_t tam)
{
    tNodo* primero=pc->pri;

    if(!primero)
        return COLA_VACIA;

    memcpy(dato,primero->info,minimo(tam,primero->tamInfo));

    return TODO_OK;
}
int colaLLena(const tCola* pc,size_t tam)
{
    return COLA_NO_LLENA;
}
int colaVacia(const tCola* pc)
{
    return !pc->pri;
}
void vaciarCola(tCola* pc)
{
    tNodo* aux=NULL;
    while(pc->pri)
    {

        aux=pc->pri;
        pc->pri=aux->sig;

        free(aux->info);
        free(aux);
    }

    pc->ult=NULL;
}



