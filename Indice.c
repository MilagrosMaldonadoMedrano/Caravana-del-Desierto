#include "indice.h"
#include "Archivo.h"

int compararIndiceJugador(const void *a, const void *b)
{
    tIndice *ia = (tIndice*)a;
    tIndice *ib = (tIndice*)b;
    return strcmp(ia->clave, ib->clave);
}

/** Cargar el indice a un arbol */
int cargarIndiceJugadores(const char* nomArchIndice, tArbol* arbolJugadores)
{
    FILE* pf;
    tIndice index;

    pf = fopen(nomArchIndice, "rb");
    if (!pf) return ERROR_ARCH;

    while (fread(&index, sizeof(tIndice), 1, pf) == 1)
    {
        insertarEnArbol(arbolJugadores, &index, sizeof(tIndice), compararIndiceJugador);
    }

    fclose(pf);
    return TODO_OK;
}

/** Crear el indice */
int indexarArchivoJugadores(const char* nomArchJugadores, tArbol* arbolJugadores)
{
    FILE* pf;
    tJugador jug;
    tIndice index;
    unsigned pos = 0;

    pf = fopen(nomArchJugadores, "a+b"); // a+b para que cree el archivo si no lo encuentra
    if (!pf) return ERROR_ARCH;

    rewind(pf); // rewind para mover el puntero al inicio del archivo

    while (fread(&jug, sizeof(tJugador), 1, pf) == 1)
    {
        strcpy(index.clave, jug.nickName);
        index.tamClave = MAX_NICK;
        index.pos = pos;

        insertarEnArbol(arbolJugadores, &index, sizeof(tIndice), compararIndiceJugador);
        pos++;
    }

    fclose(pf);
    return TODO_OK;
}

void guardarNodoIndiceJugador(const void *info, void *params)
{
    tIndice *index = (tIndice*) info;
    FILE *pf = (FILE*) params;

    fwrite(index, sizeof(tIndice), 1, pf);
}

int guardarIndiceJugadores(const char* nomArchIndice, const tArbol* arbolJugadores)
{
    FILE *pf = fopen(nomArchIndice, "wb");
    if (!pf) return ERROR_ARCH;

    recorrerInOrdenParam(arbolJugadores, pf, guardarNodoIndiceJugador);

    fclose(pf);
    return TODO_OK;
}

void mostrarNodoIndiceJugador(const void *info)
{
    tIndice *index = (tIndice*)info;
    printf("Jugador: %s | Posicion: %u\n", index->clave, index->pos);
}

void mostrarIndiceJugadores(const tArbol* arbolJugadores)
{
    printf("----------------------------------------\n");
    printf("\nIndice de Jugadores\n");
    recorrerInOrden(arbolJugadores, mostrarNodoIndiceJugador);
    printf("----------------------------------------\n");
}

