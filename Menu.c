#include "Turno.h"
#include "Menu.h"
#include "Lista.h"

void pedirNombre(char* nombre)
{
    printf("Ingrese su nombre: ");

    fgets(nombre, MAX_NOMBRE, stdin);

    if(strchr(nombre, '\n'))
        nombre[strcspn(nombre, "\n")] = '\0';
    else
        limpiarBuffer();

}
// Muestra el menu y devuelve la opcion elegida
int mostrarMenu()
{
    int opcion;

    do {
        printf("\n==============================\n");
        printf("     CARAVANA DEL DESIERTO    \n");
        printf("==============================\n");
        printf("  %d. Jugar nueva partida     \n", OPCION_JUGAR);
        printf("  %d. Ver ranking             \n", OPCION_RANKING);
        printf("  %d. Salir                   \n", OPCION_SALIR);
        printf("==============================\n");
        printf("Opcion: ");

        if(scanf("%d", &opcion) != 1)
        {
            printf("Entrada invalida.\n");
            limpiarBuffer();
            opcion = -1;
            continue;
        }

        limpiarBuffer();

        if(opcion != OPCION_JUGAR &&
           opcion != OPCION_RANKING &&
           opcion != OPCION_SALIR)
        {
            printf("Opcion invalida.\n");
        }

    } while(opcion != OPCION_JUGAR &&
            opcion != OPCION_RANKING &&
            opcion != OPCION_SALIR);

    return opcion;
}

// Muestra el ranking (para mas adelante)
void mostrarRanking(const char* nomArch)
{
    FILE* pf;
    tJugador jug;

    pf = fopen(nomArch, "rb");
    if(!pf)
    {
        printf("No hay jugadores registrados.\n");
        return;
    }

    printf("\n=====================================================\n");
    printf("                         RANKING                      \n");
    printf("=====================================================\n");
    while(fread(&jug, sizeof(tJugador), 1, pf) == 1)
        mostrarJugador(&jug);
    printf("=====================================================\n");

    fclose(pf);
}


void iniciarPartida(tConfiguracion* config)
{
    tPartida partida;
    tLista tablero;
    tLista bandidos;
    tCola historial;
    tCasilla casilla;
    tJugador jugador;

    char nombre[MAX_NOMBRE];  ///deberia ser parte de una estructura jugador
    int estado=JUEGO_CONTINUA;
    crearLista(&tablero);
    crearLista(&bandidos);
    crearCola(&historial);

    partida.posJugador=1;
    partida.cantPuntos = 0;
    partida.cantVidas = config->vidasInicio;
    partida.oasis = 0;
    partida.tormenta = 0;

    ///ESTA VALIDACION ME PARECE QUE ESTA DE MAS
    if (crearTablero(NOM_ARCH_TABLERO, &tablero, config, &bandidos) != TODO_OK)
    {
        printf("Error al crear el tablero. Volviendo al menu...\n");
        vaciarLista(&tablero);
        vaciarCola(&historial);
        return;
    }

    pedirNombre(nombre);
    if(buscarJugador(NOM_ARCH_JUGADORES, nombre, &jugador) == NO_ENCONTRADO)
    {
        jugador.idJugador = obtenerUltimoID(NOM_ARCH_JUGADORES) + 1;

        strcpy(jugador.nombre, nombre);
        jugador.totalPuntos = 0;
        jugador.partidasJugadas = 0;

        altaJugador(NOM_ARCH_JUGADORES, &jugador);

        printf("El jugador '%s' se ha creado correctamente.\n", nombre);
    }
    else
        printf("Jugador encontrado.\n");


    system("cls");
    printf("\n=========================================\n");
    printf("  Bienvenido a la caravana, %s!\n", nombre);
    printf("=========================================\n");
    printf("Vidas iniciales: %u | Puntos: %u\n\n", partida.cantVidas, partida.cantPuntos);
    system("pause");
    system("cls");



    /// vvv LOGICA DEL JUEGO vvv

    while(partida.cantVidas>0 && estado==JUEGO_CONTINUA) //agregar condicion de que slae porque gana
    {
        dibujarTablero(&tablero,config->cantPosiciones,5);//definir columnas!
        casilla.posicion = partida.posJugador;
        estado = manejarSituacionCasilla(&partida,&tablero,&bandidos,casilla);
        if (estado == JUEGO_CONTINUA)
        {
            estado=ejecutarTurnoJugador(&tablero,&partida,&historial,config);
            casilla.posicion = partida.posJugador;
            estado = manejarSituacionCasilla(&partida,&tablero,&bandidos,casilla);
            dibujarTablero(&tablero,config->cantPosiciones,5);
            if (estado == JUEGO_CONTINUA)
                ejecutarMovimientoBandido(&tablero,&bandidos,partida.posJugador, config->cantPosiciones);
        }

    }


    /// ^^^ LOGICA DEL JUEGO ^^^


    if (JUGADOR_GANO==estado)
        printf("FELICITACIONES %s! Llegaste a la salida y ganaste.\n", nombre);
    else
        if (JUGADOR_PERDIO==estado)
            printf("GAME OVER. Te quedaste sin vidas :(.\n");
        else
        {
            fprintf(stderr, "ERROR. Algo paso...\n");
            vaciarTablero(&tablero);
            vaciarLista(&bandidos);
            vaciarCola(&historial);
        }

    guardarMostrarHistorial(&historial, NOM_ARCH_MOVIMIENTOS);

    jugador.totalPuntos += partida.cantPuntos;
    jugador.partidasJugadas++;

    actualizarJugador(NOM_ARCH_JUGADORES, &jugador);

    vaciarTablero(&tablero);
    vaciarLista(&bandidos);
    vaciarCola(&historial);
}


