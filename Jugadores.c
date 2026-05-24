#include "Jugadores.h"

int buscarJugador(const char* nomArch, const char* nombre, tJugador* jug)
{
    FILE *pf;
    tJugador aux;

    pf = fopen(nomArch, "rb");
    if(!pf)
        return ERROR_ARCH;

    while(fread(&aux, sizeof(tJugador), 1, pf) == 1)
    {
        if(strcmp(aux.nombre, nombre) == 0)
        {
            *jug = aux;
            fclose(pf);
            return ENCONTRADO;
        }
    }

    fclose(pf);
    return NO_ENCONTRADO;
}

int altaJugador(const char* nomArch, tJugador* jug)
{
    FILE *pf;

    pf= fopen(nomArch, "ab");
    if(!pf)
        return ERROR_ARCH;

    fwrite(jug, sizeof(tJugador), 1, pf);

    fclose(pf);
    return TODO_OK;
}

int actualizarJugador(const char* nomArch, tJugador* jug)
{
    FILE *pf;
    tJugador aux;

    pf = fopen(nomArch, "rb+");
    if(!pf)
        return ERROR_ARCH;

    while(fread(&aux, sizeof(tJugador), 1, pf) == 1)
    {
        if(aux.idJugador == jug->idJugador)
        {
            fseek(pf, -(long)sizeof(tJugador), SEEK_CUR);
            fwrite(jug, sizeof(tJugador),1, pf);
            fclose(pf);
            return TODO_OK;
        }
    }

    fclose(pf);

    return NO_ENCONTRADO;
}

void mostrarJugador(const void* j)
{
    tJugador* jug = (tJugador*)j;
    printf("ID: %d\t| Nombre: %s\t| Puntos: %u\t| Partidas: %u\n",
           jug->idJugador,
           jug->nombre,
           jug->totalPuntos,
           jug->partidasJugadas);
}

int obtenerUltimoID(const char* nomArch)
{
    FILE *pf;
    tJugador jug;
    int ultimoID = 0;

    pf = fopen(nomArch, "rb");
    if(!pf)
        return ERROR_ARCH;

    while(fread(&jug, sizeof(tJugador), 1, pf) == 1)
    {
        ultimoID = jug.idJugador;
    }

    fclose(pf);

    return ultimoID;
}


void mostrarArchivoJugadores(const char* nomArch)
{
    FILE *pf;
    tJugador jug;

    pf = fopen(nomArch, "rb");
    if(!pf)
        printf("No se puedo abrir el archvo: '%s\n'", nomArch);

    while(fread(&jug, sizeof(tJugador), 1, pf) == 1)
        mostrarJugador(&jug);

    fclose(pf);
}
