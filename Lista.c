#include"Lista.h"
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#define minimo(a,b)(a<b?a:b)

///Para poder empezar a pensar como hacer las funciones de una lista doblemente enlazada circular propuse que el puntero del main apunte
///al final de la lista.



void crearLista(tLista* pl)
{
    *pl=NULL;
}

void vaciarLista(tLista* pl)
{
    tNodoD* primero=*pl;

    if(!primero)
            return;

    tNodoD* eliminar=primero->sig;
    while(eliminar!=primero)
    {
        tNodoD* auxiliar=eliminar->sig;
        free(eliminar->info);
        free(eliminar);
        eliminar=auxiliar;
    }
    free(eliminar->info);
    free(eliminar);
    *pl=NULL;
}
/*
int insertarAlFinal(tLista* pl,const void* elem,unsigned cantBytes)
{
    tNodoD* nuevo=malloc(sizeof(tNodoD));

    if(!nuevo)
        return LISTA_LLENA;

    nuevo->info=malloc(cantBytes);
    if(!nuevo->info)
    {
        free(nuevo);
        return LISTA_LLENA;
    }


    memcpy(nuevo->info,elem,cantBytes);
    nuevo->tamElem=cantBytes;

    if(!*pl)//primer elemento
    {
        *pl=nuevo;
        nuevo->sig=nuevo;
        nuevo->ant=nuevo;
    }
    else
    {
        tNodoD* primero = (*pl)->sig;

        nuevo->sig=primero;
        primero->ant=nuevo;
        (*pl)->sig=nuevo;
        nuevo->ant=(*pl);
        *pl=nuevo;
    }

    return TODO_OK;

}
*/

int insertarAlComienzo(tLista* pl,const void* elem,unsigned cantBytes)
{
    tNodoD* nuevo=malloc(sizeof(tNodoD));

    if(!nuevo)
        return LISTA_LLENA;

    nuevo->info=malloc(cantBytes);
    if(!nuevo->info)
    {
        free(nuevo);
        return LISTA_LLENA;
    }

     memcpy(nuevo->info,elem,cantBytes);
     nuevo->tamElem=cantBytes;


     if(!*pl)//primer nodo
     {
        *pl=nuevo;
        nuevo->sig=nuevo;
        nuevo->ant=nuevo;
     }
    else
    {
        tNodoD* primero=(*pl)->sig;

        nuevo->sig=primero;
        nuevo->ant=(*pl);

        (*pl)->sig=nuevo;
        primero->ant=nuevo;
        //en este caso no actualizo pl porque siempre debe apuntar al ultimo y aca agregue un nodo al inicio
    }


    return TODO_OK;
}

int listaVacia(tLista* pl)
{
    if(*pl==NULL)
        return LISTA_VACIA;
    return LISTA_NO_VACIA;
}

int listaLlena(tLista* lista,void* elem,unsigned cantBytes)
{
    return LISTA_NO_LLENA;
}


/*int recorrerDeIzqADer(tLista* pl,Accion accion)
{
    tNodoD* actual=(*pl)->sig;//actual esta en el primero

    if(!actual)
        return LISTA_VACIA;




    while(actual!=*pl) //hasta que actual no sea el ultimo
    {
        accion(actual->info,NULL);
        actual= actual->sig;
    }

    accion(actual->info,NULL);

    return TODO_OK;
}*/



int recorrerDeIzqADer(tLista* pl, Accion accion, const void* extra)
{
    if(!*pl) return LISTA_VACIA;

    tNodoD* actual = (*pl)->sig;

    while(actual != *pl)
    {
        accion(actual->info, extra);
        actual = actual->sig;
    }
    accion(actual->info, extra);

    return TODO_OK;
}


int recorrerDeDerAIzq(tLista* pl,Accion accion)
{
    tNodoD* actual=*pl;

    if(!actual)
        return LISTA_VACIA;




    while(actual!= (*pl)->sig)//mientras que actual no sea el inicio
    {
        accion(actual->info,NULL);
        actual= actual->ant;
    }

    accion(actual->info,NULL);

    return TODO_OK;
}