///Para interpretar lo que sucede cuando el jugador cae en una nueva casilla
int manejarSituacionCasilla(tPartida* partida,tLista* tablero,tLista* bandidos,tCasilla casillaPosicion)
{
    tCasilla* casilla=buscarElementoLista(tablero,&casillaPosicion,compararPosicion);
    tContadorElementos cont = {0};
    tElemento elem;
    int i;

    recorrerDeIzqADer(&casilla->elementos, accionContarElementos, &cont);

    if (cont.cantVida > 0)
    {
        partida->cantVidas += cont.cantVida;
        printf("\u00A1Encontraste %d vida/s extra! Ahora tenes %u vidas.\n", cont.cantVida, partida->cantVidas);  ///mejorar mensaje
        elem.tipo=ASCII_VIDA_EXTRA;
        for(i=0;i<cont.cantVida;i++)
            eliminarElementoEnCasilla(tablero,casillaPosicion,elem);
    }

    if (cont.cantBandido > 0 && partida->oasis == 0)
    {
        printf("\u00A1Te han atacado %d bandidos!\n",cont.cantBandido);
        if(cont.cantBandido>=partida->cantVidas)
            return JUGADOR_PERDIO;

        partida->cantVidas -= cont.cantBandido; ///VALIDO SI ES NEGATIVO O CERO EN EJECUTARtURNOjuagador

        /// eliminar el jugador de la casilla actual, y posicionarlo al inicio del tablero
        elem.tipo=ASCII_JUGADOR;
        eliminarElementoEnCasilla(tablero,casillaPosicion,elem);

        /// posicionar al jugador al inicio del tablero
        casillaPosicion.posicion = 1;
        if(insertarElementoSeguro(tablero, casillaPosicion, elem, NULL) != TODO_OK)
            return ERROR_MEM;

        /// eliminar los bandidos de la casilla, y de la lista de bandidos
        elem.tipo=ASCII_BANDIDO;
        casillaPosicion.posicion = partida->posJugador;
        for(i=0;i<cont.cantVida;i++)
            eliminarElementoEnCasilla(tablero,casillaPosicion,elem);

        partida->posJugador = 1;
    }

    if (cont.cantBandido > 0 && partida->oasis != 0 ) //se cubre por un oasis
        printf("¡Te salvas por el oasis! Los bandidos no pueden atacarte.\n");

    if(cont.cantTormenta>0 && partida->oasis == 0)
    {
        if(cont.cantTormenta>=partida->cantVidas)
            return JUGADOR_PERDIO;
        partida->cantVidas -=cont.cantTormenta;
        printf("¡Has pasado por una tormenta, pierdes %d vidas!\n",cont.cantTormenta);


    }

    if(cont.cantTormenta>0 && partida->oasis == 1)
        printf("¡Te salvas por el oasis! Las tormentas no pueden dañarte.\n");

    if(cont.cantTormenta)
    {
        elem.tipo=ASCII_TORMENTA;
        for(i=0;i<cont.cantTormenta;i++)
            eliminarElementoEnCasilla(tablero,casillaPosicion,elem);
    }

    if(cont.cantBandido)
    {
        elem.tipo=ASCII_BANDIDO;
        for(i=0;i<cont.cantBandido;i++)
            eliminarElementoEnCasilla(tablero,casillaPosicion,elem);
    }

    if((cont.cantTormenta>0 || cont.cantBandido>0) && partida->oasis==1 )
        partida->oasis=0;

    if(cont.cantOasis>=1)
    {
        printf("¡Tenes proteccion en el proximo turno!\n");
        partida->oasis=1;

        elem.tipo=ASCII_OASIS;
        for(i=0;i<cont.cantOasis;i++)
            eliminarElementoEnCasilla(tablero,casillaPosicion,elem);
    }

    if(cont.cantPremio>0)
    {
        partida->cantPuntos++;
        printf("\u00A1Has encontrado un premio! Sumas 1 punto.\n");

        elem.tipo=ASCII_PREMIO;
        for(i=0;i<cont.cantPremio;i++)
            eliminarElementoEnCasilla(tablero,casillaPosicion,elem);
    }


    return JUEGO_CONTINUA;
}



