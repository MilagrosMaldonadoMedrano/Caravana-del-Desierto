#include <stdio.h>
#include <stdlib.h>
#include "Cola.h"
#include "Archivo.h"
#include "Juego.h"
int main()
{
    tConfiguracion config;

    cargarConfiguracion("config.txt",&config);

    ///para ver si funciona
    printf("Cantidad de pos: %d\n",config.cantPosiciones);
    printf("Maximo de tormentas: %d\n",config.maxTormentas);


    return 0;
}
