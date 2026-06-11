#ifndef JUGADORES_H_INCLUDED
#define JUGADORES_H_INCLUDED

#include "main.h"
#include "Archivo.h"
#include "Lista.h"
#include "Menu.h"
#include "Partidas.h"
#include "Arbol.h"

#define MAX_NOMBRE 5
#define MAX_JUGADORES 100
#define MAX_NICK 15

#define NOM_ARCH_JUGADORES "jugadores.dat"

typedef struct
{
    int idJugador;
    char nombre[MAX_NOMBRE];
    unsigned totalPuntos;
    unsigned partidasJugadas;
    char nickName[MAX_NICK];
}tJugador;

int obtenerUltimoID(const char* nomArch);
void mostrarArchivoJugadores(const char* nomArch);
int buscarJugador(tArbol* arbolJugadores, const char* nomArch, const char* nickname, tJugador* jug);
int altaJugador(tArbol* arbolJugadores, const char* nomArch, tJugador* jug);
int actualizarJugador(tArbol* arbolJugadores, const char* nomArch, tJugador* jug);
void mostrarJugador(const void* j);

int cargarJugadores(const char* nomArch, tJugador* vec);
void ordenarJugadores(tJugador* vec, int cantJugadores);
int compararJugadores(const void* j1, const void* j2);
void mostrarVectorJugadores(tJugador* vec, int cantJugadores);

#endif // JUGADORES_H_INCLUDED
