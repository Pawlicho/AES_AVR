#pragma once

#include <stdint.h>
#include <avr/pgmspace.h>

#define AES_TYPE (128)
#define EXPANDED_KEY_SIZE (176)
#define ROUND_KEY_WORDS_NUMBER (4)
#define NUM_ROUNDS (10)

#define ALIGN __attribute__ ((aligned (256)))
