#include "main.h"
#include "Cola.h"
#include "Archivo.h"
#include "Juego.h"

int main()
{
    tConfiguracion config;
    tCola tablero;

    srand(time(NULL));

    crearCola(&tablero);

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

    vaciarCola(&tablero);

    return 0;
}
