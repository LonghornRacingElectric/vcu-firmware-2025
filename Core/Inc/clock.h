#ifndef LONGHORN_LIBRARY_CLOCK_H
#define LONGHORN_LIBRARY_CLOCK_H

#include <stdint.h>
#include "main.h"

void clock_init();
float clock_getDeltaTime();
float clock_getTime();

#endif //LONGHORN_LIBRARY_CLOCK_H
