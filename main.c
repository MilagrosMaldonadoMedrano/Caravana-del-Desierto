#include "main.h"
#include "Cola.h"
#include "Archivo.h"
#include "Juego.h"

int main()
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
    registrarMovimiento(&historial, FORWARD, 5);
    registrarMovimiento(&historial, FORWARD, 2);
    registrarMovimiento(&historial, BACKWARD, 3);
    registrarMovimiento(&historial, FORWARD, 1);
    guardarMostrarHistorial(&historial, NOM_ARCH_MOVIMIENTOS);

    /// Test del dado
    for (int i = 0; i < 5; i++)
        printf("\nTirada de dado: %u", tirarDado());

    vaciarCola(&tablero);

    return 0;
}
