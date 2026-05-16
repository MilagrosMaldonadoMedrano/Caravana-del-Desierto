#include "Menu.h"

void pedirNombre(char* nombre) {
    printf("Ingrese su nombre: ");
    fgets(nombre, MAX_NOMBRE, stdin);

    nombre[strcspn(nombre, "\n")] = '\0'; // saca el \n obtenido del fgets
}

// Muestra el menu y devuelve la opcion elegida
int mostrarMenu() {
    int opcion;

    printf("\n==============================\n");
    printf("     CARAVANA DEL DESIERTO    \n");
    printf("==============================\n");
    printf("  %d. Jugar nueva partida     \n", OPCION_JUGAR);
    printf("  %d. Ver ranking             \n", OPCION_RANKING);
    printf("  %d. Salir                   \n", OPCION_SALIR);
    printf("==============================\n");
    printf("Opcion: ");
    scanf(" %d", &opcion);

    return opcion;
}

// Muestra el ranking (para mas adelante)
void mostrarRanking() {
    printf("\n==============================\n");
    printf("           RANKING            \n");
    printf("==============================\n");
    printf("         <pendiente>          \n");
    printf("==============================\n");
}

// Inicializa la partida con los valores de config
void iniciarPartida(tConfiguracion* config) {
    tPartida partida;
    tCola tablero;
    tCola historial;
    char nombre[MAX_NOMBRE];

    pedirNombre(nombre);
    printf("\nBienvenido, %s!\n", nombre);

    crearCola(&tablero);
    crearCola(&historial);

    partida.cantPuntos = 0;
    partida.cantVidas = config->vidasInicio;
    partida.oasis = 0;
    partida.tormenta = 0;

    if (crearTablero(NOM_ARCH_TABLERO, &tablero, config) != TODO_OK) {
        printf("Error al crear el tablero. Volviendo al menu...\n");
        vaciarCola(&tablero);
        vaciarCola(&historial);
        return;
    }

    printf("\nVidas: %u | Puntos: %u\n", partida.cantVidas, partida.cantPuntos);


    /// vvv LOGICA DEL JUEGO vvv

    /// ^^^ LOGICA DEL JUEGO ^^^


    guardarMostrarHistorial(&historial, NOM_ARCH_MOVIMIENTOS);

    vaciarCola(&tablero);
    vaciarCola(&historial);
}
