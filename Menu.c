#include "Turno.h"
#include "Menu.h"
#include "Lista.h"
#include "Archivo.h"
#include "Indice.h"

int identificarJugador(char* nombre, char* nickname)
{
    int opcion;
    int identificacion;
    printf("\n============================\n");
    printf(  "      ACCESO AL JUEGO       \n");
    printf(  "============================\n");

    do
    {
        printf("1. Soy nuevo\n");
        printf("2. Ya tengo nickname\n");
        printf("Opcion: ");
        scanf("%d", &opcion);
    }
    while(opcion!=1 && opcion!=2);
    limpiarBuffer();

    memset(nickname, 0, MAX_NICK);

    if(opcion==1)
    {
        int numero=rand()%999;
        char cadena[5];
        ingresarNombreVal("Ingrese su nombre: ", nombre);
        strcpy(nickname,nombre);
        strcat(nickname,"_");
        sprintf(cadena, "%d", numero);
        strcat(nickname,cadena);

        printf("\n====================================\n");
        printf(  "        REGISTRO EXITOSO!           \n");
        printf(  "====================================\n");
        printf(  "  Nickname: %-22s\n", nickname);
        printf(  "   Guardalo para la proxima vez.    \n");
        printf(  "====================================\n");

        identificacion=NOMBRE;
    }
    else
    {
        ingresarNombreVal("Ingrese su nickname: ", nickname);

        ///extraer el nombre
        strncpy(nombre, nickname, MAX_NOMBRE - 1);
        nombre[MAX_NOMBRE - 1] = '\0';

        char* guion = strchr(nombre, '_');
        if(guion)
            *guion = '\0';
        identificacion=NICKNAME;
    }



    return identificacion;

}

// Muestra el menu y devuelve la opcion elegida
int mostrarMenu()
{
    int opcion;

    do
    {
        printf("\n==============================\n");
        printf("     CARAVANA DEL DESIERTO    \n");
        printf("==============================\n");
        printf("  %d. Jugar nueva partida     \n", OPCION_JUGAR);
        printf("  %d. Ver ranking             \n", OPCION_RANKING);
        printf("  %d. Salir                   \n", OPCION_SALIR);
        printf("==============================\n");
        printf("Opcion: ");

        if(scanf("%d", &opcion) != 1)
        {
            printf("Entrada invalida.\n");
            limpiarBuffer();
            opcion = -1;
            continue;
        }

        limpiarBuffer();

        if(opcion != OPCION_JUGAR &&
                opcion != OPCION_RANKING &&
                opcion != OPCION_SALIR)
        {
            printf("Opcion invalida.\n");
        }

    }
    while(opcion != OPCION_JUGAR &&
            opcion != OPCION_RANKING &&
            opcion != OPCION_SALIR);

    return opcion;
}


void mostrarRanking(const char* nomArch)
{
    tArbol arbol;
    FILE* arch;

    if (ERROR_ARCH == abrirArchivo(&arch,nomArch, "rb"))
    {
        printf("No hay jugadores registrados.\n");
        return;
    }
    crearArbol(&arbol);

    tJugador jug;

    while(fread(&jug,sizeof(tJugador),1,arch))
        insertarEnArbol(&arbol,&jug,sizeof(tJugador),cmpPuntaje);



    printf("\n======================================================\n");
    printf("                         RANKING                      \n");
    printf("======================================================\n");
    recorrerDRI(&arbol, arch, mostrarJugadorDesdeRanking);
    printf("======================================================\n");
    vaciarArbol(&arbol);
    fclose(arch);
}


int cmpPuntaje(const void* elem,const void* elem2)
{
    tJugador* jug1=(tJugador*)elem;
    tJugador* jug2=(tJugador*)elem2;

    int cmp=jug1->totalPuntos-jug2->totalPuntos;

    if(cmp==0)
    {
        cmp=strcmp(jug1->nickName,jug2->nickName);
    }
    return cmp;
}


void mostrarJugadorDesdeRanking(const void* info,void* params)
{
    tJugador* jug=(tJugador*)info;
    printf("ID: %d\t| Nombre: %s\t|Nickname: %s\t| Puntos: %u\t| Partidas: %u\n", jug->idJugador,jug->nombre,jug->nickName,jug->totalPuntos,jug->partidasJugadas);
}


char pedirDireccion()
{
    int opcion;

    do
    {
        printf("\n  %d. Avanzar\n", OPCION_AVANZAR);
        printf("  %d. Retroceder\n", OPCION_RETROCEDER);
        printf("Opcion: ");

        if (scanf(" %d", &opcion) != 1)
        {
            limpiarBuffer();
            opcion = -1;     // fuerza reintentar
            continue;
        }

        if (opcion != OPCION_AVANZAR && opcion != OPCION_RETROCEDER)
            printf("Opcion invalida. Intenta nuevamente.\n");

    }
    while (opcion != OPCION_AVANZAR && opcion != OPCION_RETROCEDER);

    return (opcion == OPCION_AVANZAR) ? FORWARD : BACKWARD;
}

void ingresarNombreVal(const char* mensaje, char* nombre)
{
    do
    {
        printf("%s", mensaje);
        fflush(stdin);
        fgets(nombre, MAX_NICK, stdin);

        if(strchr(nombre, '\n'))
            nombre[strcspn(nombre, "\n")] = '\0';
        else
            limpiarBuffer();

    }
    while (!nombreVal(nombre));
}
