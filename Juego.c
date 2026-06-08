#include "Archivo.h"
#include "Lista.h"

//esta función crea el archivo del tablero en base a la configuración, un tablero jugable
//llama a otra función que valida si la configuración, para poder crear el tablero

int crearTablero(const char* nomArch,tLista* tablero,tConfiguracion* config,tLista* bandidos)
{
    FILE* pf;
    tBandido bandido;
    int auxTormenta;
    int tableroJugable;
    ///creo todas las listas de las casillas

    tCasilla casilla;
    tElemento elem;

    if(TODO_OK!=abrirArchivo(&pf,NOM_ARCH_TABLERO,"wt"))
        return ERROR_ARCH;


    int cantDigitosCasillas = cantDigitosUnsigned(config->cantPosiciones);

    do
    {
        vaciarTablero(tablero);
        vaciarLista(bandidos);
        printf("Intento de creacion del tablero...\n");

        for(int i=0;i<config->cantPosiciones;i++)
        {
            crearCasilla(&casilla);
            casilla.posicion=i+1;


            if(listaInsertarAlFinal(tablero,&casilla,sizeof(tCasilla))!=TODO_OK)
            {
                vaciarLista(tablero);
                fclose(pf);
                return  ERROR_MEM;
            }
        }

        ///agrego el primer elemento

        casilla.posicion=1;
        elem.tipo=ASCII_INICIO;
        if(insertarElementoSeguro(tablero, casilla, elem, pf) != TODO_OK)
            return ERROR_MEM;


        elem.tipo=ASCII_JUGADOR;
        if(insertarElementoSeguro(tablero, casilla, elem, pf) != TODO_OK)
            return ERROR_MEM;


        unsigned aleatorio;
        ///bandidos
        elem.tipo=ASCII_BANDIDO;

        for(int i=0;i<config->maxBandidos;i++)
        {
            aleatorio = rand()%(config->cantPosiciones-2)+2;
            casilla.posicion=aleatorio;
            elem.id=i+1;
            if(insertarElementoSeguro(tablero, casilla, elem, pf) != TODO_OK)
                return ERROR_MEM;
            bandido.id = elem.id;
            bandido.posBandido = casilla.posicion;
            bandido.movimientos = 0;
            insertarAlFinal(bandidos, &bandido, sizeof(bandido));
        }

        ///premios
        elem.tipo=ASCII_PREMIO;
        for(int i=0;i<config->maxPremios;i++)
        {
            aleatorio = rand()%(config->cantPosiciones-2)+2;
            casilla.posicion=aleatorio;
            if(insertarElementoSeguro(tablero, casilla, elem, pf) != TODO_OK)
                return ERROR_MEM;
        }

        ///vidas
        elem.tipo=ASCII_VIDA_EXTRA;
        for(int i=0;i<config->maxVidasExtra;i++)
        {
            aleatorio = rand()%(config->cantPosiciones-2)+2;
            casilla.posicion=aleatorio;
            if(insertarElementoSeguro(tablero, casilla, elem, pf) != TODO_OK)
                return ERROR_MEM;
        }


        ///oasis
        elem.tipo=ASCII_OASIS;
        for(int i=0;i<config->maxOasis;i++)
        {
            aleatorio = rand()%(config->cantPosiciones-2)+2;
            casilla.posicion=aleatorio;
            if(insertarElementoSeguro(tablero, casilla, elem, pf) != TODO_OK)
                return ERROR_MEM;
        }

        ///tormenta
        elem.tipo = ASCII_TORMENTA;
        auxTormenta = config->maxTormentas;

        while(auxTormenta>0)
        {
            aleatorio = rand() % (config->cantPosiciones - 2) + 2;
            casilla.posicion = aleatorio;

            tCasilla* casillaAux = buscarElementoLista(tablero, &casilla, compararPosicion);

            if(casillaAux != NULL)
            {
                tContadorElementos cont = {0};
                recorrerDeIzqADer(&casillaAux->elementos, accionContarElementos, &cont);

                if(cont.cantOasis == 0)///inserto si NO HAY oasis en la casilla
                {
                    if(insertarElementoSeguro(tablero, casilla, elem, pf) != TODO_OK)
                        return ERROR_MEM;
                    auxTormenta--;
                }
            }
        }


        casilla.posicion=config->cantPosiciones;
        elem.tipo=ASCII_SALIDA;
        if(insertarElementoSeguro(tablero, casilla, elem, pf) != TODO_OK)
            return ERROR_MEM;


        tableroJugable = verificarTablero(tablero, config);

    } while (!tableroJugable);


    guardarTableroArchivo(tablero,pf,config,cantDigitosCasillas);


    fclose(pf);

    return TODO_OK;
}

