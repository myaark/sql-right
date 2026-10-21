#pragma once
#include <stddef.h>
#include "status.h"

status_t checked_add(size_t a, size_t b, size_t *out);
status_t checked_mul(size_t a, size_t b, size_t *out);