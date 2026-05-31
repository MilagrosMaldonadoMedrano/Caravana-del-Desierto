#ifndef HERRAMIENTAS_H_INCLUDED
#define HERRAMIENTAS_H_INCLUDED

#include "main.h"

#define MODULO(X)((X) > 0 ? (X) : (-X))
#define DIVISOR_DECIMAL 10 //para evitar el uso de magic numbers
#define COD_ASCII_O_TILDE 162
#define COD_ASCII_ENIE 164

int cantDigitosUnsigned(unsigned num);

#endif // HERRAMIENTAS_H_INCLUDED
