#include "Herramientas.h"

int cantDigitosUnsigned(unsigned num)
{
    int cantDigitos = 0;

    do
    {
        num /= DIEZ;
        cantDigitos++;
    } while (num > 0);
    return cantDigitos;
}
