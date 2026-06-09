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

int buscarJugadorIndice(tArbol* arbolJugadores, const char* nomArch, const char* nickname, tJugador* jug) {
    FILE *pf;
    tIndice index;

    pf = fopen(nomArch, "rb");
    if (!pf) return ERROR_ARCH;

    index.clave = (void*)nickname;
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
int altaJugadorIndice(tArbol* arbolJugadores, const char* nomArch, tJugador* jug) {
    FILE *pf;
    tIndice index;
    char *clave;
    unsigned pos;

    pf = fopen(nomArch, "ab");
    if(!pf) return ERROR_ARCH;

    fseek(pf, 0, SEEK_END);
    pos = ftell(pf) / sizeof(tJugador);

    fwrite(jug, sizeof(tJugador), 1, pf);
    fclose(pf);

    clave = (char*) malloc(MAX_NICK);
    strcpy(clave, jug->nickName);


    index.clave = clave;
    index.tamClave = MAX_NICK;
    index.pos = pos;

    insertarEnArbol(arbolJugadores, &index, sizeof(tIndice), compararIndiceJugador);

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
int actualizarJugadorIndice(tArbol* arbolJugadores, const char* nomArch, tJugador* jug) {
    FILE *pf;
    tIndice index;
    index.clave = (void*)jug->nickName;
    index.tamClave=MAX_NICK;

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
/*
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
}*/
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

int compararJugadores(const void* j1, const void* j2)
{
    tJugador* jugador1 = (tJugador*)j1;
    tJugador* jugador2 = (tJugador*)j2;

    if(jugador2->totalPuntos != jugador1->totalPuntos)
    {
        return jugador2->totalPuntos - jugador1->totalPuntos;
    }

    if(jugador1->partidasJugadas != jugador2->partidasJugadas)
    {
        return jugador1->partidasJugadas - jugador2->partidasJugadas;
    }

    return strcmp(jugador1->nombre, jugador2->nombre);
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

