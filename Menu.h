#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "main.h"
#include "Archivo.h"
#include "Juego.h"
#include "Jugadores.h"
#include "Partidas.h"
#include "Arbol.h"



#define OPCION_JUGAR 1
#define OPCION_RANKING 2
#define OPCION_SALIR 3

void pedirNombre(char* nombre, char* nickname);
int mostrarMenu();
void mostrarRanking(const char* nomArch);
void iniciarPartida(tConfiguracion* config, tArbol* arbolJugadores);


void planificarMovimientosBandidos(tPartida* partida, tCola* bufferMovs, tLista* tablero, tLista* bandidos);

int planificarMovimientoJugador(tConfiguracion* config,tCola* bufferMovs,tCola* historial,tPartida* partida);
void procesarTurno(tLista* tablero, tCola* bufferMovs, tPartida* partida);
int manejarSituacionCasilla(tPartida* partida,tLista* tablero,tLista* bandidos,tCasilla casillaPosicion,tConfiguracion* config);
void limpiarBuffer();
int ejecutarTurnoJugador(tLista* tablero,tPartida* partida, tCola* historial,tConfiguracion* config);
char pedirDireccion() ;
void reiniciarContador(tContadorElementos* cont);

#endif // MENU_H_INCLUDED