int listaInsertarAlFinal(tLista* pl, const void* dato, unsigned tamElem)
{
    tNodoD* anterior;
    tNodoD* siguiente;

    tNodoD* nuevo = malloc(sizeof(tNodoD));
    if(!nuevo)
        return LISTA_LLENA;

    nuevo->info = malloc(tamElem);

    if(!nuevo->info)
    {
        free(nuevo);
        return LISTA_LLENA;
    }

    memcpy(nuevo->info, dato, tamElem);
    nuevo->tamElem = tamElem;

    /// caso lista vacia
    if(!*pl)
    {
        nuevo->sig = nuevo;
        nuevo->ant = nuevo;
        *pl = nuevo;

        return TODO_OK;
    }

    anterior=(*pl);
    siguiente=(*pl)->sig;//primero

    nuevo->ant=anterior;
    nuevo->sig=siguiente;

    anterior->sig=nuevo;
    siguiente->ant=nuevo;


    *pl = nuevo;
    return TODO_OK;
}



int listaInsertarOrdenado(tLista* pl,const void* dato,unsigned tamElem,Cmp tCmp,Accion tAccion)
{
    tNodoD* actual;
    tNodoD* anterior;

    tNodoD* nuevo = malloc(sizeof(tNodoD));
    if(!nuevo)
        return LISTA_LLENA;

    nuevo->info = malloc(tamElem);

    if(!nuevo->info)
    {
        free(nuevo);
        return LISTA_LLENA;
    }

    memcpy(nuevo->info, dato, tamElem);
    nuevo->tamElem = tamElem;

    /// caso lista vacia
    if(!*pl)
    {
        nuevo->sig = nuevo;
        nuevo->ant = nuevo;
        *pl = nuevo;

        return TODO_OK;
    }

    actual = (*pl)->sig; /// hago que el actual sea el primero

    while(actual != *pl && tCmp(dato, actual->info) > 0)
    {
        actual = actual->sig;
    }

    // llegue al ultimo y todavia es mayor
    if(actual == *pl && tCmp(dato, actual->info) > 0)
        actual = actual->sig;

    // duplicado
    if(tCmp(dato, actual->info) == 0)
    {
        tAccion(actual->info, dato);

        free(nuevo->info);
        free(nuevo);

        return DUPLICADO;
    }

    anterior = actual->ant;

    nuevo->sig = actual;
    nuevo->ant = anterior;

    anterior->sig = nuevo;
    actual->ant = nuevo;

    // si inserte después del último
    if(anterior == *pl)
        *pl = nuevo;

    return TODO_OK;
}

///si la lista esta desordenada admite repetidos
int eliminarListaDesordenadaPorClave(tLista* pl, void* dato, unsigned tamElem,Cmp cmp,Accion accion)
{

    if(!*pl)
        return LISTA_VACIA;

    tNodoD* actual=(*pl)->sig;
    int primerAparicion=VERDADERO;

    do
    {
        tNodoD* auxAnterior;
        tNodoD* auxSiguiente;

        if(cmp(dato,actual->info)==0)//hay coincidencia
        {
            if(primerAparicion)//si es el primerp (lista que acepta repetidos) lo copia
            {
                memcpy(dato,actual->info,minimo(tamElem,actual->tamElem));
                primerAparicion=FALSO;
            }
            else               //si no es el primero lo actualiza
                accion(dato,actual->info);

            ///ACOMODO LOS PUNTEROS

            auxSiguiente=actual->sig;
            auxAnterior=actual->ant;

            if(auxAnterior!=actual)///quiere decir que no es el unico nodo
            {
                auxSiguiente->ant=auxAnterior;
                auxAnterior->sig=auxSiguiente;
            }
            else                         ///unico nodo tiene que apuntar a null                 no me gusta como quedo esto!
            {
                free(actual->info);
                free(actual);

                *pl = NULL;
                return TODO_OK;
            }

            if(actual == *pl) //si elimino el ultimo tenfo que modificar el puntero del main
                *pl = actual->ant;


            free(actual->info);
            free(actual);
        }
        actual=actual->sig;


    }while(actual!=(*pl)->sig);///mientras que el actual sea distinto del primero

    if(primerAparicion==VERDADERO)
        return NO_ENCONTRADO;

    return TODO_OK;
}




