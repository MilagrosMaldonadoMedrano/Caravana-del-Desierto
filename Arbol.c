#include "Arbol.h"

void crearArbol(tArbol *p)
{
    *p = NULL;
}
///modificar para que acepte una accion si hay duplicado
int insertarEnArbol(tArbol *p, const void *d, unsigned cantBytes, int (*cmp)(const void*, const void*))
{
    tNodoArbol *nue;
    int comp;

    while (*p)
    {
        if ((comp = cmp(d, (*p)->info)) < 0)
            p = &(*p)->izq;
        else if (comp > 0)
            p = &(*p)->der;
        else
            return HAY_DUPLICADO;
    }

    nue = (tNodoArbol*) malloc(sizeof(tNodoArbol));
    if (!nue) return SIN_MEM;

    nue->info = malloc(cantBytes);
    if (!nue->info)
    {
        free(nue);
        return SIN_MEM;
    }
    memcpy(nue->info, d, cantBytes);
    nue->tamInfo = cantBytes;
    nue->izq = NULL;
    nue->der = NULL;
    *p = nue;
    return TODO_BIEN;
}



int insertarEnArbolAccion(tArbol *p, const void *d, unsigned cantBytes, int (*cmp)(const void*, const void*),void (*accion)(void*, const void*))
{
    tNodoArbol *nue;
    int comp;

    while (*p)
    {
        if ((comp = cmp(d, (*p)->info)) < 0)
            p = &(*p)->izq;
        else if (comp > 0)
            p = &(*p)->der;
        else
        {
            accion((*p)->info,d);
            return HAY_DUPLICADO;
        }

    }

    nue = (tNodoArbol*) malloc(sizeof(tNodoArbol));
    if (!nue) return SIN_MEM;

    nue->info = malloc(cantBytes);
    if (!nue->info)
    {
        free(nue);
        return SIN_MEM;
    }
    memcpy(nue->info, d, cantBytes);
    nue->tamInfo = cantBytes;
    nue->izq = NULL;
    nue->der = NULL;
    *p = nue;
    return TODO_BIEN;
}




// Recorrido preOrden
void recorrerPreOrden(const tArbol *p, void (*accion)(const void *))
{
    if (*p)
    {
        accion((*p)->info);
        recorrerPreOrden(&(*p)->izq, accion);
        recorrerPreOrden(&(*p)->der, accion);
    }
}

// Recorrido inOrden
void recorrerInOrden(const tArbol *p, void (*accion)(const void *))
{
    if (*p)
    {
        recorrerInOrden(&(*p)->izq, accion);
        accion((*p)->info);
        recorrerInOrden(&(*p)->der, accion);
    }
}

void recorrerInOrdenParam(const tArbol *p, void *param, void (*accion)(const void *, void *))
{
    if (*p)
    {
        recorrerInOrdenParam(&(*p)->izq, param, accion);
        accion((*p)->info, param);
        recorrerInOrdenParam(&(*p)->der, param, accion);
    }
}

///se llama inOrden Inverso
void recorrerDRI(const tArbol* pa, void *param, void (*accion)(const void *, void *))
{
    if(!*pa)
        return;

    recorrerDRI(&(*pa)->der,param, accion);
    accion((*pa)->info,param);
    recorrerDRI(&(*pa)->izq,param, accion);
}

void vaciarArbol(tArbol *p)
{
    if (*p)
    {
        vaciarArbol(&(*p)->izq);
        vaciarArbol(&(*p)->der);
        free((*p)->info);
        free(*p);
        *p = NULL;
    }
}

int buscarEnArbol(const tArbol *p, void *dato, unsigned tamDato, int (*cmp)(const void*, const void*))
{
    if (!*p) return 0;

    int comp = cmp(dato, (*p)->info);

    if (comp == 0)
    {
        memcpy(dato, (*p)->info, tamDato);
        return 1;
    }

    return comp < 0
           ? buscarEnArbol(&(*p)->izq, dato, tamDato, cmp)
           : buscarEnArbol(&(*p)->der, dato, tamDato, cmp);
}

void recorrerPreOrdenParam(const tArbol *p, void *param,void (*accion)(const void *, void *))
{
    if (*p)
    {
        accion((*p)->info, param);
        recorrerPreOrdenParam(&(*p)->izq, param, accion);
        recorrerPreOrdenParam(&(*p)->der, param, accion);
    }
}

int arbolVacio(const tArbol* pa)
{
    if(!*pa)
        return ARBOL_VACIO;
    return ARBOL_NO_VACIO;
}
