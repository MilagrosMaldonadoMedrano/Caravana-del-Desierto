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
int configuracionPorDefecto(FILE** arch,const char* nomArch)
{

    if (abrirArchivo(arch,nomArch,"wt") != TODO_OK)
        return ERROR_ARCH;

    fprintf(*arch,"cantidad_posiciones: 25\n");
    fprintf(*arch,"vidas_inicio: 3\n");
    fprintf(*arch,"maximo_bandidos: 2\n");
    fprintf(*arch,"maximo_premios: 3\n");
    fprintf(*arch,"maximo_vidas_extra: 1\n");
    fprintf(*arch,"maximo_oasis: 2\n");
    fprintf(*arch,"maximo_tormentas: 3\n");

    fclose(*arch);

    return TODO_OK;
}



//consultarle al profe si e sposible que el txt tenga las lineas modificadas
//elemplo: que la primer linea sea "vidas_inicio: 3" y no "cantidad_posiciones: 25"

int cargarConfiguracion(const char* nomArch,tConfiguracion* config)
{
    FILE* arch;
    if(ERROR_ARCH==abrirArchivo(&arch,nomArch,"rt"))
    {
//        perror("Error:");
        printf("\nSe cargaran configuraciones por defecto\n");
        if (configuracionPorDefecto(&arch,nomArch) != TODO_OK)
        {
            fprintf(stderr, "\nHubo un error al cargar las configuraciones por defecto...\n");
            return ERROR_ARCH_DEF;
        }
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

    return validarConfiguracion(config);
}

//función que verifica que la configuración sea válida
int validarConfiguracion(const tConfiguracion* config)
{
    unsigned cantMinBandidos = (unsigned)config->cantPosiciones * RELACION_MIN_BANDIDOS;

    ///que no haya mas cantidad de posiciones que elementos (REVISAR)
    if (config->cantPosiciones < (config->maxBandidos + config->maxOasis + config->maxPremios+\
                                  config->maxTormentas + config->maxVidasExtra))
        return ERROR_CONFIG_CANT_POS;

    if (config->cantPosiciones < CANT_MIN_POSICIONES)
        return ERROR_CONFIG_CANT_POS;

    ///que el jugador tenga minimo 1 vida
    if (config->vidasInicio < 0)
        return ERROR_CONFIG_VIDA;

    ///que no sean todo maxBandidos menos 1 o 2 casillas
    if (config->maxBandidos > config->cantPosiciones * RELACION_MAX_BANDIDOS)
        //if (config->vidasInicio < config->maxBandidos)
        return ERROR_CONFIG_BANDIDO1;

    /// si la cantidad de bandidos es mayor a la ideal (el 15% del tablero):
    /// el jugador debe tener como mínimo de vidas: la cantidad de bandidos - la cantidad IDEAL de bandidos
    /// dependiendo la cantidad de casillas.
    /// esto no verifica la cantidad de vidas extra en el tablero, el problema es que capaz no cumple esto,
    /// pero el tablero esta todo repleto de vidas extra. Es demasiado raro, confiamos que el usuario no
    /// va a hacer esas cosas
    if (config->maxBandidos > cantMinBandidos)
        if (config->vidasInicio + config->maxVidasExtra + config->maxOasis / 2 < config->maxBandidos - cantMinBandidos)
            return ERROR_CONFIG_BANDIDO2;

    if (config->vidasInicio + config->maxVidasExtra < config->maxBandidos)
    {
        /// si hay muchas tormentas, y pocas o ninguna vida extra
        if (config->maxTormentas >= config->maxOasis + ((unsigned)config->cantPosiciones * RELACION_MAX_TORMENTAS))
            return ERROR_CONFIG_TORMENTA;
    }

    return TODO_OK;
}

void informarErrorConfiguracion(int codError)
{
    switch (codError)
    {
    case ERROR_ARCH:
        fprintf(stderr, "\nNo se pudo abrir el archivo de configuraciones");
        break;

    case ERROR_ARCH_DEF:
        fprintf(stderr, "\nNo se pudo crear el archivo de configuraciones");
        break;

        printf("\nConfiguraciones no jugables:");
    case ERROR_CONFIG_CANT_POS:
        fprintf(stderr, "\nLa cantidad de posiciones es insuficiente");
        break;

    case ERROR_CONFIG_VIDA:
        fprintf(stderr, "\nEl jugador debe tener minimo 1 vida al inicio");
        break;

    case ERROR_CONFIG_BANDIDO1:
        fprintf(stderr, "\nNo puede haber tantos bandidos");
        break;

    case ERROR_CONFIG_BANDIDO2:
        fprintf(stderr, "\nEl jugador necesita mas vidas para esa cantidad de bandidos");
        break;

    case ERROR_CONFIG_TORMENTA:
        fprintf(stderr, "\nHay muchas tormentas, y pocas vidas y/u oasis");
        break;
    }
}
