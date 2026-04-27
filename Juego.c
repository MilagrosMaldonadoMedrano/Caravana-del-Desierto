#include "Juego.h"
#include "Archivo.h"

//esta función crea el archivo del tablero en base a la configuración, un tablero jugable
//llama a otra función que valida si la configuración, para poder crear el tablero
int crearTablero(const char* nomArch,tCola* tablero,tConfiguracion* config)
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
}
