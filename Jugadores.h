#ifndef JUGADORES_H_INCLUDED
#define JUGADORES_H_INCLUDED

#include "main.h"

#define ARCH_JUGADORES "jugadores.dat"

typedef struct
{
    int idJugador;
    char nombre[30];
    unsigned totalPuntos;
    unsigned partidasJugadas;
} tJugador;

int buscarJugador(const char* nomArch, const char* nombre, tJugador* jug);
int altaJugador(const char* nomArch, tJugador* jug);
int actualizarJugador(const char* nomArch, tJugador* jug);
void mostrarJugador(const void* j);


#endif // JUGADORES_H_INCLUDED
