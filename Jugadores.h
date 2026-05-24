#ifndef JUGADORES_H_INCLUDED
#define JUGADORES_H_INCLUDED

#include "main.h"
#include "Archivo.h"
#include "Lista.h"
#include "Menu.h"

#define MAX_NOMBRE 5

#define NOM_ARCH_JUGADORES "jugadores.dat"

typedef struct
{
    int idJugador;
    char nombre[MAX_NOMBRE];
    unsigned totalPuntos;
    unsigned partidasJugadas;
} tJugador;

int buscarJugador(const char* nomArch, const char* nombre, tJugador* jug);
int altaJugador(const char* nomArch, tJugador* jug);
int actualizarJugador(const char* nomArch, tJugador* jug);
void mostrarJugador(const void* j);
int obtenerUltimoID(const char* nomArch);

void mostrarArchivoJugadores(const char* nomArch);

#endif // JUGADORES_H_INCLUDED
