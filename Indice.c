#include "indice.h"

int compararIndiceJugador(const void *a, const void *b) {
    tIndice *ia = (tIndice*)a;
    tIndice *ib = (tIndice*)b;
    return strcmp((char*)ia->clave, (char*)ib->clave);
}

/** Cargar el indice a un arbol */
int cargarIndiceJugadores(const char* nomArchIndice, tArbol* arbolJugadores) {
    FILE* pf;
    tIndice index;
    unsigned pos;
    char buffer[MAX_NOMBRE];
    char *clave;

    pf = fopen(nomArchIndice, "rb");
    if (!pf) return ERROR_ARCH;

    while (fread(buffer, sizeof(char), MAX_NOMBRE, pf) == MAX_NOMBRE) {
        fread(&pos, sizeof(unsigned), 1, pf);

        clave = (char*) malloc(MAX_NOMBRE);
        strcpy(clave, buffer);

        index.clave = clave;
        index.tamClave = MAX_NOMBRE;
        index.pos = pos;

        insertarEnArbol(arbolJugadores, &index, sizeof(tIndice), compararIndiceJugador);
    }

    fclose(pf);
    return TODO_OK;
}

/** Crear el indice */
int indexarArchivoJugadores(const char* nomArchJugadores, tArbol* arbolJugadores) {
    FILE* pf;
    tJugador jug;
    tIndice index;
    unsigned pos = 0;
    char *clave;

    pf = fopen(nomArchJugadores, "rb");
    if (!pf) return ERROR_ARCH;

    while (fread(&jug, sizeof(tJugador), 1, pf) == 1) {
        clave = (char*) malloc(MAX_NOMBRE);
        strcpy(clave, jug.nombre);

        index.clave = clave;
        index.tamClave = MAX_NOMBRE;
        index.pos = pos;

        insertarEnArbol(arbolJugadores, &index, sizeof(tIndice), compararIndiceJugador);
        pos++;
    }

    fclose(pf);
    return TODO_OK;
}

void guardarNodoIndiceJugador(const void *info, void *params) {
    tIndice *index = (tIndice*) info;
    FILE *pf = (FILE*) params;

    fwrite(index->clave, index->tamClave, 1, pf);
    fwrite(&index->pos, sizeof(unsigned), 1, pf);
}

int guardarIndiceJugadores(const char* nomArchIndice, const tArbol* arbolJugadores) {
    FILE *pf = fopen(nomArchIndice, "wb");
    if (!pf) return ERROR_ARCH;

    recorrerInOrdenParam(arbolJugadores, pf, guardarNodoIndiceJugador);

    fclose(pf);
    return TODO_OK;
}

void mostrarNodoIndiceJugador(const void *info)
{
    tIndice *index = (tIndice*)info;
    printf("Jugador: %s | Posicion: %u\n", (char*)index->clave, index->pos);
}

void mostrarIndiceJugadores(const tArbol* arbolJugadores)
{
    printf("----------------------------------------\n");
    printf("\nIndice de Jugadores\n");
    recorrerInOrden(arbolJugadores, mostrarNodoIndiceJugador);
    printf("----------------------------------------\n");
}
