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
void ejecutarTurno(tPartida* partida, tCola* historial) {
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
}
