#pragma once

#define CEIL_DIV(a, b) \
    ({ __typeof__(a) _a = (a); __typeof__(b) _b = (b); (_a + _b - 1) / _b; })
