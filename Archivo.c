#include "Archivo.h"

int abrirArchivo(FILE** arch,const char* nomArch,const char* modoApertura)
{
    *arch=fopen(nomArch,modoApertura);

    if(!*arch)
        return ERROR_ARCH;
    return TODO_OK;

}



//en esta funcion directamente se podrían setear los valores de  tConfiguracion* config, no lo hice p´orque la consigna pide
//que se cargue desde un archivo pero se podria consultar
void configuracionPorDefecto(FILE** arch,const char* nomArch)
{

    abrirArchivo(arch,nomArch,"wt");

    fprintf(*arch,"cantidad_posiciones: 25\n");
    fprintf(*arch,"vidas_inicio: 3\n");
    fprintf(*arch,"maximo_bandidos: 2\n");
    fprintf(*arch,"maximo_premios: 3\n");
    fprintf(*arch,"maximo_vidas_extra: 1\n");
    fprintf(*arch,"maximo_oasis: 2\n");
    fprintf(*arch,"maximo_tormentas: 3\n");

    fclose(*arch);
}



//consultarle al profe si e sposible que el txt tenga las lineas modificadas
//elemplo: que la primer linea sea "vidas_inicio: 3" y no "cantidad_posiciones: 25"

int cargarConfiguracion(const char* nomArch,tConfiguracion* config)
{
    FILE* arch;
    if(ERROR_ARCH==abrirArchivo(&arch,nomArch,"rt"))
    {
        perror("Error:");
        printf("\nSe cargaran configuraciones por defecto\n");
        configuracionPorDefecto(&arch,nomArch);
    }

    if (ERROR_ARCH==abrirArchivo(&arch,nomArch,"rt"))
    {
        perror("Error:");
        return ERROR_ARCH;
    }

    char linea[255];

    fgets(linea,sizeof(linea),arch);
    sscanf(linea,"%*[^:]: %d",&config->cantPosiciones); //ignora toda la linea (%*[^:]: ) y escribe el nro final

    fgets(linea,sizeof(linea),arch);
    sscanf(linea,"%*[^:]: %d",&config->vidasInicio);

    fgets(linea,sizeof(linea),arch);
    sscanf(linea,"%*[^:]: %d",&config->maxBandidos);

    fgets(linea,sizeof(linea),arch);
    sscanf(linea,"%*[^:]: %d",&config->maxPremios);

    fgets(linea,sizeof(linea),arch);
    sscanf(linea,"%*[^:]: %d",&config->maxVidasExtra);

    fgets(linea,sizeof(linea),arch);
    sscanf(linea,"%*[^:]: %d",&config->maxOasis);

    fgets(linea,sizeof(linea),arch);
    sscanf(linea,"%*[^:]: %d",&config->maxTormentas);

    fclose(arch);

    if (validarConfiguracion(config)!=TODO_OK)
        return ERROR_CONFIG;

    return TODO_OK;
}

//función que verifica que la configuración sea válida
int validarConfiguracion(const tConfiguracion* config)
{
    ///validaciones del archivo de configuraciones
    if (config->cantPosiciones < (config->maxBandidos + config->maxOasis + config->maxPremios+\
                                  config->maxTormentas + config->maxVidasExtra))
        return ERROR_CONFIG;

    ///otras validaciones podrian ser como: que no sean todo maxBandidos menos 1 o 2 casillas

    return TODO_OK;
}
