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


void iniciarPartida(tConfiguracion* config, tArbol *arbolJugadores)
{
    tPartida partida;
    tLista tablero;
    tLista bandidos;
    tCola historial;
    tCola bufferMovs;
    tCasilla casilla;
    tJugador jugador;
    tRegistroPartida reg;
    int identificacion;
    char nombre[MAX_NOMBRE];  ///deberia ser parte de una estructura jugador
    char nickname[MAX_NICK];
    int estado=JUEGO_CONTINUA;

    crearLista(&tablero);
    crearLista(&bandidos);
    crearCola(&historial);
    crearCola(&bufferMovs);

    partida.posJugador=1;
    partida.cantPuntos = 0;
    partida.movimientos = 0;
    partida.cantVidas = config->vidasInicio;
    partida.oasis = 0;
    partida.tormenta = 0;

    if (crearTablero(NOM_ARCH_TABLERO, &tablero, config, &bandidos) != TODO_OK)
    {
        printf("Error al crear el tablero. Volviendo al menu...\n");
        vaciarLista(&tablero);
        vaciarCola(&historial);
        return;
    }

    identificacion=identificarJugador(nombre,nickname);
    if(buscarJugador(arbolJugadores, NOM_ARCH_JUGADORES, nickname, &jugador) == NO_ENCONTRADO)
    {
        jugador.idJugador = obtenerUltimoID(NOM_ARCH_JUGADORES) + 1;

        strcpy(jugador.nombre, nombre);
        strcpy(jugador.nickName,nickname);
        jugador.totalPuntos = 0;
        jugador.partidasJugadas = 0;

        altaJugador(arbolJugadores, NOM_ARCH_JUGADORES, &jugador);
        if(identificacion==NICKNAME)
        {
            printf("No hemos encontrado tu usuario, por lo tanto te dimos de alta!\n");
        }
        printf("Hola '%s' tu nickname es %s.\n", nombre,nickname);
        system("pause");
    }
    else
    {
        printf("Jugador %s encontrado.\n", jugador.nombre);
        strcpy(jugador.nombre, nombre);
        system("pause");
    }


    system("cls");
    printf("\n=========================================\n");
    printf("  Bienvenido a la caravana, %s!\n", nombre);
    printf("=========================================\n");
    printf("Vidas iniciales: %u | Puntos: %u\n\n", partida.cantVidas, partida.cantPuntos);
    system("pause");
    system("cls");

    printf("%s: Vidas: %u | Puntos: %u\n", nickname, partida.cantVidas, partida.cantPuntos);
    dibujarTablero(&tablero,config->cantPosiciones);
    ///---------------BUCLE DEL JUEGO------------------------

    while(partida.cantVidas>0 && estado==JUEGO_CONTINUA)
    {

        if (estado == JUEGO_CONTINUA)
        {
            ///planifico el mov del jugador
            planificarMovimientoJugador(config,&bufferMovs,&historial,&partida);
            planificarMovimientosBandidos(&partida,&bufferMovs,&tablero,&bandidos);
            procesarTurno(&tablero,&bufferMovs,&partida);

            casilla.posicion=partida.posJugador;
            estado = manejarSituacionCasilla(&partida,&tablero,&bandidos,casilla,config);
        }

        printf("%s: Vidas: %u | Puntos: %u\n", nickname, partida.cantVidas, partida.cantPuntos);
        dibujarTablero(&tablero,config->cantPosiciones);

    }
    ///------------------------------------------------------
    if (JUGADOR_GANO==estado)
    {
        printf("FELICITACIONES %s! Llegaste a la salida y ganaste.\n", nombre);
        printf("+10 puntos por ganar la partida!");
        partida.cantPuntos+=10;
    }
    else if (JUGADOR_PERDIO==estado)
        printf("GAME OVER. Te quedaste sin vidas :(.\n");
    else
    {
        fprintf(stderr, "ERROR. Algo paso...\n");
        vaciarTablero(&tablero);
        vaciarLista(&bandidos);
        vaciarCola(&historial);
    }

    guardarMostrarHistorial(&historial, NOM_ARCH_MOVIMIENTOS);

    reg.idPartida = obtenerUltimoIdPartida(NOM_ARCH_PARTIDAS) + 1;
    reg.idJugador = jugador.idJugador;
    reg.puntos = partida.cantPuntos;
    reg.movimientos = partida.movimientos;
    reg.gano = (estado == JUGADOR_GANO);
    altaPartida(NOM_ARCH_PARTIDAS, &reg);

    jugador.totalPuntos += partida.cantPuntos;
    jugador.partidasJugadas++;

    actualizarJugador(arbolJugadores, NOM_ARCH_JUGADORES, &jugador);

    vaciarTablero(&tablero);
    vaciarLista(&bandidos);
    vaciarCola(&historial);
    vaciarCola(&bufferMovs);
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
