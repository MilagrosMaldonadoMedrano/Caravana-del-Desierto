#ifndef PARTIDAS_H_INCLUDED
#define PARTIDAS_H_INCLUDED

#include "main.h"
#include "Archivo.h"
#include "Lista.h"
#include "Menu.h"
#include "Jugadores.h"

#define NOM_ARCH_PARTIDAS "partidas.dat"

typedef struct
{
    int idPartida;
    int idJugador;
    unsigned puntos;
    unsigned movimientos;
    int gano;
} tRegistroPartida;

int altaPartida(const char* nomArch, const tRegistroPartida* reg);
int obtenerUltimoIdPartida(const char* nomArch);
void mostrarPartida(const void* reg);
void mostrarArchivoPartidas(const char* nomArch);

#endif // PARTIDAS_H_INCLUDED