// función que verifica que el tablero sea jugable
int verificarTablero(tLista* tablero, const tConfiguracion* config)
{
    tCasilla casillaPos;
    tContadorElementos cont = {0};
    unsigned mitadCantPosiciones = config->cantPosiciones / 2;
    int i;

    /// que no haya muchos bandidos apenas el jugador sale del inicio
    casillaPos.posicion = 2;

    /// se tiene que fijar las casillas previas al inicio (desde la casilla anterior a la salida
    /// y desde la casilla siguiente al inicio, que no haya muchos bandidos, para que el jugador
    /// no muera al inicio
    for (i = 0; i < minimo(mitadCantPosiciones, config->maxBandidos); i++)
    {
        tCasilla* casilla = buscarElementoLista(tablero, &casillaPos, compararPosicion);
        recorrerDeIzqADer(&casilla->elementos, accionContarElementos, &cont);
        casillaPos.posicion++;
    }
    casillaPos.posicion = config->cantPosiciones - 1;
    for (i = 0; i < minimo(mitadCantPosiciones, config->maxBandidos); i++)
    {
        tCasilla* casilla = buscarElementoLista(tablero, &casillaPos, compararPosicion);
        recorrerDeIzqADer(&casilla->elementos, accionContarElementos, &cont);
        casillaPos.posicion--;
    }

    /// si máS del 60% de los bandidos se encuentran en posiciones cercanas al inicio
    if (cont.cantBandido >= (unsigned)(config->maxBandidos * 0.6))
        return TABLERO_NO_JUGABLE;

    return TABLERO_JUGABLE;
}

int crearCasilla(tCasilla* casilla)
{
    crearLista(&casilla->elementos);

    return TODO_OK;
}
///pequeña funcion que valida que se pudo reservar memoria (para no repetir esto mil veces en la funcion crear tablero)
int insertarElementoSeguro(tLista* tablero,tCasilla casilla,tElemento elem,FILE* pf)
{
    if(agregarElementoEnCasilla(tablero, casilla, elem) != TODO_OK)
    {
        vaciarTablero(tablero);

        /// check porque a veces se pasa null
        if (pf != NULL)
            fclose(pf);
        return ERROR_MEM;
    }

    return TODO_OK;
}

int guardarTableroArchivo(tLista* tablero, FILE* pf, tConfiguracion* config, int cantDigitos)
{
    tCasilla* casilla;
    tCasilla casillaPos;

    for(int i = 0; i < config->cantPosiciones; i++)
    {
        casillaPos.posicion = i + 1;
        casilla = buscarElementoLista(tablero, &casillaPos, compararPosicion);

        fprintf(pf, "%0*u: [", cantDigitos, casilla->posicion);

        if(listaVacia(&casilla->elementos) == LISTA_VACIA)
        {
            // Si no hay elementos, pongo el punto indicando que el casillero esta vacio
            fprintf(pf, ".");
        }
        else
        {

           recorrerDeIzqADer(&casilla->elementos, accionEscribirArchivo, pf);
        }

        fprintf(pf, "]\n");
    }
    return TODO_OK;
}



