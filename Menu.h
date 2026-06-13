#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "main.h"
#include "Archivo.h"
#include "Juego.h"
#include "Jugadores.h"
#include "Partidas.h"
#include "Arbol.h"

#define NICKNAME 1
#define NOMBRE 2

#define OPCION_JUGAR 1
#define OPCION_RANKING 2
#define OPCION_SALIR 3

int identificarJugador(char* nombre, char* nickname);
int mostrarMenu();
void mostrarRanking(const char* nomArch);
void iniciarPartida(tConfiguracion* config, tArbol* arbolJugadores);
char pedirDireccion();
void ingresarNombreVal(const char* mensaje, char* nombre);

int cmpPuntaje(const void* elem,const void* elem2);
void mostrarJugadorDesdeRanking(const void* info,void* params);

#endif // MENU_H_INCLUDED
