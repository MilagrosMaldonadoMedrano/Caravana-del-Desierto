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
int buscarJugadorIndice(tArbol* arbolJugadores, const char* nomArch, const char* nombre, tJugador* jug) {
    FILE *pf;
    tIndice index;

    pf = fopen(nomArch, "rb");
    if (!pf) return ERROR_ARCH;

    index.clave = (void*)nombre;

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

    clave = (char*) malloc(MAX_NOMBRE);
    strcpy(clave, jug->nombre);

    index.clave = clave;
    index.tamClave = MAX_NOMBRE;
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
    index.clave = (void*)jug->nombre;

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

int cargarJugadores(const char* nomArch, tJugador* vec)
{
    FILE *pf;
    tJugador* pj = vec;
    int cantJugadores = 0;

    pf = fopen(nomArch, "rb");
    if(!pf)
        return ERROR_ARCH;

    while(fread(pj, sizeof(tJugador), 1, pf) == 1)
    {
        pj++;
        cantJugadores++;
    }

    fclose(pf);

    return cantJugadores;
}

void ordenarJugadores(tJugador* vec, int cantJugadores)
{
    qsort(vec, cantJugadores, sizeof(tJugador), compararJugadores);
}

void mostrarVectorJugadores(tJugador* vec, int cantJugadores)
{
    tJugador* pj;
    tJugador* ult = vec + cantJugadores - 1;

    for(pj=vec; pj<=ult; pj++)
    {
        mostrarJugador(pj);
    }
}
