#include "Herramientas.h"

int cantDigitosUnsigned(unsigned num)
{
    int cantDigitos = 0;

    do
    {
        num /= DIVISOR_DECIMAL;
        cantDigitos++;
    }
    while (num > 0);
    return cantDigitos;
}

void limpiarBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    ///get char lee y elimina de a un caracter del buffer hasta que no hay mas datos (EOF)
}

int nombreVal(char* nombre)
{
    char* ini = nombre;
    char caracterVal = 0;

    while(*nombre && !caracterVal)
    {
        if ((*nombre) == ' ')
            nombre++;
        else
            caracterVal = 1;
    }

    nombre = ini;

    return caracterVal;
}
