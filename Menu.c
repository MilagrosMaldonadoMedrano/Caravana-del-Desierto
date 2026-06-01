#include "Turno.h"
#include "Menu.h"
#include "Lista.h"

#include "Archivo.h"

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

    tJugador vec[MAX_JUGADORES];
    int cantJugadores;

    cantJugadores = cargarJugadores(nomArch, vec);
    if(cantJugadores == 0)
    {
        printf("No hay jugadores registrados.\n");
        return;
    }
    ordenarJugadores(vec, cantJugadores);
    printf("\n=====================================================\n");
    printf("                         RANKING                      \n");
    printf("=====================================================\n");
    mostrarVectorJugadores(vec, cantJugadores);
    printf("=====================================================\n");

}


void iniciarPartida(tConfiguracion* config)
{
    tPartida partida;
    tLista tablero;
    tLista bandidos;
    tCola historial;
    tCola bufferMovs;
    tCasilla casilla;
    tJugador jugador;
    tRegistroPartida reg;

    char nombre[MAX_NOMBRE];  ///deberia ser parte de una estructura jugador
    int estado=JUEGO_CONTINUA;
    crearLista(&tablero);
    crearLista(&bandidos);
    crearCola(&historial);
    crearCola(&bufferMovs);

    partida.posJugador=1;
    partida.cantPuntos = 0;
    partida.movimientos = 0;
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
        system("pause");
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


    printf("Vidas: %u | Puntos: %u\n", partida.cantVidas, partida.cantPuntos);
    dibujarTablero(&tablero,config->cantPosiciones,5);//definir columnas!
    ///---------------BUCLE DEL JUEGO------------------------

    while(partida.cantVidas>0 && estado==JUEGO_CONTINUA)
    {

        if (estado == JUEGO_CONTINUA)
        {
            ///planifico el mov del jugador
            planificarMovimientoJugador(config,&bufferMovs,&historial,&partida);
            planificarMovimientosBandidos(config,&bufferMovs,&tablero,&bandidos);
            procesarTurno(&tablero,&bufferMovs,&partida);

            casilla.posicion=partida.posJugador;
            estado = manejarSituacionCasilla(&partida,&tablero,&bandidos,casilla,config);
        }

        printf("Vidas: %u | Puntos: %u\n", partida.cantVidas, partida.cantPuntos);
        dibujarTablero(&tablero,config->cantPosiciones,5);//definir columnas!

    }
    ///------------------------------------------------------
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

    reg.idPartida = obtenerUltimoIdPartida(NOM_ARCH_PARTIDAS) + 1;
    reg.idJugador = jugador.idJugador;
    reg.puntos = partida.cantPuntos;
    reg.movimientos = partida.movimientos;
    reg.gano = (estado == JUGADOR_GANO);
    altaPartida(NOM_ARCH_PARTIDAS, &reg);

    jugador.totalPuntos += partida.cantPuntos;
    jugador.partidasJugadas++;

    actualizarJugador(NOM_ARCH_JUGADORES, &jugador);

    vaciarTablero(&tablero);
    vaciarLista(&bandidos);
    vaciarCola(&historial);
    vaciarCola(&bufferMovs);
}


///Funciones para los turnos y el estado del tablero
int planificarMovimientoJugador(tConfiguracion* config,tCola* bufferMovs,tCola* historial,tPartida* partida)
{
    int dado;

    unsigned nuevaPos;
    tMovimiento mov;

    if (partida->tormenta)
    {
        printf("\nEstas atrapado en una tormenta. Perdes este turno.\n");
        partida->tormenta = 0;

        mov.elem.tipo=ASCII_JUGADOR;
        mov.posOrigen=partida->posJugador;
        mov.posFinal=partida->posJugador;
        ponerEnCola(bufferMovs, &mov,sizeof(tMovimiento));

        return JUEGO_CONTINUA;
    }

    dado=tirarDado();
    printf("El valor del dado es: %d\n",dado);
    mov.direccion=pedirDireccion();

    nuevaPos=partida->posJugador;
    if(FORWARD==mov.direccion)
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


    mov.elem.tipo=ASCII_JUGADOR;
    mov.posOrigen=partida->posJugador;
    mov.posFinal=nuevaPos;
    ponerEnCola(bufferMovs, &mov,sizeof(tMovimiento));
    mov.cantMovim=dado;
    ponerEnCola(historial, &mov,sizeof(tMovimiento));

    return JUEGO_CONTINUA;
}

