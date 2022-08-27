#include "aes_test_vector.h"
#include <stdlib.h>
#include <assert.h>

void generate_random_block_values(uint8_t* block)
{
    for(uint8_t i = 0; i < BLOCK_SIZE; i++)
    {
        uint8_t random_value = rand();
        block[i] = random_value;
    }
}

void check_cipher_symmetry(uint8_t* plain_text, uint8_t* deciphered_text) 
{
    for(uint8_t value_idx = 0; value_idx < BLOCK_SIZE; value_idx++)
    {
        assert(plain_text[value_idx] == deciphered_text[value_idx]);
    }
}