void* buscarElementoLista(tLista* pl, void* dato,Cmp cmp)
{
    if(!*pl)
        return NULL;

    tNodoD* actual=(*pl);

    do
    {

        if(cmp(dato,actual->info)==0)
            return actual->info;

        actual=actual->sig;

    }while(actual!=(*pl));


    if(cmp(dato,actual->info)==0)
        return actual->info;

    return NULL;

}



unsigned listaCantidadElementos(const tLista* pl)
{
    tNodoD* act = *pl;
    unsigned cant = 0;

    if(!act)
        return 0;

    do
    {
        cant++;
        act = act->sig;

    }while(act != *pl);

    return cant;
}


int listaEliminarPrimerElemento(tLista* pl, void* dato, unsigned tamElem)
{
    if(!*pl)
        return LISTA_VACIA;

    tNodoD* primero=(*pl)->sig;//le asigno el primero
    tNodoD* siguiente;
    tNodoD* anterior;

    if(primero==*pl)///unico nodo
    {
        memcpy(dato,primero->info,minimo(tamElem,primero->tamElem));
        free(primero->info);
        free(primero);

        *pl=NULL;
        return TODO_OK;
    }

    anterior=*pl;
    siguiente=primero->sig;

    anterior->sig=siguiente;
    siguiente->ant=anterior;


    memcpy(dato,primero->info,minimo(tamElem,primero->tamElem));
    free(primero->info);
    free(primero);

    return TODO_OK;

}

///si la lista esta ordenada no admite repetidos
int eliminarListaOrdenadaPorClave(tLista* pl, void* dato, unsigned tamElem,Cmp cmp)
{
    if(!*pl)
        return LISTA_VACIA;

    tNodoD* actual=(*pl)->sig;

    while(actual!=(*pl) && cmp(dato,actual->info)>0)
    {
        actual=actual->sig;
    }///mientras que el actual sea distinto del primero




    if(cmp(dato,actual->info)==0)//hay coincidencia
    {
        tNodoD* auxAnterior;
        tNodoD* auxSiguiente;

        memcpy(dato,actual->info,minimo(tamElem,actual->tamElem));

        ///ACOMODO LOS PUNTEROS
        auxSiguiente=actual->sig;
        auxAnterior=actual->ant;

        if(auxAnterior!=actual)///quiere decir que no es el unico nodo
        {
            auxSiguiente->ant=auxAnterior;
            auxAnterior->sig=auxSiguiente;
            if(actual == *pl) //si elimino el ultimo tenfo que modificar el puntero del main
                *pl = actual->ant;
        }
        else                         ///unico nodo tiene que apuntar a null
        {
            *pl = NULL;
        }

        free(actual->info);
        free(actual);
    }

    else
        return NO_ENCONTRADO;


    return TODO_OK;
}

void ordenarLista(tLista *pl, Cmp cmp)
{
    if(!*pl || (*pl)->sig == *pl) ///si la lista esta vacia o hay un solo nodo salgo
        return;

    int huboCambio;
    tNodoD *act;
    tNodoD *inicio = *pl;

    do
    {
        huboCambio = 0; ///si es 0 no hubo intercambio, si es 1 si
        act = inicio;

        do
        {
            tNodoD *sig = act->sig;

            if(act != sig && cmp(act->info, sig->info) > 0)
            {
                void *auxInfo = act->info;
                unsigned auxTam = act->tamElem;

                act->info = sig->info;
                act->tamElem = sig->tamElem;

                sig->info = auxInfo;
                sig->tamElem = auxTam;

                huboCambio = 1;
            }

            act = act->sig;

        } while(act->sig != inicio);

    } while(huboCambio);
}
