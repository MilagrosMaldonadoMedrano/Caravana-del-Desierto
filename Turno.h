#ifndef TURNO_H_INCLUDED
#define TURNO_H_INCLUDED

#include "main.h"
#include "Juego.h"

int planificarMovimientoJugador(tConfiguracion* config,tCola* bufferMovs,tCola* historial,tPartida* partida);
void planificarMovimientosBandidos(tPartida* partida, tCola* bufferMovs, tLista* tablero, tLista* bandidos);
void procesarTurno(tLista* tablero, tCola* bufferMovs, tPartida* partida);
int manejarSituacionCasilla(tPartida* partida,tLista* tablero,tLista* bandidos,tCasilla casillaPosicion,tConfiguracion* config);
void reiniciarContador(tContadorElementos* cont);

#endif // TURNO_H_INCLUDED
