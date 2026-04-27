#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include "main.h"
#include "Cola.h"

#define NOM_ARCH_CONFIG "config.txt"
#define NOM_ARCH_TABLERO "caravana.txt"

#define ERROR_SIN_MEM -3

#define AUMENTO_PROB_POS_VACIA 1.5


#define ASCII_JUGADOR   'J'
#define ASCII_INICIO    'I'
#define ASCII_SALIDA    'S'

#define ASCII_BANDIDO   'B'
#define ASCII_PREMIO    'P'
#define ASCII_VIDA_EXTRA 'V'
#define ASCII_OASIS 'O'
#define ASCII_TORMENTA  'T'
#define ASCII_POS_VACIA '.'

///macros para el switch de crearTablero
#define BANDIDO 0
#define PREMIO 1
#define VIDA_EXTRA 2
#define OASIS 3
#define TORMENTA 4
#define POS_VACIA 5

#define CANT_COMPONENTES 6

///macros de dirección para la estructura tMovimientos
#define FORWARD 'F'
#define BACKWARD 'B'

/// estructura para el archivo de configuraciones
typedef struct
{
    unsigned cantPosiciones;
    unsigned vidasInicio;
    unsigned maxBandidos;
    unsigned maxPremios;
    unsigned maxVidasExtra;
    unsigned maxOasis;
    unsigned maxTormentas;
}tConfiguracion;

/// estructura para cada casilla del tablero
typedef struct
{
    char caractElem;
}tCasilla;

/// estructura para la partida
typedef struct
{
    unsigned cantPuntos;
    unsigned cantVidas;
    char oasis;
    char tormenta;
}tPartida;

/// estructura para encolar los movimientos
typedef struct
{
    unsigned cantMovim;
    char direccion;
}tMovimiento;

int crearTablero(const char* nomArch,tCola* tablero,tConfiguracion* config);

#endif // JUEGO_H_INCLUDED