int agregarElementoEnCasilla(tLista* tablero,tCasilla casillaPos,tElemento elem)
{
    tCasilla* casilla=buscarElementoLista(tablero,&casillaPos,compararPosicion); ///busco en la lista del tablero en que casilla vpy a insertar

    if(listaInsertarAlFinal(&casilla->elementos,&elem,sizeof(tElemento))==TODO_OK) ///en la lista de casilla inserto
        return TODO_OK;


    return ERROR_MEM;  ///VACIAR EL TABLERO
}
int eliminarElementoEnCasilla(tLista* tablero,tCasilla casillaPos,tElemento elem)
{
    tCasilla* casilla=buscarElementoLista(tablero,&casillaPos,compararPosicion);

    if(eliminarListaDesordenadaPorClaveSinDup(&casilla->elementos,&elem,sizeof(tElemento),compararElementos)==ENCONTRADO)
        return TODO_OK;

    return NO_ENCONTRADO;
}
void vaciarTablero(tLista* tablero)
{
    tCasilla casilla;

    while(listaVacia(tablero) == LISTA_NO_VACIA)
    {
        if(listaEliminarPrimerElemento(tablero, &casilla, sizeof(tCasilla)) == TODO_OK)
        {
            vaciarLista(&casilla.elementos);
        }
    }
}
void dibujarTablero(tLista* tablero, int cantPosiciones)
{
    tCasilla* casilla;
    tCasilla casillaPos;
//    int filas = (cantPosiciones + columnas - 1) / columnas; ///(A + B - 1) / B para no tener problema con los decimales
                                                            ///y que no me falten lugares
    int columnas = (int)sqrt(cantPosiciones);
    int filas = (cantPosiciones + columnas - 1) / columnas;

    for(int f = 0; f < filas; f++)
    {
        for(int c = 0; c < columnas; c++)
            printf("+----------------");
        printf("+\n");

        for(int c = 0; c < columnas; c++)
        {

            if (c == 0)
            {
                if (f % 2 != 0 && f < filas - 1)
                    printf("\u2193 ");
                else
                    printf("| ");
            }
            else
            {
                if (f % 2 == 0)
                    printf("\u2192 ");
                else
                    printf("\u2190 ");
            }


            int posLogica; ///como tiene un recorrido de "serpiente"

            if (f % 2 == 0)
                posLogica = (f * columnas) + c + 1; ///calculo desde donde inicia para imprimir
            else
                posLogica = (f * columnas) + (columnas - 1 - c) + 1;

            if (posLogica <= cantPosiciones)
            {
                casillaPos.posicion = posLogica;
                casilla = buscarElementoLista(tablero, &casillaPos, compararPosicion);

                if(listaVacia(&casilla->elementos) == LISTA_VACIA)
                {
                    printf("               ");
                }
                else
                {
                    tContadorElementos cont = { 0, 0, 0, 0, 0, 0, 0, 0 };

                    recorrerDeIzqADer(&casilla->elementos, accionContarElementos, &cont);

                    int anchoUsado = 0;

                    ///valores que nunca van a tener un numero delante
                    if (cont.cantInicio > 0)
                        anchoUsado += printf("I ");
                    if (cont.cantJugador > 0)
                        anchoUsado += printf("J ");
                    if (cont.cantSalida > 0)
                        anchoUsado += printf("S ");

                    ///en estos casos solo muestro el numero por delante cuando este es mayor a 1
                    if (cont.cantBandido > 0)
                        anchoUsado += (cont.cantBandido > 1) ? printf("%dB ", cont.cantBandido) : printf("B ");

                    if (cont.cantTormenta > 0)
                        anchoUsado += (cont.cantTormenta > 1) ? printf("%dT ", cont.cantTormenta) : printf("T ");

                    if (cont.cantPremio > 0)
                        anchoUsado += (cont.cantPremio > 1) ? printf("%dP ", cont.cantPremio) : printf("P ");

                    if (cont.cantVida > 0)
                        anchoUsado += (cont.cantVida > 1) ? printf("%dV ", cont.cantVida) : printf("V ");

                    if (cont.cantOasis > 0)
                        anchoUsado += (cont.cantOasis > 1) ? printf("%dO ", cont.cantOasis) : printf("O ");

                    ///relleno el resto de la casilla con " "
                    for(int k = anchoUsado; k < 15; k++)
                        printf(" ");
                }
            }
            else
            {
                printf("-------------- ");
            }
        }


        if (f < filas - 1)
        {
            if (f % 2 == 0)
                printf("\u2193\n");
            else
                printf("|\n");
        }
        else
            printf("|\n");

    }

    for(int c = 0; c < columnas; c++)
        printf("+----------------");
    printf("+\n");
}






