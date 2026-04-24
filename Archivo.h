#ifndef ARCHIVO_H_INCLUDED
#define ARCHIVO_H_INCLUDED
#include<stdio.h>
#include"Juego.h"

#define TODO_OK 1
#define ERROR_ARCH 2

int abrirArchivo(FILE** arch,const char* nomArch,const char* modoApertura);
void configuracionPorDefecto(FILE** arch,const char* nomArch);
void cargarConfiguracion(const char* nomArch,tConfiguracion* config);

#endif // ARCHIVO_H_INCLUDED
