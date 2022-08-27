#pragma once
#include "aes_common.h"

void generate_random_block_values(uint8_t* block);
void check_cipher_symmetry(uint8_t* plain_text, uint8_t* deciphered_text);