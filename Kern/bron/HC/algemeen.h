#ifndef ALGEMEEN_H
#define ALGEMEEN_H

#include <stddef.h>

typedef enum { onwaar = 0, waar = 1 } booleaan;

typedef booleaan (*vergelijk_opdracht)(const void* eerste, const void* tweede, size_t grootte);
typedef void (*verwijder_opdracht)(void* geheugen);

booleaan geheugen_vergelijker(const void* eerste, const void* tweede, size_t grootte);

#endif