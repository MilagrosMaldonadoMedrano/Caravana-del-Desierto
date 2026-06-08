#include "main.h"
#include "Cola.h"
#include "Archivo.h"
#include "Juego.h"
#include "Menu.h"
#include "Turno.h"
#include "Jugadores.h"
#include "Arbol.h"
#include "Indice.h"

int main()
{
    tConfiguracion config;
    tArbol arbolJugadores;
    int opcion, resultado;

    srand(time(NULL));

    ///para uso  e interpretacion de caracteres especiales
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    resultado = cargarConfiguracion(NOM_ARCH_CONFIG, &config);
    if (resultado != TODO_OK)
    {
        informarErrorConfiguracion(resultado);
        perror("\nError al cargar las configuraciones:");
        printf("\nFinalizando programa...\n");
        return ERROR_ARCH;
    }

    crearArbol(&arbolJugadores);

    if (cargarIndiceJugadores(NOM_ARCH_INDICE_JUGADORES, &arbolJugadores) != TODO_OK)
        indexarArchivoJugadores(NOM_ARCH_JUGADORES, &arbolJugadores);

    mostrarIndiceJugadores(&arbolJugadores);

    /*//fc para verificar los archivos:
    mostrarArchivoJugadores(NOM_ARCH_JUGADORES);
    mostrarArchivoPartidas(NOM_ARCH_PARTIDAS);*/

    do {
        opcion = mostrarMenu();

        switch (opcion)
        {
            case OPCION_JUGAR:
                iniciarPartida(&config, &arbolJugadores);
                break;
            case OPCION_RANKING:
                mostrarRanking(NOM_ARCH_JUGADORES);
                break;
            case OPCION_SALIR:
                printf("\nSaliendo...\n");
                break;
            default:
                printf("\nOpcion invalida. Intente nuevamente.\n");
        }

    } while (opcion != OPCION_SALIR);

    guardarIndiceJugadores(NOM_ARCH_INDICE_JUGADORES, &arbolJugadores);
    vaciarArbol(&arbolJugadores);

    return 0;
}
