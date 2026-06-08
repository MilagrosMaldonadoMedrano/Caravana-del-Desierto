#ifndef ARCHIVO_H_INCLUDED
#define ARCHIVO_H_INCLUDED

#include "main.h"
#include "Juego.h"
//#include "Cola.h"

#define TODO_OK 1
#define ERROR_ARCH -1
#define ERROR_ARCH_DEF -2
#define ERROR_CONFIG_CANT_POS -3
#define ERROR_CONFIG_VIDA -4
#define ERROR_CONFIG_BANDIDO1 -5
#define ERROR_CONFIG_BANDIDO2 -6
#define ERROR_CONFIG_TORMENTA -7

int abrirArchivo(FILE** arch,const char* nomArch,const char* modoApertura);
int configuracionPorDefecto(FILE** arch,const char* nomArch);
int cargarConfiguracion(const char* nomArch,tConfiguracion* config);
int validarConfiguracion(const tConfiguracion* config);
void informarErrorConfiguracion(int codError);

#endif // ARCHIVO_H_INCLUDED
