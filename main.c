#include "main.h"
#include "Cola.h"
#include "Archivo.h"
#include "Juego.h"
#include "Menu.h"
#include "Turno.h"

int main()
{
    tConfiguracion config;
    int opcion;

    srand(time(NULL));

    tLista tablero;

    crearLista(&tablero);




    if (cargarConfiguracion(NOM_ARCH_CONFIG, &config) != TODO_OK)
    {
        perror("\nError al cargar las configuraciones:");
        printf("\nFinalizando programa...\n");
        return ERROR_ARCH;
    }


    crearTablero(NOM_ARCH_CONFIG,&tablero,&config);
    dibujarTablero(&tablero,config.cantPosiciones,5);

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

    vaciarTablero(&tablero);
    return 0;
}

/*int main()
{
    tConfiguracion config;
    tCola tablero;
    tCola historial; // registro de movimientos

    srand(time(NULL));

    crearCola(&tablero);
    crearCola(&historial);

    if (cargarConfiguracion(NOM_ARCH_CONFIG,&config)!=TODO_OK)
    {
        perror("\nError al cargar las configuraciones:");
        printf("\nFinalizando programa...\n");
        return ERROR_ARCH;
    }

    if (crearTablero(NOM_ARCH_TABLERO,&tablero,&config)!=TODO_OK)
    {
        perror("\nError al crear el tablero:");
        printf("\nFinalizando programa...\n");
        return ERROR_CONFIG;
    }


    ///para ver si funciona
    printf("Cantidad de pos: %d\n",config.cantPosiciones);
    printf("Maximo de tormentas: %d\n",config.maxTormentas);

    /// Test del registro de movimientos
    registrarMovimiento(&historial, FORWARD, tirarDado());
    registrarMovimiento(&historial, FORWARD,tirarDado());
    registrarMovimiento(&historial, BACKWARD, tirarDado());
    registrarMovimiento(&historial, FORWARD, tirarDado());
    guardarMostrarHistorial(&historial, NOM_ARCH_MOVIMIENTOS);

    /// Test del dado
    for (int i = 0; i < 5; i++)
        printf("\nTirada de dado: %u\n", tirarDado());

    dibujarTablero(&tablero,config.cantPosiciones,5);//debemos definir un criterio para las columnas
    vaciarCola(&tablero);
    vaciarCola(&historial);

    return 0;
}*/
