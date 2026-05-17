#include "Juego.h"
#include "Archivo.h"

//esta función crea el archivo del tablero en base a la configuración, un tablero jugable
//llama a otra función que valida si la configuración, para poder crear el tablero




int crearTablero(const char* nomArch,tLista* tablero,tConfiguracion* config)
{
    FILE* pf;

    if(TODO_OK!=abrirArchivo(&pf,NOM_ARCH_TABLERO,"wt"))
        return ERROR_ARCH;


    int cantDigitosCasillas = cantDigitosUnsigned(config->cantPosiciones);


    ///creo todas las listas de las casillas

    tCasilla casilla;
    tElemento elem;

    for(int i=0;i<config->cantPosiciones;i++)
    {
        crearCasilla(&casilla);
        casilla.posicion=i+1;


        if(listaInsertarAlFinal(tablero,&casilla,sizeof(tCasilla))!=TODO_OK)
        {
            vaciarLista(tablero);
            return  ERROR_MEM;
        }
    }

    ///agrego el primer elemento

    casilla.posicion=1;
    elem.tipo=ASCII_INICIO;
    agregarElementoEnCasilla(tablero,casilla,elem); ///validar si se pudo
    elem.tipo=ASCII_JUGADOR;
    agregarElementoEnCasilla(tablero,casilla,elem);



    unsigned aleatorio;
    ///bandidos
    elem.tipo=ASCII_BANDIDO;
    for(int i=0;i<config->maxBandidos;i++)
    {
        aleatorio = rand()%(config->cantPosiciones-2)+2;
        casilla.posicion=aleatorio;
        agregarElementoEnCasilla(tablero,casilla,elem);
    }

    ///premios
    elem.tipo=ASCII_PREMIO;
    for(int i=0;i<config->maxPremios;i++)
    {
        aleatorio=rand()%config->cantPosiciones+1;
        casilla.posicion=aleatorio;
        agregarElementoEnCasilla(tablero,casilla,elem);
    }

    ///vidas
    elem.tipo=ASCII_VIDA_EXTRA;
    for(int i=0;i<config->maxVidasExtra;i++)
    {
        aleatorio=rand()%config->cantPosiciones+1;
        casilla.posicion=aleatorio;
        agregarElementoEnCasilla(tablero,casilla,elem);
    }


    ///oasis
    elem.tipo=ASCII_OASIS;
    for(int i=0;i<config->maxOasis;i++)
    {
        aleatorio=rand()%config->cantPosiciones+1;
        casilla.posicion=aleatorio;
        agregarElementoEnCasilla(tablero,casilla,elem);
    }

    ///tormenta
    elem.tipo=ASCII_TORMENTA;
    for(int i=0;i<config->maxTormentas;i++)
    {
        aleatorio = rand()%(config->cantPosiciones-2)+2;
        casilla.posicion=aleatorio;
        agregarElementoEnCasilla(tablero,casilla,elem);
    }

    casilla.posicion=config->cantPosiciones;
    elem.tipo=ASCII_SALIDA;
    agregarElementoEnCasilla(tablero,casilla,elem);





    guardarTableroArchivo(tablero,pf,config,cantDigitosCasillas);


    fclose(pf);

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
            // Si no hay elementos, ponemos el punto de vacío
            fprintf(pf, ".");
        }
        else
        {
           // Pasamos 'pf' como tercer argumento para que llegue a la acción
           recorrerDeIzqADer(&casilla->elementos, (Accion)accionEscribirArchivo, pf);
        }

        fprintf(pf, "]\n");
    }
    return TODO_OK;
}

/// Accion para imprimir en consola (dibujarTablero)

void accionImprimirConsola(const void* elem, const void* extra)
{
    tElemento* e = (tElemento*)elem;
    printf("%c ", e->tipo);
}

/// Accion para escribir en archivo (guardarTableroArchivo)
void accionEscribirArchivo(const void* elem, const void* pf)
{
    tElemento* e = (tElemento*)elem;
    if (pf != NULL)
    {
        fprintf((FILE*)pf, "%c", e->tipo);
    }
}

void dibujarTablero(tLista* tablero, int cantPosiciones, int columnas)
{
    tCasilla* casilla;
    tCasilla casillaPos;
    int filas = (cantPosiciones + columnas - 1) / columnas;

    for(int f = 0; f < filas; f++)
    {

        for(int c = 0; c < columnas; c++)
            printf("+--------");
        printf("+\n");


        for(int c = 0; c < columnas; c++)
        {
            int posLogica;


            if (f % 2 == 0)
                posLogica = (f * columnas) + c + 1;
            else
                posLogica = (f * columnas) + (columnas - 1 - c) + 1;


            printf("| ");

            if (posLogica <= cantPosiciones)
            {
                casillaPos.posicion = posLogica;
                casilla = buscarElementoLista(tablero, &casillaPos, compararPosicion);

                if(listaVacia(&casilla->elementos) == LISTA_VACIA)
                    printf(".      ");
                else
                {
                    recorrerDeIzqADer(&casilla->elementos, (Accion)accionImprimirConsola, NULL);
                    unsigned cant = listaCantidadElementos(&casilla->elementos);
                    int anchoUsado = cant * 2;
                    for(int k = anchoUsado; k < 7; k++) printf(" ");
                }
            }
            else
                printf("        ");

        }
        printf("|\n");
    }

    for(int c = 0; c < columnas; c++)
        printf("+--------");
    printf("+\n");
}
int agregarElementoEnCasilla(tLista* tablero,tCasilla casillaPos,tElemento elem)
{
    tCasilla* casilla=buscarElementoLista(tablero,&casillaPos,compararPosicion); ///busco en la lista del tablero en que casilla vpy a insertar

    if(listaInsertarAlFinal(&casilla->elementos,&elem,sizeof(tElemento))==TODO_OK) ///en la lista de casilla inserto
        return TODO_OK;


    return ERROR_MEM;  ///VACIAR EL TABLERO
}

int crearCasilla(tCasilla* casilla)
{
    crearLista(&casilla->elementos);

    return TODO_OK;
}

int compararPosicion(const void* a,const void* b)
{
    tCasilla* auxA=(tCasilla*)a;
    tCasilla* auxB=(tCasilla*)b;


    return auxA->posicion-auxB->posicion;

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
