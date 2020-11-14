#include "algemeen.h"

#include <string.h>

inline booleaan geheugen_vergelijker(const void* eerste, const void* tweede, size_t grootte) { return memcmp(eerste, tweede, grootte) == 0; }
