#pragma once
#include <stdint.h>


#define AES_TYPE (128)
#define EXPANDED_KEY_SIZE (176)
#define ROUND_KEY_WORDS_NUMBER (4)
#define NUM_ROUNDS (10)

uint8_t* generate_random_block();