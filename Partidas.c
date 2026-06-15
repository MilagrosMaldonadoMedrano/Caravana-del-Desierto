#include "Partidas.h"
#include "Archivo.h"

int altaPartida(const char* nomArch, const tRegistroPartida* reg)
{
    FILE* pf;

    pf = fopen(nomArch, "ab");
    if(!pf)
        return ERROR_ARCH;

    fwrite(reg, sizeof(tRegistroPartida), 1, pf);

    fclose(pf);
    return TODO_OK;
}

int obtenerUltimoIdPartida(const char* nomArch)
{
    FILE* pf;
    tRegistroPartida reg;
    int ultimoId = 0;

    pf = fopen(nomArch, "rb");
    if(!pf)
        return 0;

    while(fread(&reg, sizeof(tRegistroPartida), 1, pf) == 1)
    {
        ultimoId = reg.idPartida;
    }

    fclose(pf);
    return ultimoId;
}

void mostrarPartida(const void* reg)
{
    tRegistroPartida* partida = (tRegistroPartida*)reg;

    printf("ID Partida: %d\t| ID Jugador: %d\t|"
           "Puntos: %u\t|Movimientos: %u\t|Resultado: %s\n",
           partida->idPartida,
           partida->idJugador,
           partida->puntos,
           partida->movimientos,
           partida->gano ? "GANO" : "PERDIO");
}

void mostrarArchivoPartidas(const char* nomArch)
{
    FILE* pf;
    tRegistroPartida reg;

    pf = fopen(nomArch, "rb");
    if(!pf)
    {
        printf("No hay partidas registradas.\n");
        return;
    }

    printf("\n===================================== PARTIDAS =====================================\n");
    while(fread(&reg, sizeof(tRegistroPartida), 1, pf) == 1)
        mostrarPartida(&reg);
    printf("====================================================================================\n");

    fclose(pf);
}
