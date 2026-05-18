#ifndef TURNO_H_INCLUDED
#define TURNO_H_INCLUDED

#include "main.h"
#include "Juego.h"
#include "Cola.h"

#define JUGADOR_GANO 1
#define JUGADOR_PERDIO -1
#define JUEGO_CONTINUA 0

#define OPCION_AVANZAR 1
#define OPCION_RETROCEDER 2

char pedirDireccion();
void aplicarEfectoCasilla(char casilla, tPartida* partida);
int verificarFinPartida(char casilla, const tPartida* partida);
void ejecutarTurno(tLista* tablero, tPartida* partida, tCola* historial);
void ejecutarMovimientoBandido(tLista* tablero, tLista* bandidos, unsigned cantPosiciones);

#endif // TURNO_H_INCLUDED
