#include "Turno.h"
#include "Menu.h"
#include "Lista.h"
#include "Archivo.h"
#include "Indice.h"

int identificarJugador(char* nombre, char* nickname)
{
    int opcion;
    int identificacion;
    printf("\n============================\n");
    printf(  "      ACCESO AL JUEGO       \n");
    printf(  "============================\n");

    do
    {
        printf("1. Soy nuevo\n");
        printf("2. Ya tengo nickname\n");
        printf("Opcion: ");
        scanf("%d", &opcion);
    }while(opcion!=1 && opcion!=2);
    limpiarBuffer();

    if(opcion==1)
    {
        int numero=rand()%999;
        char cadena[5];
        printf("Ingrese su nombre: ");
        memset(nickname, 0, MAX_NICK);
        fgets(nombre, MAX_NOMBRE, stdin);

        if(strchr(nombre, '\n'))
            nombre[strcspn(nombre, "\n")] = '\0';
        else
            limpiarBuffer();

        strcpy(nickname,nombre);
        strcat(nickname,"_");
        sprintf(cadena, "%d", numero);
        strcat(nickname,cadena);

        printf("\n====================================\n");
        printf(  "        REGISTRO EXITOSO!           \n");
        printf(  "====================================\n");
        printf(  "  Nickname: %-22s\n", nickname);
        printf(  "   Guardalo para la proxima vez.    \n");
        printf(  "====================================\n");

        identificacion=NOMBRE;
    }
    else
    {
        printf("Ingrese su nickname: ");
        memset(nickname, 0, MAX_NICK);
        fgets(nickname, MAX_NICK, stdin);

        if(strchr(nickname, '\n'))
            nickname[strcspn(nickname, "\n")] = '\0';
        else
            limpiarBuffer();

        ///extraer el nombre
        strncpy(nombre, nickname, MAX_NOMBRE - 1);
        nombre[MAX_NOMBRE - 1] = '\0';

        char* guion = strchr(nombre, '_');
        if(guion)
            *guion = '\0';
        identificacion=NICKNAME;
    }



    return identificacion;

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



void mostrarRanking(const char* nomArch)
{
    tArbol arbol;
    FILE* arch;

    if (ERROR_ARCH == abrirArchivo(&arch,nomArch, "rb"))
    {
        printf("No hay jugadores registrados.\n");
        return;
    }
    crearArbol(&arbol);

    tJugador jug;

    while(fread(&jug,sizeof(tJugador),1,arch))
        insertarEnArbol(&arbol,&jug,sizeof(tJugador),cmpPuntaje);



    printf("\n======================================================\n");
    printf("                         RANKING                      \n");
    printf("======================================================\n");
    recorrerDRI(&arbol, arch, mostrarJugadorDesdeRanking);
    printf("======================================================\n");
    vaciarArbol(&arbol);
    fclose(arch);
}


int cmpPuntaje(const void* elem,const void* elem2)
{
    tJugador* jug1=(tJugador*)elem;
    tJugador* jug2=(tJugador*)elem2;

    int cmp=jug1->totalPuntos-jug2->totalPuntos;

    if(cmp==0)
    {
        cmp=strcmp(jug1->nickName,jug2->nickName);
    }
    return cmp;
}


void mostrarJugadorDesdeRanking(const void* info,void* params)
{
    tJugador* jug=(tJugador*)info;
    printf("ID: %d\t| Nombre: %s\t|Nickname: %s\t| Puntos: %u\t| Partidas: %u\n", jug->idJugador,jug->nombre,jug->nickName,jug->totalPuntos,jug->partidasJugadas);
}


void iniciarPartida(tConfiguracion* config, tArbol *arbolJugadores)
{
    tPartida partida;
    tLista tablero;
    tLista bandidos;
    tCola historial;
    tCola bufferMovs;
    tCasilla casilla;
    tJugador jugador;
    tRegistroPartida reg;
    int identificacion;
    char nombre[MAX_NOMBRE];  ///deberia ser parte de una estructura jugador
    char nickname[MAX_NICK];
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

    if (crearTablero(NOM_ARCH_TABLERO, &tablero, config, &bandidos) != TODO_OK)
    {
        printf("Error al crear el tablero. Volviendo al menu...\n");
        vaciarLista(&tablero);
        vaciarCola(&historial);
        return;
    }

    identificacion=identificarJugador(nombre,nickname);
    if(buscarJugador(arbolJugadores, NOM_ARCH_JUGADORES, nickname, &jugador) == NO_ENCONTRADO)
    {
        jugador.idJugador = obtenerUltimoID(NOM_ARCH_JUGADORES) + 1;

        strcpy(jugador.nombre, nombre);
        strcpy(jugador.nickName,nickname);
        jugador.totalPuntos = 0;
        jugador.partidasJugadas = 0;

        altaJugador(arbolJugadores, NOM_ARCH_JUGADORES, &jugador);
        if(identificacion==NICKNAME)
        {
            printf("No hemos encontrado tu usuario, por lo tanto te dimos de alta!\n");
        }
        printf("Hola'%s' tu nickname es %s.\n", nombre,nickname);
        system("pause");
    }
    else
    {
        printf("Jugador %s encontrado.\n", jugador.nombre);
        strcpy(jugador.nombre, nombre);
        system("pause");
    }


    system("cls");
    printf("\n=========================================\n");
    printf("  Bienvenido a la caravana, %s!\n", nombre);
    printf("=========================================\n");
    printf("Vidas iniciales: %u | Puntos: %u\n\n", partida.cantVidas, partida.cantPuntos);
    system("pause");
    system("cls");


    printf("Vidas: %u | Puntos: %u\n", partida.cantVidas, partida.cantPuntos);
    dibujarTablero(&tablero,config->cantPosiciones);
    ///---------------BUCLE DEL JUEGO------------------------

    while(partida.cantVidas>0 && estado==JUEGO_CONTINUA)
    {

        if (estado == JUEGO_CONTINUA)
        {
            ///planifico el mov del jugador
            planificarMovimientoJugador(config,&bufferMovs,&historial,&partida);
            planificarMovimientosBandidos(&partida,&bufferMovs,&tablero,&bandidos);
            procesarTurno(&tablero,&bufferMovs,&partida);

            casilla.posicion=partida.posJugador;
            estado = manejarSituacionCasilla(&partida,&tablero,&bandidos,casilla,config);
        }

        printf("Vidas: %u | Puntos: %u\n", partida.cantVidas, partida.cantPuntos);
        dibujarTablero(&tablero,config->cantPosiciones);

    }
    ///------------------------------------------------------
    if (JUGADOR_GANO==estado)
    {
        printf("FELICITACIONES %s! Llegaste a la salida y ganaste.\n", nombre);
        printf("+10 puntos por ganar la partida!");
        partida.cantPuntos+=10;
    }
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

    actualizarJugador(arbolJugadores, NOM_ARCH_JUGADORES, &jugador);

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
        //partida->tormenta = 0 // esto lo hacemos en manejarSituacionCasilla

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

void planificarMovimientosBandidos(tPartida* partida, tCola* bufferMovs, tLista* tablero, tLista* bandidos)
{
    int dado;
    char movimiento;

    unsigned cantBandidosAMover, cantBandAMoverEstrateg;
    unsigned cantPosiciones = listaCantidadElementos(tablero);
    unsigned menorDistancia;
    unsigned idBandidoMenorDistancia;
//    unsigned pos = 0;
//    unsigned nuevaPosBandido;
//    unsigned posBandidoAMover;
//    char direccion;
//    tElemento elem;
    tContadorElementos cont = {0};
    tCasilla casillaPos;
    tCasilla* casilla=NULL;
    tMovimiento mov;
    tBandido bandido;

    if (!*bandidos)
        return; /// no hay bandidos en el tablero

    cantBandidosAMover = listaCantidadElementos(bandidos);
    printf("Cantidad de bandidos: %u\n", cantBandidosAMover);

    printf("\nPosiciones de los bandidos en el tablero:\n");
    recorrerDeIzqADer(bandidos, mostrarBandidos, NULL);

    /// Primero que busque al jugador, luego se vé implementar el resto...

    mov.elem.tipo = ASCII_BANDIDO;

    cantBandAMoverEstrateg = cantPosiciones * RELACION_MIN_BANDIDOS;
    if (cantBandAMoverEstrateg > cantBandidosAMover / 2)
        cantBandAMoverEstrateg = cantBandAMoverEstrateg * RELACION_MIN_BANDIDOS;

    /// si el jugador está en una tormenta, todos los bandidos contra el jugador
    cantBandAMoverEstrateg = partida->tormenta ? 0 : cantBandAMoverEstrateg;

    while (cantBandidosAMover > 0)
    {
        char pri = 1;
        unsigned idAnt;
        unsigned idBandidoAMover = 0;
        movimiento = 0;
        dado = tirarDado();

        recuperarUltElemOperadoEnLista(bandidos, &bandido, sizeof(bandido));
        idAnt = bandido.id;

        do
        {
            reiniciarContador(&cont);
            /// si el bandido no se movió en este turno
            if (bandido.movimientos == partida->movimientos)
            {
                unsigned posNuevaAvanzando = (bandido.posBandido + dado) % cantPosiciones;
                unsigned posNuevaRetrocediendo = (cantPosiciones + (bandido.posBandido - dado)) % cantPosiciones;
                unsigned distanciaAvanzando;
                unsigned distanciaRetrocediendo;

                posNuevaAvanzando == 0 ? posNuevaAvanzando = cantPosiciones : posNuevaAvanzando;
                posNuevaRetrocediendo == 0 ? posNuevaRetrocediendo = cantPosiciones : posNuevaRetrocediendo;

                distanciaAvanzando = MODULO((int)(posNuevaAvanzando - partida->posJugador));
                distanciaAvanzando > cantPosiciones / 2 ? distanciaAvanzando = cantPosiciones - distanciaAvanzando : distanciaAvanzando;
                distanciaRetrocediendo = MODULO((int)(posNuevaRetrocediendo - partida->posJugador));
                distanciaRetrocediendo > cantPosiciones / 2 ? distanciaRetrocediendo = cantPosiciones - distanciaRetrocediendo : distanciaRetrocediendo;

                if (cantBandAMoverEstrateg)
                {
                    /// si la distancia entre el bandido moviéndose y el jugador no es muy cercana
                    if (minimo(distanciaAvanzando,distanciaRetrocediendo) > cantPosiciones / 3)
                    {
                        /// se fija que hay en la casilla donde se ubica el bandido
                        casillaPos.posicion = bandido.posBandido;
                        casilla = buscarElementoLista(tablero, &casillaPos, compararPosicion);
                        recorrerDeIzqADer(&casilla->elementos, accionContarElementos, &cont);

                        /// si hay más de 2 bandidos en la misma posicion
                        /// o si el bandido se ubica en la salida, que se vaya, porque no le
                        /// afecta al jugador si llega a la salida
                        if (cont.cantBandido > 1 || cont.cantSalida == 1)
                        {
                            /// se fija hacia donde se mueve

                            /// se fija que no se mueva a la salida
                            if (posNuevaAvanzando != cantPosiciones)
                            {
                                reiniciarContador(&cont);
                                casillaPos.posicion = posNuevaAvanzando;
                                casilla = buscarElementoLista(tablero, &casillaPos, compararPosicion);
                                recorrerDeIzqADer(&casilla->elementos, accionContarElementos, &cont);

                                /// se fija si se mueve hacia adelante, que hay en esa casilla
                                if (cont.cantOasis || cont.cantVida)
                                {
                                    /// se mueve hacia adelante
                                    mov.direccion = FORWARD;
                                    mov.posOrigen = bandido.posBandido;
                                    mov.posFinal = posNuevaAvanzando;
                                    idBandidoAMover = bandido.id;
                                    movimiento = 1;
                                }
                                else
                                {
                                    /// se mueve hacia atras (si conviene)
                                    reiniciarContador(&cont);
                                    casillaPos.posicion = posNuevaAvanzando;
                                    casilla = buscarElementoLista(tablero, &casillaPos, compararPosicion);
                                    recorrerDeIzqADer(&casilla->elementos, accionContarElementos, &cont);
                                    if (cont.cantOasis || cont.cantVida)
                                    {
                                        mov.direccion = BACKWARD;
                                        mov.posOrigen = bandido.posBandido;
                                        mov.posFinal = posNuevaRetrocediendo;
                                        idBandidoAMover = bandido.id;
                                        movimiento = 1;
                                    }
                                }
                            }
                            else
                            {
                                /// se mueve hacia atras (si conviene)
                                reiniciarContador(&cont);
                                casillaPos.posicion = posNuevaAvanzando;
                                casilla = buscarElementoLista(tablero, &casillaPos, compararPosicion);
                                recorrerDeIzqADer(&casilla->elementos, accionContarElementos, &cont);
                                if (cont.cantOasis || cont.cantVida)
                                {
                                    mov.direccion = BACKWARD;
                                    mov.posOrigen = bandido.posBandido;
                                    mov.posFinal = posNuevaRetrocediendo;
                                    idBandidoAMover = bandido.id;
                                    movimiento = 1;
                                }
                            }

                        }
                        else
                        {
                            /// se fija si hay algun beneficio para el jugador en la casilla que está
                            if (cont.cantVida || cont.cantOasis)
                            {
                                /// se queda
                                mov.direccion = FORWARD;
                                mov.posOrigen = bandido.posBandido;
                                mov.posFinal = bandido.posBandido;
                                idBandidoAMover = bandido.id;
                                movimiento = 1;
                            }
                            /// si no hay nada, espera a tirar denuevo, si puede.
                        }

                    }

                }

                /// si no hubo movimiento estratégico
                if (!movimiento)
                {
                    if (pri || distanciaAvanzando < menorDistancia || distanciaRetrocediendo < menorDistancia)
                    {
                        if (distanciaAvanzando < distanciaRetrocediendo)
                        {
                            menorDistancia = distanciaAvanzando;
                            mov.direccion = FORWARD;
                            mov.posFinal = posNuevaAvanzando;
                        }
                        else
                        {
                            menorDistancia = distanciaRetrocediendo;
                            mov.direccion = BACKWARD;
                            mov.posFinal = posNuevaRetrocediendo;
                        }
                        mov.posOrigen = bandido.posBandido;
                        idBandidoMenorDistancia = bandido.id;
                        pri = 0;
                    }
                }
                else
                    cantBandAMoverEstrateg--;
            }

            recuperarUltElemOperadoEnLista(bandidos, &bandido, sizeof(bandido));
        } while (!idBandidoAMover && bandido.id != idAnt);


            ///actualizo la pos del bandido
            if (!idBandidoAMover)
                idBandidoAMover = idBandidoMenorDistancia;

            bandido.id = idBandidoAMover;
            if (buscarElementoEnLista(bandidos, &bandido, sizeof(bandido), compararBandidos, accionActualizarBandido, &mov.posFinal) != ENCONTRADO)
            {
                fprintf(stderr, "Error: No se encontro al bandido en la lista de bandidos\n");
                system("pause");
            }

            mov.elem.id = bandido.id;
//            mov.posOrigen = bandido.posBandido;
            mov.cantMovim = mov.posFinal == mov.posOrigen ? 0 : dado;

            ///inserto el movimiento del bandido en la cola
            ponerEnCola(bufferMovs, &mov, sizeof(tMovimiento));

//                posBandidoAMover = bandido.posBandido;
//                pri = 0;
//            pos++;
        cantBandidosAMover--;
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

    ///VIDA EXTRA
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
                sizeof(tBandido), compararPosBandidos);

            /// posicionar al jugador al inicio del tablero
            elem.tipo=ASCII_JUGADOR;
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

    ///TORMENTA
    //si el jugador no está en una tormenta, se fija si en la casilla hay una tormenta?
//    if (!partida->tormenta)
//    {
    partida->tormenta = FALSO;
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
//    }
//    else
//        partida->tormenta = 0;


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

void reiniciarContador(tContadorElementos* cont)
{
    cont->cantBandido = 0;
    cont->cantInicio = 0;
    cont->cantJugador = 0;
    cont->cantOasis = 0;
    cont->cantPremio = 0;
    cont->cantSalida = 0;
    cont->cantTormenta = 0;
    cont->cantVida = 0;
}

/// ésta podría ir en Herramientas.c
void limpiarBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    ///get char lee y elimina de a un caracter del buffer hasta que no hay mas datos (EOF)
}

