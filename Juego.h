#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include "main.h"
#include "Cola.h"
#include "Lista.h"
#include "Arbol.h"
#include "Jugadores.h"


/// Nombre del archivo de configuraciones
#define NOM_ARCH_CONFIG "config.txt"

/// Nombre del archivo donde se crea el tablero
#define NOM_ARCH_TABLERO "caravana.txt"

/// Nombre del archivo donde se guarda el historial
#define NOM_ARCH_MOVIMIENTOS "movimientos.txt"

///macros para el bucle del juego:
#define JUGADOR_GANO 1
#define JUGADOR_PERDIO -1
#define JUEGO_CONTINUA 0
//#define JUEGO_ERROR -2

#define OPCION_AVANZAR 1
#define OPCION_RETROCEDER 2

///macros para los códigos ASCII de los elementos del juego
#define ASCII_JUGADOR   'J'
#define ASCII_INICIO    'I'
#define ASCII_SALIDA    'S'

#define ASCII_BANDIDO   'B'
#define ASCII_PREMIO    'P'
#define ASCII_VIDA_EXTRA 'V'
#define ASCII_OASIS 'O'
#define ASCII_TORMENTA  'T'
#define ASCII_POS_VACIA '.'

///macros para el Tablero
#define BANDIDO 0
#define PREMIO 1
#define VIDA_EXTRA 2
#define OASIS 3
#define TORMENTA 4
#define POS_VACIA 5

#define CANT_COMPONENTES 6

///macros para cálculos de lógica y evitar uso de magic numbers
#define CANT_MIN_POSICIONES 3
#define RELACION_MIN_BANDIDOS 0.15
#define RELACION_MAX_BANDIDOS 0.80
#define RELACION_MIN_TORMENTAS 0.2
#define RELACION_MAX_TORMENTAS 0.6
#define TABLERO_JUGABLE 1
#define TABLERO_NO_JUGABLE 0


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
} tConfiguracion;


typedef struct
{
    char tipo;
    unsigned id;
} tElemento;

typedef struct
{
    int cantInicio;
    int cantJugador;
    int cantBandido;
    int cantPremio;
    int cantVida;
    int cantOasis;
    int cantTormenta;
    int cantSalida;
} tContadorElementos;

/// estructura para cada casilla del tablero
typedef struct
{
    tLista elementos;
    unsigned posicion;
} tCasilla;



/// estructura para la partida
typedef struct
{
    unsigned cantPuntos;
    int cantVidas;
    unsigned oasis;         ///modifique estos char por unsigned
    unsigned tormenta;
    unsigned posJugador;
    unsigned movimientos; //agregue en version mod funciones 1.1
} tPartida;

/// estructura para encolar los movimientos
typedef struct
{
    tElemento elem;
    unsigned posOrigen;
    unsigned posFinal;

    unsigned cantMovim;
    char direccion;
} tMovimiento;

/// estructura para los bandidos (guarda las posiciones)
typedef struct
{
    unsigned id;
    unsigned posBandido;
    unsigned movimientos;
} tBandido;


/// Funciones de juego
void iniciarPartida(tConfiguracion* config, tArbol* arbolJugadores);


///FUNCIONES DEL TABLERO
int crearTablero(const char* nomArch,tLista* tablero,tConfiguracion* config,tLista* bandidos);
int verificarTablero(tLista* tablero, const tConfiguracion* config);
int crearCasilla(tCasilla* casilla);
int guardarTableroArchivo(tLista* tablero, FILE* pf,tConfiguracion* config,int cantDigitos);


/// Funciones de registro de movimientos
int guardarMostrarHistorial(tCola* historial, const char* nomArch);


///FUNCIONES DE USO COMUN
int agregarElementoEnCasilla(tLista* tablero,tCasilla casillaPos,tElemento elem);
void dibujarTablero(tLista* tablero,int cantPosiciones);
void vaciarTablero(tLista* tablero);
int insertarElementoSeguro(tLista* tablero,tCasilla casilla,tElemento elem,FILE* pf);
int eliminarElementoEnCasilla(tLista* tablero,tCasilla casillaPos,tElemento elem);
int tirarDado();



///FUNCIONES AUXILIARES DE CMP Y ACCION
int compararPosicion(const void* a,const void* b);
int compararElementos(const void* a,const void* b);
int compararBandidos(const void* a,const void* b);
int compararPosBandidos(const void* a, const void* b);
void mostrarElementos(const void* elem, const void* extra);
void mostrarBandidos(const void* elem, const void* extra);

void accionEscribirArchivo(const void* elem, const void* pf);
void accionImprimirConsola(const void* elem, const void* extra);
void accionContarElementos(const void* elem, const void* extra);
void accionContarElementosDesdeTablero(const void* elem, const void* extra);
void accionActualizarBandido(const void* elem, const void* extra);


#endif // JUEGO_H_INCLUDED