///FUNCIONES AUXILIARES DE CMP Y ACCION
int compararPosicion(const void* a,const void* b)
{
    tCasilla* auxA=(tCasilla*)a;
    tCasilla* auxB=(tCasilla*)b;


    return auxA->posicion-auxB->posicion;

}

int compararElementos(const void* a, const void* b)
{
    tElemento* elemA = (tElemento*)a;
    tElemento* elemB = (tElemento*)b;

    return elemA->tipo - elemB->tipo;
}

int compararBandidos(const void* a,const void* b)
{
    tBandido* bandidoA = (tBandido*)a;
    tBandido* bandidoB = (tBandido*)b;

    return bandidoA->id - bandidoB->id;
}

int compararPosBandidos(const void* a, const void* b)
{
    tBandido* bandidoA = (tBandido*)a;
    tBandido* bandidoB = (tBandido*)b;

    return bandidoA->posBandido - bandidoB->posBandido;
}

void mostrarElementos(const void* elem, const void* extra)
{
    tElemento* elemento = (tElemento*)elem;
    printf("Elemento: %c\n", elemento->tipo);
}

void mostrarBandidos(const void* elem, const void* extra)
{
    tBandido* bandido = (tBandido*)elem;
    printf("Id de bandido: %u | Posicion: %u\n", bandido->id, bandido->posBandido);
}

void accionContarElementos(const void* elem, const void* extra)
{
    tElemento* e = (tElemento*)elem;
    tContadorElementos* cont = (tContadorElementos*)extra;

    switch(e->tipo)
    {
        case ASCII_INICIO:
            cont->cantInicio++;
            break;
        case ASCII_JUGADOR:
            cont->cantJugador++;
            break;
        case ASCII_BANDIDO:
            cont->cantBandido++;
            break;
        case ASCII_PREMIO:
            cont->cantPremio++;
            break;
        case ASCII_VIDA_EXTRA:
            cont->cantVida++;
            break;
        case ASCII_OASIS:
            cont->cantOasis++;
            break;
        case ASCII_TORMENTA:
             cont->cantTormenta++;
             break;
        case ASCII_SALIDA:
            cont->cantSalida++;
            break;
    }

}

void accionContarElementosDesdeTablero(const void* elem, const void* extra)
{
    tCasilla* e = (tCasilla*)elem;
    tContadorElementos* cont = (tContadorElementos*)extra;

    recorrerDeIzqADer(&e->elementos, accionContarElementos, cont);
}

void accionActualizarBandido(const void* elem, const void* extra)
{
    tBandido* b = (tBandido*)elem;
    unsigned nuevaPos = *(unsigned*)extra;

    ///para testear
    printf("Bandido %-2u: Pos anterior: %-2u | Nueva pos: %-2u\n", b->id, b->posBandido, nuevaPos);

    b->posBandido = nuevaPos;
    b->movimientos++;
}

void accionImprimirConsola(const void* elem, const void* extra)
{
    tElemento* e = (tElemento*)elem;
    printf("%c ", e->tipo);
}

void accionEscribirArchivo(const void* elem, const void* pf)
{
    tElemento* e = (tElemento*)elem;
    if (pf != NULL)
    {
        fprintf((FILE*)pf, "%c", e->tipo);
    }
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
    int nroTurno = 1;

    if (abrirArchivo(&arch, nomArch, "wt") != TODO_OK)
    {
        perror("Error al guardar historial");
        return ERROR_ARCH;
    }

    printf("\n===== REGISTRO DE MOVIMIENTOS =====\n");
    fprintf(arch, "===== REGISTRO DE MOVIMIENTOS =====\n");

    if (colaVacia(historial))
    {
        printf("No se realizaron movimientos.\n");
        fprintf(arch, "No se realizaron movimientos.\n");
    }
    else
    {
        do {
            sacarDeCola(historial, &mov, sizeof(mov));
            printf("Turno %2d: %c%u\n", nroTurno, mov.direccion, mov.cantMovim);
            fprintf(arch, "Turno %2d: %c%u\n", nroTurno, mov.direccion, mov.cantMovim);
            nroTurno++;
        } while (!colaVacia(historial));
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





