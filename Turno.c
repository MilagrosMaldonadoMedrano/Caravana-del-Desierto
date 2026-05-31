#include "Turno.h"
#include "Menu.h"

// Aplica el efecto de la casilla sobre la partida
void aplicarEfectoCasilla(char casilla, tPartida* partida) {
    switch (casilla) {
        case ASCII_PREMIO:
            partida->cantPuntos++;
            printf("Encontraste un premio! +1 punto. Puntos: %u\n", partida->cantPuntos);
            break;

        case ASCII_VIDA_EXTRA:
            partida->cantVidas++;
            printf("Encontraste una vida extra! Vidas: %u\n", partida->cantVidas);
            break;

        case ASCII_OASIS:
            partida->oasis = 1;
            printf("Llegaste a un oasis! Estas protegido el proximo turno.\n");
            break;

        case ASCII_TORMENTA:
            if (partida->oasis)
            {
                printf("Caiste en una Tormenta de arena! Pero el oasis te protege.\n");
                partida->oasis = 0; // se consume la proteccion
            }
            else
            {
                partida->tormenta = 1;
                printf("Caiste en una Tormenta de arena! Perdes el proximo turno.\n");
            }
            break;

        case ASCII_BANDIDO:
            ///
            /// LOGICA DEL BANDIDO
            ///
            break;

        case ASCII_POS_VACIA:
        case ASCII_INICIO:
            // sin efecto
            break;
    }
}

// Devuelve JUGADOR_GANO, JUGADOR_PERDIO o JUEGO_CONTINUA
int verificarFinPartida(char casilla, const tPartida* partida)
{
    if (casilla == ASCII_SALIDA)
        return JUGADOR_GANO;

    if (partida->cantVidas == 0)
        return JUGADOR_PERDIO;

    return JUEGO_CONTINUA;
}

// Ejecuta un turno completo del jugador
void ejecutarTurno(tLista* tablero, tPartida* partida, tCola* historial) {
    int dado;
    char direccion;
    char casillaActual;

    // turno bloqueado por tormenta
    if (partida->tormenta) {
        printf("\nEstas atrapado en una tormenta. Perdes este turno.\n");
        partida->tormenta = 0;
        return;
    }

    // expira el oasis si no se usó
    if (partida->oasis) {
        printf("La proteccion del oasis expiro.\n");
        partida->oasis = 0;
    }

    dado = tirarDado();
    printf("\nTiraste el dado: %d\n", dado);

    direccion = pedirDireccion();

    /// queda pendiente obtener la casilla actual real y mover al jugador
    casillaActual = ASCII_POS_VACIA; // temporal

    registrarMovimiento(historial, direccion, dado);

    printf("Vidas: %u | Puntos: %u\n", partida->cantVidas, partida->cantPuntos);

    aplicarEfectoCasilla(casillaActual, partida);

    system("pause");
    //system("cls");
}

/// ejecuta el turno de los bandidos (lógica)
void ejecutarMovimientoBandido(tLista* tablero, tLista* bandidos, unsigned posJugador, unsigned cantPosiciones)
{
    tBandido bandido;
    tCasilla casilla;
    tElemento elem;
    //tContadorElementos cont;
    int dado;
    unsigned pos = 0;
    unsigned posBandidoAMover;
    unsigned menorDistancia;
    unsigned nuevaPosBandido;
    char direccion;
    char pri = 1;

    if (!*bandidos)
        return; /// no hay bandidos en el tablero

//    printf("\nPosiciones de los bandidos en el tablero:\n");
//    recorrerDeIzqADer(bandidos, mostrarBandidos, NULL);

    dado = tirarDado();

    /// deberia recorrer todo el tablero y guardar las posiciones de los elementos del tablero (jugador, vidas, etc)
    /// para la lógica de analizar el proximo movimiento del bandido.

    //while (actual != *bandidos && recuperarElementoXPosLista(bandidos, &bandido, sizeof(bandido), pos) == TODO_OK)
    while (recuperarElementoXPosLista(bandidos, &bandido, sizeof(bandido), pos) == ENCONTRADO)
    {

        unsigned posNuevaAvanzando = (bandido.posBandido + dado) % cantPosiciones;
        unsigned posNuevaRetrocediendo = (cantPosiciones + (bandido.posBandido - dado)) % cantPosiciones;
        unsigned distanciaAvanzando;
        unsigned distanciaRetrocediendo;

        posNuevaAvanzando == 0 ? posNuevaAvanzando = cantPosiciones : posNuevaAvanzando;

        distanciaAvanzando = (cantPosiciones + (posNuevaAvanzando - posJugador)) % cantPosiciones;
        distanciaRetrocediendo = (cantPosiciones + (posNuevaRetrocediendo - posJugador)) % cantPosiciones;

        if (pri || distanciaAvanzando < menorDistancia || distanciaRetrocediendo < menorDistancia)
        {
            if (distanciaAvanzando < distanciaRetrocediendo)
            {
                direccion = FORWARD;
                menorDistancia = distanciaAvanzando;
                nuevaPosBandido = posNuevaAvanzando;
            }
            else
            {
                direccion = BACKWARD;
                menorDistancia = distanciaRetrocediendo;
                nuevaPosBandido = posNuevaRetrocediendo;
            }

            posBandidoAMover = bandido.posBandido;
            pri = 0;
        }
        pos++;
    }

    /// falta verificar si es conveniente realizar este movimiento, u otro

    elem.tipo = ASCII_BANDIDO;
    casilla.posicion = posBandidoAMover;

    /// remover el bandido de la casilla anterior
    eliminarElementoEnCasilla(tablero,casilla,elem);

    /// remover el bandido de la lista de bandidos, para actualizar la pos y volverlo a guardar
    bandido.posBandido = posBandidoAMover;
    if (eliminarListaDesordenadaPorClaveSinDup(bandidos, &bandido, sizeof(bandido), compararBandidos) != ENCONTRADO)
    {
        fprintf(stderr, "\nERROR. No encontró al bandido en la lista de bandidos\n");
        return; /// return ERROR_NO_ENCONTRADO (esto no tiene que pasar)!
    }

    /// añadir al bandido en la casilla nueva
    casilla.posicion = nuevaPosBandido;
    if (insertarElementoSeguro(tablero, casilla, elem, NULL) != TODO_OK)
    {
        fprintf(stderr, "\nERROR. No pudo insertar el bandido en la casilla que se iba a mover\n");
        return; /// return ERROR_MEM;
    }

    /// añadir al bandido con la nueva pos en la lista de bandidos
    bandido.posBandido = nuevaPosBandido;
    insertarAlFinal(bandidos, &bandido, sizeof(bandido));

    /// Si no captura al jugador, analizar estos casos y su orden de prioridad:

    /// 1. Ocupar una casilla con beneficios para el jugador (vida extra, oasis, punto)
    /// 2. Avanzar hacia el jugador en el caso de no quedar demasiado lejos
    /// 3. Si hay mas de un bandido en la misma casilla, separarse

    printf("\nLa IA tiro el dado: %d\nEl bandido se movio desde la casilla %u hasta la %u\n", dado, posBandidoAMover, nuevaPosBandido); /// modificar luego
    if (direccion == FORWARD)
        printf("Se movio hacia adelante\n");
    else
        printf("Se movio hacia atras\n");

}





