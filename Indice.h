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

int compararIndiceJugador(const void *a, const void *b);
int cargarIndiceJugadores(const char* nomArchIndice, tArbol* arbolJugadores);
int indexarArchivoJugadores(const char* nomArchJugadores, tArbol* arbolJugadores);
int guardarIndiceJugadores(const char* nomArchIndice, const tArbol* arbolJugadores);
void guardarNodoIndiceJugador(const void *info, void *params);

// testing
void mostrarNodoIndiceJugador(const void *info);
void mostrarIndiceJugadores(const tArbol* arbolJugadores);

#endif // INDICES_H_INCLUDED
