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
    char buffer[MAX_NICK];
    char *clave;

    pf = fopen(nomArchIndice, "rb");
    if (!pf) return ERROR_ARCH;

    while (fread(buffer, sizeof(char), MAX_NICK, pf) == MAX_NICK) {
        fread(&pos, sizeof(unsigned), 1, pf);

        clave = (char*) malloc(MAX_NICK);
        strcpy(clave, buffer);

        index.clave = clave;
        index.tamClave = MAX_NICK;
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

        clave = (char*) malloc(MAX_NICK);
        strcpy(clave, jug.nickName);

        index.clave = clave;
        index.tamClave = MAX_NICK;
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





///indice compuesto del ranking
int indexarArchivoJugadoresPorPuntaje(const char* nomArchJugadores, tArbol* arbol)
{

    FILE* pf;
    tJugador jug;
    tIndiceCompuesto index;
    unsigned pos = 0;
    unsigned *puntos;
    char *nombre;

    pf = fopen(nomArchJugadores, "rb");
    if (!pf)
        return ERROR_ARCH;

    while (fread(&jug, sizeof(tJugador), 1, pf))
    {



        puntos = malloc(sizeof(unsigned));
        if (!puntos)
        {
            fclose(pf);
            return ERROR_MEM;
        }
        *puntos = jug.totalPuntos;


        nombre =malloc(MAX_NOMBRE);

        if(!nombre)
        {
            fclose(pf);
            free(puntos);
            return ERROR_MEM;
        }

        strcpy(nombre, jug.nombre);



        index.clave = puntos;
        index.tamClave = sizeof(unsigned);
        index.pos = pos;
        index.clave2 = nombre;
        index.tamClave2= MAX_NOMBRE;


        insertarEnArbol(arbol, &index, sizeof(tIndiceCompuesto), compararPorPuntaje);
        pos++;
    }

    fclose(pf);
    return TODO_OK;
}
int compararPorPuntaje(const void *a, const void *b)
{
    const tIndiceCompuesto *ia = (const tIndiceCompuesto*)a;
    const tIndiceCompuesto *ib = (const tIndiceCompuesto*)b;

    unsigned *puntajeA = (unsigned*) ia->clave;
    unsigned *puntajeB = (unsigned*) ib->clave;

    int cmp = *puntajeA - *puntajeB;

    if(cmp == 0)
    {
        const char *nombreA = (const char*) ia->clave2;
        const char *nombreB = (const char*) ib->clave2;
        cmp = strcmp(nombreA, nombreB);
    }

    return cmp;
}

void mostrarJugadorDesdeRanking(const void* info, void* params)
{
    FILE* arch = (FILE*)params;
    const tIndiceCompuesto* idx = (const tIndiceCompuesto*)info;
    tJugador jug;


    fseek(arch, idx->pos * sizeof(tJugador), SEEK_SET);
    fread(&jug, sizeof(tJugador), 1, arch);

    printf("ID: %d\t| Nombre: %s\t| Puntos: %u\t| Partidas: %u\n", jug.idJugador,jug.nombre,jug.totalPuntos,jug.partidasJugadas);
}



