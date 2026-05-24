#include "main.h"
#include "Cola.h"
#include "Archivo.h"
#include "Juego.h"
#include "Menu.h"
#include "Turno.h"
#include "Jugadores.h"

int main()
{
    tConfiguracion config;
    int opcion;

    srand(time(NULL));

    ///para uso  e interpretacion de caracteres especiales
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);


    if (cargarConfiguracion(NOM_ARCH_CONFIG, &config) != TODO_OK)
    {
        perror("\nError al cargar las configuraciones:");
        printf("\nFinalizando programa...\n");
        return ERROR_ARCH;
    }

    mostrarArchivoJugadores(NOM_ARCH_JUGADORES);

    do {
        opcion = mostrarMenu();

        switch (opcion)
        {
            case OPCION_JUGAR:
                iniciarPartida(&config);
                break;
            case OPCION_RANKING:
                mostrarRanking();
                break;
            case OPCION_SALIR:
                printf("\nSaliendo...\n");
                break;
            default:
                printf("\nOpcion invalida. Intente nuevamente.\n");
        }

    } while (opcion != OPCION_SALIR);

    return 0;
}
