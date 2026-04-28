#include "Herramientas.h"

int cantDigitosUnsigned(unsigned num)
{
    int cantDigitos = 0;

    do
    {
        num /= DIVISOR_DECIMAL;
        cantDigitos++;
    } while (num > 0);
    return cantDigitos;
}
