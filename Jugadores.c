#include "Jugadores.h"
#include "Indice.h"

int buscarJugador(tArbol* arbolJugadores, const char* nomArch, const char* nickname, tJugador* jug) {
    FILE *pf;
    tIndice index;

    pf = fopen(nomArch, "rb");
    if (!pf) return ERROR_ARCH;

    strcpy(index.clave, nickname);
    index.tamClave = MAX_NICK;

    if (buscarEnArbol(arbolJugadores, &index, sizeof(tIndice), compararIndiceJugador)) {
        fseek(pf, index.pos * sizeof(tJugador), SEEK_SET);
        fread(jug, sizeof(tJugador), 1, pf);
        fclose(pf);
        return ENCONTRADO;
    }

    fclose(pf);
    return NO_ENCONTRADO;
}

int altaJugador(tArbol* arbolJugadores, const char* nomArch, tJugador* jug) {
    FILE *pf;
    tIndice index;
    unsigned pos;

    pf = fopen(nomArch, "ab");
    if(!pf) return ERROR_ARCH;

    fseek(pf, 0, SEEK_END);
    pos = ftell(pf) / sizeof(tJugador);

    fwrite(jug, sizeof(tJugador), 1, pf);
    fclose(pf);

    strcpy(index.clave, jug->nickName);
    index.tamClave = MAX_NICK;
    index.pos = pos;

    insertarEnArbol(arbolJugadores, &index, sizeof(tIndice), compararIndiceJugador);

    return TODO_OK;
}

int actualizarJugador(tArbol* arbolJugadores, const char* nomArch, tJugador* jug) {
    FILE *pf;
    tIndice index;

    strcpy(index.clave, jug->nickName);
    index.tamClave = MAX_NICK;

    pf = fopen(nomArch, "rb+");
    if (!pf) return ERROR_ARCH;

    if (buscarEnArbol(arbolJugadores, &index, sizeof(tIndice), compararIndiceJugador)) {
        fseek(pf, index.pos * sizeof(tJugador), SEEK_SET);
        fwrite(jug, sizeof(tJugador), 1, pf);

        fclose(pf);
        return TODO_OK;
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
    int cant = 0;

    pf = fopen(nomArch, "rb");
    if(!pf)
        return 0;

    fseek(pf, 0, SEEK_END);
    cant = ftell(pf) / sizeof(tJugador);

    fclose(pf);
    return cant;
}








void mostrarArchivoJugadores(const char* nomArch)
{
    FILE *pf;
    tJugador jug;

    pf = fopen(nomArch, "rb");
    if(!pf) {
        printf("No hay jugadores registrados.\n");
        return;
    }

    while(fread(&jug, sizeof(tJugador), 1, pf) == 1)
        mostrarJugador(&jug);

    fclose(pf);
}

