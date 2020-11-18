#include "algemeen.h"

#include <stdio.h>
#include <string.h>

inline booleaan geheugen_vergelijker(const void* eerste, const void* tweede, size_t grootte) {
	return memcmp(eerste, tweede, grootte) == 0;
}

void booleaan_afdrukker(booleaan* booleaan) { puts(booleaan ? "waar" : "onwaar"); }
void getal_afdrukker(int* getal) { printf("%d", *getal); }
void kommagetal_afdrukker(float* getal) { printf("%f", getal); }
