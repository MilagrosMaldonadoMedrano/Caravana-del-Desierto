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

/// Nombre del archivo donde se guarda el historial
#define NOM_ARCH_MOVIMIENTOS "movimientos.txt"

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


typedef struct
{
    char tipo;
} tElemento;

typedef struct
{
    tLista elementos;
    unsigned posicion;
}tCasilla;


/// estructura para cada casilla del tablero
/*typedef struct
{
    char caractElem;
}tCasilla;*/

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

/// estructura para los bandidos (guarda las posiciones)
typedef struct
{
    unsigned posBandido;
}tBandido;

int crearTablero(const char* nomArch,tLista* tablero,tConfiguracion* config);

/// Funciones de registro de movimientos
void registrarMovimiento(tCola* historial, char direccion, unsigned cantMovim);
void mostrarHistorial(tCola* historial);
int guardarMostrarHistorial(tCola* historial, const char* nomArch);

void dibujarTablero(tLista* tablero,int cantPosiciones,int columnas);


int crearCasilla(tCasilla* casilla);

int agregarElementoEnCasilla(tLista* tablero,tCasilla casillaPos,tElemento elem);

int compararPosicion(const void* a,const void* b);
int compararElem(const void* a, const void* b);
int compararPosYElem(const void* a, const void* b);

int guardarTableroArchivo(tLista* tablero, FILE* pf,tConfiguracion* config,int cantDigitos);


void accionEscribirArchivo(const void* elem, const void* pf);

void accionImprimirConsola(const void* elem, const void* extra);

void vaciarTablero(tLista* tablero);


/// Dado
int tirarDado(void);

#endif // JUEGO_H_INCLUDED
