#include "Juego.h"
#include "Archivo.h"

//esta función crea el archivo del tablero en base a la configuración, un tablero jugable
//llama a otra función que valida si la configuración, para poder crear el tablero
/*int crearTablero(const char* nomArch,tCola* tablero,tConfiguracion* config)
{
    FILE* pf;
    unsigned tableroValido = 0, elemValido, aleatorio, cantPosiVacias;
    int cantDigitosCasillas = cantDigitosUnsigned(config->cantPosiciones);
    //char codAsciiComponentes[] = {ASCII_BANDIDO,ASCII_PREMIO,ASCII_VIDA_EXTRA,
    //                                ASCII_OASIS,ASCII_TORMENTA,ASCII_POS_VACIA};
    //esto es para reemplazar el switch que hace lo mismo para cada componentes. A MEJORAR LUEGO
    tConfiguracion configTemp;
    tCasilla elem;

    do
    {
        configTemp = *config;
        elem.caractElem = ASCII_INICIO;
        if (ponerEnCola(tablero,&elem,sizeof(elem))!=TODO_OK)
            return ERROR_SIN_MEM;

        configTemp.cantPosiciones--; //ignora la casilla del inicio
        configTemp.cantPosiciones--; //ignora la casiila del final

        cantPosiVacias = configTemp.cantPosiciones - (configTemp.maxBandidos + configTemp.maxOasis+\
                                                    configTemp.maxPremios + configTemp.maxTormentas+\
                                                    configTemp.maxVidasExtra);

        while (configTemp.cantPosiciones)
        {
            elemValido = 0;
            do
            {
                aleatorio = rand()%(int)(CANT_COMPONENTES*AUMENTO_PROB_POS_VACIA);
                //printf("Nro Aleatorio: %d\n", aleatorio);
                switch(aleatorio)
                {
                    case BANDIDO: if (configTemp.maxBandidos)
                            {
                                configTemp.maxBandidos--;
                                elem.caractElem = ASCII_BANDIDO;
                                elemValido = 1;
                            }
                    break;
                    case PREMIO: if (configTemp.maxPremios)
                            {
                                configTemp.maxPremios--;
                                elem.caractElem = ASCII_PREMIO;
                                elemValido = 1;
                            }
                    break;
                    case VIDA_EXTRA: if (configTemp.maxVidasExtra)
                            {
                                configTemp.maxVidasExtra--;
                                elem.caractElem = ASCII_VIDA_EXTRA;
                                elemValido = 1;
                            }
                    break;
                    case OASIS: if (configTemp.maxOasis)
                            {
                                configTemp.maxOasis--;
                                elem.caractElem = ASCII_OASIS;
                                elemValido = 1;
                            }
                    break;
                    case TORMENTA: if (configTemp.maxTormentas)
                            {
                                configTemp.maxTormentas--;
                                elem.caractElem = ASCII_TORMENTA;
                                elemValido = 1;
                            }
                    break;
                    default: if (cantPosiVacias)
                            {
                                cantPosiVacias--;
                                elem.caractElem = ASCII_POS_VACIA;
                                elemValido = 1;
                            }

                }
            } while (!elemValido);

            if (ponerEnCola(tablero, &elem, sizeof(elem))!=TODO_OK)
            {
                vaciarCola(tablero);
                return ERROR_SIN_MEM;
            }
            configTemp.cantPosiciones--;
        }

        elem.caractElem = ASCII_SALIDA;
        if (ponerEnCola(tablero, &elem, sizeof(elem))!=TODO_OK)
        {
            vaciarCola(tablero);
            return ERROR_SIN_MEM;
        }

    /// validaciones del tablero



    if (tableroValido) /// poner !
        vaciarCola(tablero);

    } while (tableroValido); /// poner !

    /// cargar la cola en el archivo
    if (abrirArchivo(&pf,nomArch,"wt")!=TODO_OK)
    {
        vaciarCola(tablero);
        return ERROR_ARCH;
    }

    sacarDeCola(tablero,&elem,sizeof(elem));
    ponerEnCola(tablero,&elem,sizeof(elem));
    sacarDeCola(tablero,&elem,sizeof(elem));
    fprintf(pf,"%0*d: [I J]\n",cantDigitosCasillas,configTemp.cantPosiciones+1);
    configTemp.cantPosiciones++;
    while(elem.caractElem != ASCII_SALIDA)
    {
        fprintf(pf,"%0*d: %c\n",cantDigitosCasillas,configTemp.cantPosiciones+1,elem.caractElem);
        configTemp.cantPosiciones++;
        ponerEnCola(tablero,&elem,sizeof(elem));
        sacarDeCola(tablero,&elem,sizeof(elem));
    }
    ponerEnCola(tablero,&elem,sizeof(elem));
    fprintf(pf,"%0*d: %c\n",cantDigitosCasillas,config->cantPosiciones,elem.caractElem);

    fclose(pf);

    return TODO_OK;
}*/
int crearTablero(const char* nomArch,tCola* tablero,tConfiguracion* config)
{
    FILE* pf;
    unsigned tableroValido = 0;
    unsigned elemValido; //controla que casilla sea valida
    unsigned aleatorio;  //nro aleatorio que decide que poner en la casilla
    unsigned cantPosiVacias; //cantidad de casillas vacias
    int cantDigitosCasillas = cantDigitosUnsigned(config->cantPosiciones);
    //char codAsciiComponentes[] = {ASCII_BANDIDO,ASCII_PREMIO,ASCII_VIDA_EXTRA,
    //                                ASCII_OASIS,ASCII_TORMENTA,ASCII_POS_VACIA};
    //esto es para reemplazar el switch que hace lo mismo para cada componentes. A MEJORAR LUEGO
    tConfiguracion configTemp;
    tCasilla casilla;

    do
    {
        configTemp = *config;
        casilla.caractElem = ASCII_INICIO;
        if (ponerEnCola(tablero,&casilla,sizeof(tCasilla))!=TODO_OK)
            return ERROR_SIN_MEM;

        configTemp.cantPosiciones--; //ignora la casilla del inicio
        configTemp.cantPosiciones--; //ignora la casiila del final

        cantPosiVacias = configTemp.cantPosiciones - (configTemp.maxBandidos + configTemp.maxOasis+\
                                                     configTemp.maxPremios + configTemp.maxTormentas+\
                                                     configTemp.maxVidasExtra);

        while (configTemp.cantPosiciones) //mientras que hayan casillas vacias
        {
            elemValido = 0;
            do
            {
                aleatorio = rand()%(int)(CANT_COMPONENTES*AUMENTO_PROB_POS_VACIA);
                //printf("Nro Aleatorio: %d\n", aleatorio);
                switch(aleatorio)
                {
                case BANDIDO:
                    if (configTemp.maxBandidos)
                    {
                        configTemp.maxBandidos--;
                        casilla.caractElem = ASCII_BANDIDO;
                        elemValido = 1;
                    }
                    break;
                case PREMIO:
                    if (configTemp.maxPremios)
                    {
                        configTemp.maxPremios--;
                        casilla.caractElem = ASCII_PREMIO;
                        elemValido = 1;
                    }
                    break;
                case VIDA_EXTRA:
                    if (configTemp.maxVidasExtra)
                    {
                        configTemp.maxVidasExtra--;
                        casilla.caractElem = ASCII_VIDA_EXTRA;
                        elemValido = 1;
                    }
                    break;
                case OASIS:
                    if (configTemp.maxOasis)
                    {
                        configTemp.maxOasis--;
                        casilla.caractElem = ASCII_OASIS;
                        elemValido = 1;
                    }
                    break;
                case TORMENTA:
                    if (configTemp.maxTormentas)
                    {
                        configTemp.maxTormentas--;
                        casilla.caractElem = ASCII_TORMENTA;
                        elemValido = 1;
                    }
                    break;
                default:
                    if (cantPosiVacias)
                    {
                        cantPosiVacias--;
                        casilla.caractElem = ASCII_POS_VACIA;
                        elemValido = 1;
                    }

                }
            }
            while (!elemValido); //no se si es lo mas eficiente pero es un buen comienzo

            if (ponerEnCola(tablero, &casilla, sizeof(tCasilla))!=TODO_OK)
            {
                vaciarCola(tablero);
                return ERROR_SIN_MEM;
            }
            configTemp.cantPosiciones--;
        }

        casilla.caractElem = ASCII_SALIDA;
        if (ponerEnCola(tablero, &casilla, sizeof(tCasilla))!=TODO_OK)
        {
            vaciarCola(tablero);
            return ERROR_SIN_MEM;
        }

        /// validaciones del tablero



        if (tableroValido) /// poner !
            vaciarCola(tablero);

    }
    while (tableroValido);   /// poner !

    /// cargar la cola en el archivo
    if (abrirArchivo(&pf,nomArch,"wt")!=TODO_OK)
    {
        vaciarCola(tablero);
        return ERROR_ARCH;
    }

    //saca y vuelve a poner el primer elemento
    sacarDeCola(tablero,&casilla,sizeof(tCasilla));
    ponerEnCola(tablero,&casilla,sizeof(tCasilla));
    sacarDeCola(tablero,&casilla,sizeof(tCasilla));
    fprintf(pf,"%0*d: [I J]\n",cantDigitosCasillas,configTemp.cantPosiciones+1);
    configTemp.cantPosiciones++;
    while(casilla.caractElem != ASCII_SALIDA)
    {
        fprintf(pf,"%0*d: %c\n",cantDigitosCasillas,configTemp.cantPosiciones+1,casilla.caractElem);
        configTemp.cantPosiciones++;
        ponerEnCola(tablero,&casilla,sizeof(tCasilla));
        sacarDeCola(tablero,&casilla,sizeof(tCasilla));
    }
    ponerEnCola(tablero,&casilla,sizeof(tCasilla));
    fprintf(pf,"%0*d: %c\n",cantDigitosCasillas,config->cantPosiciones,casilla.caractElem);

    fclose(pf);

    return TODO_OK;
}



