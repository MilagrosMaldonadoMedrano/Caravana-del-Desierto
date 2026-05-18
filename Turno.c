#include "Turno.h"

// Muestra el menu de direccion y devuelve FORWARD o BACKWARD
char pedirDireccion() {
    int opcion;

    do {
        printf("\n  %d. Avanzar\n", OPCION_AVANZAR);
        printf("  %d. Retroceder\n", OPCION_RETROCEDER);
        printf("Opcion: ");
        scanf(" %d", &opcion);

        if (opcion != OPCION_AVANZAR && opcion != OPCION_RETROCEDER)
            printf("Opcion invalida. Intenta nuevamente.\n");

    } while (opcion != OPCION_AVANZAR && opcion != OPCION_RETROCEDER);

    return (opcion == OPCION_AVANZAR) ? FORWARD : BACKWARD;
}

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
    system("cls");
}

/// ejecuta el turno de los bandidos (lógica)
void ejecutarMovimientoBandido(tLista* tablero, tLista* bandidos, unsigned cantPosiciones)
{
    //tNodoD* actual;
    tBandido bandido;
    tCasilla* casilla;
    tElemento elem;
    int dado;
    unsigned pos = 1;
    unsigned posBandidoAMover;
    unsigned menorDistancia;
    unsigned nuevaPosBandido;
    char direccion;
    char pri = 1;

    if (!bandidos)
        return;

    crearCasilla(casilla);
    casillaJugador->posicion = 1; // para no meterle basura, pero esto no sirve de nada
    elem.tipo = ASCII_JUGADOR;
    insertarAlComienzo(&casilla->elementos, &elem, sizeof(elem));
    /// creo un tCasilla y le guardo el ASCII_JUGADOR, para recuperar la pos del jugador
    casillaJugador = buscarElementoLista(tablero, *casilla, compararElem); /// recupera la pos del jugador

    dado = tirarDado();

    //actual = (*bandidos)->sig;

    //while (actual != *bandidos && recuperarElementoXPosLista(bandidos, &bandido, sizeof(bandido), pos) == TODO_OK)
    while (recuperarElementoXPosLista(bandidos, &bandido, sizeof(bandido), pos) == TODO_OK)
    {
        unsigned posNuevaAvanzando = (bandido.posBandido + dado) % cantPosiciones;
        unsigned posNuevaRetrocediendo = bandido.posBandido + cantPosiciones - dado) % cantPosiciones;

        unsigned distanciaAvanzando = (cantPosiciones + posNuevaAvanzando - casillaJugador->posicion) % cantPosiciones;
        unsigned distanciaRetrocediendo = (cantPosiciones + posNuevaRetrocediendo - casillaJugador->posicion) % cantPosiciones;

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

    }

    /// falta verificar si es conveniente realizar este movimiento, u otro

    elem.tipo = ASCII_BANDIDO;
    casilla->posicion = posBandidoAMover;
    /// remover el bandido de la casilla anterior: Hace falta un "eliminarElementoEnCasilla"

    casilla->posicion = nuevaPosBandido;
    agregarElementoEnCasilla(tablero, *casillaBandido, elem); /// añado el bandido a la casilla nueva

    /// Si no captura al jugador, analizar estos casos y su orden de prioridad:

    /// 1. Ocupar una casilla con beneficios para el jugador (vida extra, oasis, punto)
    /// 2. Avanzar hacia el jugador en el caso de no quedar demasiado lejos
    /// 3. Si hay mas de un bandido en la misma casilla, separarse

    printf("\nLa IA tiro el dado: %d, y desplazo el bandido de la casilla %u", dado, posBandidoAMover); /// modificar luego
    if (direccion == FORWARD)
        printf(" hacia adelante\n");
    else
        printf(" hacia atras\n");
}