void planificarMovimientosBandidos(tConfiguracion* config, tCola* bufferMovs, tLista* tablero, tLista* bandidos)
{
    int dado;
    unsigned pos = 0;
    unsigned posJugador = 1;
    unsigned cantBandidos;
//    unsigned nuevaPosBandido;
//    unsigned posBandidoAMover;
//    unsigned menorDistancia;
//    char direccion;
//    char pri = 1;
//    tElemento elem;
    tMovimiento mov;
    tBandido bandido;
    tCasilla casillaPosicion;
    tCasilla* casilla=NULL;
    tContadorElementos cont = {0};

    if (!*bandidos)
        return; /// no hay bandidos en el tablero

    cantBandidos = listaCantidadElementos(bandidos);

    do
    {
        ///deberia resetear el contador en cada iteración
        casillaPosicion.posicion = posJugador;
        casilla = buscarElementoLista(tablero,&casillaPosicion,compararPosicion);
        recorrerDeIzqADer(&casilla->elementos, accionContarElementos, &cont);
        posJugador++;
        //recorrerDeIzqADer(tablero, accionContarElementosDesdeTablero, &cont);
    } while (cont.cantJugador == 0);
//    printf("\nPosiciones de los bandidos en el tablero:\n");
//    recorrerDeIzqADer(bandidos, mostrarBandidos, NULL);
    /// Primero que busque al jugador, luego se vé implementar el resto...

    mov.elem.tipo = ASCII_BANDIDO;

    while (cantBandidos > 0)
    {
        dado = tirarDado();
        if (recuperarUltElemOperadoEnLista(bandidos, &bandido, sizeof(bandido)) == TODO_OK)
        {
            unsigned posNuevaAvanzando = (bandido.posBandido + dado) % config->cantPosiciones;
            unsigned posNuevaRetrocediendo = (config->cantPosiciones + (bandido.posBandido - dado)) % config->cantPosiciones;
            unsigned distanciaAvanzando;
            unsigned distanciaRetrocediendo;

            posNuevaAvanzando == 0 ? posNuevaAvanzando = config->cantPosiciones : posNuevaAvanzando;

            distanciaAvanzando = MODULO((int)posNuevaAvanzando - posJugador);
            distanciaAvanzando > config->cantPosiciones / 2 ? distanciaAvanzando = config->cantPosiciones - distanciaAvanzando : distanciaAvanzando;
            distanciaRetrocediendo = MODULO((int)posNuevaRetrocediendo - posJugador);
            distanciaRetrocediendo > config->cantPosiciones / 2 ? distanciaRetrocediendo = config->cantPosiciones - distanciaRetrocediendo : distanciaRetrocediendo;

//            if (pri || distanciaAvanzando < menorDistancia || distanciaRetrocediendo < menorDistancia)
//            {

                if (distanciaAvanzando < distanciaRetrocediendo)
                {
                    mov.direccion = FORWARD;
                    mov.posFinal = posNuevaAvanzando;
//                    direccion = FORWARD;
//                    menorDistancia = distanciaAvanzando;
//                    nuevaPosBandido = posNuevaAvanzando;
                }
                else
                {
                    mov.direccion = BACKWARD;
                    mov.posFinal = posNuevaRetrocediendo;
//                    direccion = BACKWARD;
//                    menorDistancia = distanciaRetrocediendo;
//                    nuevaPosBandido = posNuevaRetrocediendo;
                }

                mov.posOrigen = bandido.posBandido;
                mov.cantMovim = dado;

                ///actualizo la pos del bandido
                buscarElementoEnLista(bandidos, &bandido, sizeof(bandido), compararBandidos, accionActualizarPosBandido, &mov.posFinal);

                ///inserto el movimiento del bandido en la cola
                ponerEnCola(bufferMovs, &mov, sizeof(tMovimiento));

//                posBandidoAMover = bandido.posBandido;
//                pri = 0;
//            }
            pos++;
        }
        cantBandidos--;
    }

}

void procesarTurno(tLista* tablero, tCola* bufferMovs, tPartida* partida)
{
    tMovimiento mov;
    tCasilla casilla;
    tElemento elem;

    ///como se que el primer elemento de la cola es el jugador podria desencolarlo aca asi me ahorro el if el while

    while(TODO_OK==sacarDeCola(bufferMovs,&mov, sizeof(tMovimiento)))
    {
        elem.tipo=mov.elem.tipo;
        elem.id=mov.elem.id;

        ///Saco el elemento de la posicion vieja
        casilla.posicion=mov.posOrigen;
        if (eliminarElementoEnCasilla(tablero,casilla,elem) != TODO_OK)
        {
            fprintf(stderr, "Error, algo paso al eliminar de la pos anterior...\n");
            system("pause");
        }

        ///Pongo el elemento en la nueva posicion
        casilla.posicion=mov.posFinal;
        if (insertarElementoSeguro(tablero,casilla,elem,NULL) != TODO_OK)
        {
            fprintf(stderr, "Error, algo paso al insertar en la nueva pos...\n");
            system("pause");
        }

        if(mov.elem.tipo==ASCII_JUGADOR)
        {
            partida->posJugador=casilla.posicion;
            partida->movimientos++;
        }
    }

}



///El elemento mas importante de la casilla es la vida
///Despues revisa si hay un bandido, si es veradero le quita una vida y vuelve al inicio
///Si no hay bandidos se procesan los otros elementos (oasis, premios, tormentas)


