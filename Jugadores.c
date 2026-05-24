#include "Jugadores.h"

int buscarJugador(const char* nomArch, const char* nombre, tJugador* jug)
{

}

int altaJugador(const char* nomArch, tJugador* jug)
{
    FILE *pf;

    pf= fopen(nomArch, "ab");

    if(!pf)
        return ERROR_ARCH;

    fwrite(jug, sizeof(tJugador), 1, pf);

    fclose(pf);

    return TODO_OK
}

int actualizarJugador(const char* nomArch, tJugador* jug)
{

}

void mostrarJugador(const void* j)
{

}