void dibujarTablero(tCola* tablero, int cantPosiciones, int columnas)
{
    tCasilla casilla;
    int filas = (cantPosiciones + columnas - 1) / columnas;

    for(int f = 0; f < filas; f++)
    {
        // línea superior
        for(int c = 0; c < columnas; c++)
            printf("+----");
        printf("+\n");

        //if(f%2==0)
            for(int c = 0; c < columnas; c++)
            {
                printf("| ");

                if(TODO_OK==sacarDeCola(tablero,&casilla,sizeof(tCasilla)))
                {
                    printf("%c  ", casilla.caractElem);
                    ponerEnCola(tablero,&casilla,sizeof(tCasilla));
                }

                else
                    printf("   ");

            }
        /*else
            for(int c = columnas; 0 < c; c--)
            {
                printf("| ");

                if(TODO_OK==sacarDeCola(tablero,&casilla,sizeof(tCasilla)))
                {
                    printf("%c  ", casilla.caractElem);
                    ponerEnCola(tablero,&casilla,sizeof(tCasilla));
                }

                else
                    printf("   ");
            }*/

        printf("|\n");
    }

    for(int c = 0; c < columnas; c++)
        printf("+----");
    printf("+\n");
}

/// ---===================================---
/// |  FUNCIONES DE REGISTRO DE MOVIMIENTO  |
/// ---===================================---

