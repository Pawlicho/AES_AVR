#include "aes_test.h"
#include <string.h>
#include <stdlib.h>

uint8_t* generate_random_block()
{

    uint8_t* block = (uint8_t*)malloc(sizeof(uint8_t)*16);
    for(uint8_t i = 0; i < 16; i++)
    {
        uint8_t r = rand();
        block[i] == r;
    }
    

    return block;
}