int ejecutarTurnoJugador(tLista* tablero,tPartida* partida, tCola* historial,tConfiguracion* config)
{
    int dado;
    char direccion;
    tElemento elem;
    tCasilla casilla;
    unsigned nuevaPos;

    printf("Vidas: %u | Puntos: %u\n", partida->cantVidas, partida->cantPuntos);

    if (partida->tormenta)
    {
        printf("\nEstas atrapado en una tormenta. Perdes este turno.\n");
        partida->tormenta = 0;
        return JUEGO_CONTINUA;
    }

    if (partida->oasis)
    {
        printf("La proteccion del oasis expiro.\n");
        partida->oasis = 0;
    }

    dado=tirarDado();
    printf("El valor del dado es: %d\n",dado);
    direccion=pedirDireccion();

    nuevaPos=partida->posJugador;
    if(FORWARD==direccion)
    {
        if(partida->posJugador+dado<=config->cantPosiciones)
            nuevaPos+=dado;
        else
        {
            unsigned exceso= (partida->posJugador+dado)-config->cantPosiciones;
            nuevaPos=config->cantPosiciones-exceso;
            printf("\u00A1Rebotaste en la salida! Te posicionas en la casilla %d\n",nuevaPos);
        }
    }
    else  ///direccion backward
    {
        if((int)(partida->posJugador)-dado >= 1)
            nuevaPos-=dado;
        else ///TOPE INFERIOR, ENTIENDO QUE LA DIRECCION PASA A SER FORWARD
        {
            nuevaPos+=dado;
            printf("No podes retroceder mas alla del inicio del tablero. Tu direccion es hacia adelante.\n");
        }
    }

    elem.tipo=ASCII_JUGADOR;

    casilla.posicion=partida->posJugador;
    if (eliminarElementoEnCasilla(tablero,casilla,elem) != TODO_OK)
        return JUEGO_ERROR;

    registrarMovimiento(historial, direccion, dado);



    casilla.posicion=nuevaPos;
    partida->posJugador=nuevaPos;



//    if(manejarSituacionCasilla(partida,tablero,casilla)==JUGADOR_PERDIO)
//        return JUGADOR_PERDIO;

    insertarElementoSeguro(tablero,casilla,elem,NULL);

    system("pause");
    limpiarBuffer();
    //system("cls");

    if(partida->posJugador==config->cantPosiciones)
        return JUGADOR_GANO;


    return JUEGO_CONTINUA;
}

char pedirDireccion()
{
    int opcion;

    do {
        printf("\n  %d. Avanzar\n", OPCION_AVANZAR);
        printf("  %d. Retroceder\n", OPCION_RETROCEDER);
        printf("Opcion: ");

        if (scanf(" %d", &opcion) != 1)
        {
            limpiarBuffer();
            opcion = -1;     // fuerza reintentar
            continue;
        }

        if (opcion != OPCION_AVANZAR && opcion != OPCION_RETROCEDER)
            printf("Opcion invalida. Intenta nuevamente.\n");

    } while (opcion != OPCION_AVANZAR && opcion != OPCION_RETROCEDER);

    return (opcion == OPCION_AVANZAR) ? FORWARD : BACKWARD;
}


void limpiarBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    ///get char lee y elimina de a un caracter del buffer hasta que no hay mas datos (EOF)
}

