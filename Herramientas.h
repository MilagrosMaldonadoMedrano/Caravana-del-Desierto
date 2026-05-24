#ifndef HERRAMIENTAS_H_INCLUDED
#define HERRAMIENTAS_H_INCLUDED

#include "main.h"

#define MODULO(X) ((X) > 0 ? (X) : (-X))
#define DIVISOR_DECIMAL 10 //para evitar el uso de magic numbers

int cantDigitosUnsigned(unsigned num);

#endif // HERRAMIENTAS_H_INCLUDED
