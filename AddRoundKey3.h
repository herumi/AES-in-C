#pragma once
#include <stdlib.h>
#include "KeyExpansion.h"
#include "SubBytes2.h"

void AddRoundKey_generic(struct state *s, const struct expKey *ekey, int i);
int isEqualMessage(const uint8_t *x, const uint8_t *y);
