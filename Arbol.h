#ifndef ARBOL_H_INCLUDED
#define ARBOL_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define HAY_DUPLICADO -2
#define SIN_MEM -1
#define TODO_BIEN 0

#define ARBOL_VACIO -3
#define ARBOL_NO_VACIO 3


typedef struct sNodoArbol {
    void *info;
    unsigned tamInfo;
    struct sNodoArbol *izq;
    struct sNodoArbol *der;
} tNodoArbol;

typedef tNodoArbol *tArbol;

void crearArbol(tArbol *p);
void vaciarArbol(tArbol *p);
int insertarEnArbol(tArbol *p, const void *d, unsigned cantBytes, int (*cmp)(const void*, const void*));
void recorrerInOrden(const tArbol *p, void (*accion)(const void *));
void recorrerInOrdenParam(const tArbol *p, void *param, void (*accion)(const void *, void *));
int buscarEnArbol(const tArbol *p, void *dato, unsigned tamDato, int (*cmp)(const void*, const void*));

void recorrerDRI(const tArbol* pa, void *param, void (*accion)(const void *, void *));
int insertarEnArbolAccion(tArbol *p, const void *d, unsigned cantBytes, int (*cmp)(const void*, const void*),void (*accion)(void*, const void*));
int arbolVacio(const tArbol* pa);


#endif // ARBOL_H_INCLUDED