// Encola un movimiento en el historial
void registrarMovimiento(tCola* historial, char direccion, unsigned cantMovim)
{
    tMovimiento mov;
    mov.direccion = direccion;
    mov.cantMovim = cantMovim;
    ponerEnCola(historial, &mov, sizeof(mov));
}

// Muestra todos los movimientos por pantalla y vacía la cola
void mostrarHistorial(tCola* historial)
{
    tMovimiento mov;
    int nroTurno = 1;

    printf("\n===== REGISTRO DE MOVIMIENTOS =====\n");

    if (colaVacia(historial))
    {
        printf("No se realizaron movimientos.\n");
        printf("===================================\n");
        return;
    }

    do {
        sacarDeCola(historial, &mov, sizeof(mov));
        printf("Turno %2d: %c%u\n", nroTurno, mov.direccion, mov.cantMovim);
        nroTurno++;
    } while (!colaVacia(historial));

    printf("===================================\n");
}

// Guarda el historial en archivo y lo muestra por pantalla
int guardarMostrarHistorial(tCola* historial, const char* nomArch)
{
    FILE* arch;
    tMovimiento mov;
    tCola copiaTemporal;
    int nroTurno = 1;

    crearCola(&copiaTemporal);

    while (!colaVacia(historial))
    {
        sacarDeCola(historial, &mov, sizeof(mov));
        ponerEnCola(&copiaTemporal, &mov, sizeof(mov));
    }

    if (abrirArchivo(&arch, nomArch, "wt") != TODO_OK)
    {
        vaciarCola(&copiaTemporal);
        perror("Error al guardar historial");
        return ERROR_ARCH;
    }

    printf("\n===== REGISTRO DE MOVIMIENTOS =====\n");
    fprintf(arch, "===== REGISTRO DE MOVIMIENTOS =====\n");

    if (colaVacia(&copiaTemporal))
    {
        printf("No se realizaron movimientos.\n");
        fprintf(arch, "No se realizaron movimientos.\n");
    }
    else
    {
        do
        {
            sacarDeCola(&copiaTemporal, &mov, sizeof(mov));
            printf("Turno %2d: %c%u\n", nroTurno, mov.direccion, mov.cantMovim);
            fprintf(arch, "Turno %2d: %c%u\n", nroTurno, mov.direccion, mov.cantMovim);
            nroTurno++;
        }
        while (!colaVacia(&copiaTemporal));
    }

    printf("===================================\n");
    fprintf(arch, "===================================\n");

    fclose(arch);
    return TODO_OK;
}


/// ---===================================---
/// |                 Dado                  |
/// ---===================================---
int tirarDado(void)
{
    return rand() % 6 + 1;
}
