#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "main.h"
#include "Juego.h"
#include "Archivo.h"

#define MAX_NOMBRE 3

#define OPCION_JUGAR 1
#define OPCION_RANKING 2
#define OPCION_SALIR 3

void pedirNombre(char* nombre);
int mostrarMenu();
void mostrarRanking();
void iniciarPartida(tConfiguracion* config);

#endif // MENU_H_INCLUDED