///Para interpretar lo que sucede una vez actualizado el tablero
int manejarSituacionCasilla(tPartida* partida, tLista* tablero, tLista* bandidos, tCasilla casillaPosicion, tConfiguracion* config)
{
    tCasilla* casilla = buscarElementoLista(tablero, &casillaPosicion, compararPosicion);
    tContadorElementos cont = {0};
    tElemento elem;
    tBandido bandidoAEliminar;

    ///cuento todos los elementos
    recorrerDeIzqADer(&casilla->elementos, accionContarElementos, &cont);


    ///primero veo si llego al final de la casilla
    if(partida->posJugador==config->cantPosiciones)
        return JUGADOR_GANO;


    if (cont.cantVida > 0)
    {
        partida->cantVidas++;
        printf("\u00A1Encontraste %d vida/s extra! Ahora tenes %u vidas.\n", cont.cantVida, partida->cantVidas);  ///mejorar mensaje
        elem.tipo=ASCII_VIDA_EXTRA;
        eliminarElementoEnCasilla(tablero,casillaPosicion,elem);
    }

    ///BANDIDOS

    if(cont.cantBandido>0)
    {

        if(partida->oasis==FALSO)
        {
            printf("\u00A1Te ha atacado un bandido!\n");

            partida->cantVidas--;
            if(partida->cantVidas==0)
                return JUGADOR_PERDIO;


            /// eliminar al jugador de la casilla actual
            elem.tipo=ASCII_JUGADOR;
            eliminarElementoEnCasilla(tablero,casillaPosicion,elem);

            /// eliminar los bandidos de la casilla, y de la lista de bandidos
            elem.tipo=ASCII_BANDIDO;
            if (eliminarElementoEnCasilla(tablero, casillaPosicion, elem) != TODO_OK)
            {
                fprintf(stderr, "Error. No encontro el bandido en la casilla\n");
                system("pause");
            }

            bandidoAEliminar.posBandido = partida->posJugador;
            eliminarListaDesordenadaPorClaveSinDup(bandidos, &bandidoAEliminar,
                sizeof(tBandido), compararBandidos);

            /// posicionar al jugador al inicio del tablero
            elem.tipo=ASCII_JUGADOR
            casillaPosicion.posicion = 1;
            if(insertarElementoSeguro(tablero, casillaPosicion, elem, NULL) != TODO_OK)
                return ERROR_MEM;

            partida->posJugador = 1;
            printf("El bandido te mando de vuelta al inicio.\n");
            return JUEGO_CONTINUA;
        }
        else
            printf("\u00A1Te salvas por el oasis! El bandido no puede atacarte.\n");
    }


    ///Tormenta
    if(cont.cantTormenta>0)
    {
        if(partida->oasis==FALSO)
        {
            partida->tormenta=VERDADERO;
            printf("\u00A1Has pasado por una tormenta! Perdes el siguiente turno.\n");
            elem.tipo=ASCII_TORMENTA;
            eliminarElementoEnCasilla(tablero,casillaPosicion,elem);
        }
        else
            printf("\u00A1Te salvas por el oasis! Las tormentas no pueden da%carte.\n", COD_ASCII_ENIE);
    }

    ///OASIS
    if (partida->oasis)
    {
        printf("\u00A1Termin%c la proteccion del oasis!\n", COD_ASCII_O_TILDE);
        partida->oasis=FALSO;
    }

    if(cont.cantOasis>=1)
    {
        printf("\u00A1Tenes proteccion en el proximo turno!\n");
        partida->oasis=VERDADERO;

        elem.tipo=ASCII_OASIS;
        eliminarElementoEnCasilla(tablero,casillaPosicion,elem);
    }

    ///PREMIO
    if(cont.cantPremio>0)
    {
        partida->cantPuntos++;
        printf("\u00A1Has encontrado un premio! Sumas 1 punto.\n");

        elem.tipo=ASCII_PREMIO;
        eliminarElementoEnCasilla(tablero,casillaPosicion,elem);
    }


    return JUEGO_CONTINUA;
}



/*int ejecutarTurnoJugador(tLista* tablero,tPartida* partida, tCola* historial,tConfiguracion* config)
{
    int dado;
    char direccion;
    tElemento elem;
    tCasilla casilla;
    unsigned nuevaPos;



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
    partida->movimientos++;



//    if(manejarSituacionCasilla(partida,tablero,casilla)==JUGADOR_PERDIO)
//        return JUGADOR_PERDIO;

    insertarElementoSeguro(tablero,casilla,elem,NULL);

    system("pause");
    limpiarBuffer();
    //system("cls");

    if(partida->posJugador==config->cantPosiciones)
        return JUGADOR_GANO;


    return JUEGO_CONTINUA;
}*/

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


/// ésta podría ir en Herramientas.c
void limpiarBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    ///get char lee y elimina de a un caracter del buffer hasta que no hay mas datos (EOF)
}

