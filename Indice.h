#ifndef INDICES_H_INCLUDED
#define INDICES_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include "Arbol.h"
#include "Jugadores.h"

#define NOM_ARCH_INDICE_JUGADORES "indice_jugadores.idx"

typedef struct
{
    void* clave;
    unsigned tamClave;
    unsigned pos;
} tIndice;

typedef struct
{
    void* clave;
    void* clave2;
    unsigned tamClave;
    unsigned tamClave2;
    unsigned pos;
} tIndiceCompuesto;



int compararIndiceJugador(const void *a, const void *b);
int cargarIndiceJugadores(const char* nomArchIndice, tArbol* arbolJugadores);
int indexarArchivoJugadores(const char* nomArchJugadores, tArbol* arbolJugadores);
int guardarIndiceJugadores(const char* nomArchIndice, const tArbol* arbolJugadores);
void guardarNodoIndiceJugador(const void *info, void *params);

// testing
void mostrarNodoIndiceJugador(const void *info);
void mostrarIndiceJugadores(const tArbol* arbolJugadores);


///indice de puntaje
int compararPorPuntaje(const void *a, const void *b);
int indexarArchivoJugadoresPorPuntaje(const char* nomArchJugadores, tArbol* arbol);

void mostrarJugadorRanking(const void* j);
void mostrarJugadorDesdeRanking(const void* info, void* params);


#endif // INDICES_H_INCLUDED
