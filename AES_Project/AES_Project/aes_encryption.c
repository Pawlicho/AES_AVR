#pragma once
#include <avr/pgmspace.h>
#include <stdint.h>
#include <string.h>
#include "aes_encryption.h"


void expand_key(uint8_t* key, uint8_t* expanded_key)
{

    uint8_t temp[4];

    /* Initial copy of original 16 bytes of key */
    memcpy(expanded_key, key, ROUND_KEY_WORDS_NUMBER*4);

    for (int8_t i = ROUND_KEY_WORDS_NUMBER; i < 4 * (NUM_ROUNDS + 1); i++)
    {

        /* Copying last 4 bytes of expanded key into temporary vector and rotating */
        temp[0] = expanded_key[4*i - 4];
        temp[1] = expanded_key[4*i - 3];
        temp[2] = expanded_key[4*i - 2];
        temp[3] = expanded_key[4*i - 1];


        if (i % ROUND_KEY_WORDS_NUMBER == 0)
        {
            /* Substituting bytes from sbox */
            for(uint8_t j = 0; j<ROUND_KEY_WORDS_NUMBER; j++) 
            {
                temp[j] = pgm_read_byte(&sbox[temp[j]]);
            }

            /* "left" vector byte XOR 2^(round_number - 1) */
            temp[0] ^= pgm_read_byte(&rcon_values[i/ROUND_KEY_WORDS_NUMBER]); //dunno if this operation is even legal
        }

        /* Temp vector XOR last 4 bytes befor i(round) bytes before key end */
        expanded_key[4*i] = temp[0] ^ expanded_key[4*i - 4*ROUND_KEY_WORDS_NUMBER];
        expanded_key[4*i + 1] = temp[1] ^ expanded_key[4*i - 4*ROUND_KEY_WORDS_NUMBER + 1];
        expanded_key[4*i + 2] = temp[2] ^ expanded_key[4*i - 4*ROUND_KEY_WORDS_NUMBER + 1];
        expanded_key[4*i + 3] = temp[3] ^ expanded_key[4*i - 4*ROUND_KEY_WORDS_NUMBER + 3];

    }

}

void aes_encrypt(uint8_t* expand_key, uint8_t* plain_text, uint8_t* encrypted_text)
{

    /* Initiating */
    //uint8_t round_number = 0;
    //add_round_key(&expand_key, &round_number, &state_matrix);

    for(uint8_t round_ctr = 0; round_ctr< NUM_ROUNDS; round_ctr++)
    {

    }

}

static void add_round_key(uint8_t* expand_key, uint8_t* round_number, uint8_t* state_matrix)
{

    if(*round_number > 10)
    {
        return;
    }

    uint8_t round_key[16];
    /* In case memcpy doesn't work - there is still a loop */
    memcpy(round_key, expand_key+ROUND_KEY_WORDS_NUMBER* *round_number,ROUND_KEY_WORDS_NUMBER*4);

    for(uint8_t word = 0; word< 16; word++) 
    {
        state_matrix[word] ^= round_key[word];
    }


    *round_number++;
}

static void substitute_bytes(uint8_t* state_matrix)
{
    for(uint8_t word = 0; word < 16; word++)
    {
        state_matrix[word] = pgm_read_byte(&sbox[state_matrix[word]]);
    }
}
static void shift_rows(uint8_t* state_matrix)
{
    uint8_t temp[4][4];

    /* Trzeba przekminić mocniej */
    for(uint8_t i =0; i< 4; i++)
    {
        for(uint8_t j = 0; j<4; j++)
        {
            uint8_t row_number = i/4;
            if(i % 4 == 1) //row 2
            {

            }

            else if(i % 4 == 2) //row 3
            {

            }

            else if(i % 4 == 3) //row 4
            {

            }
        }
        
    }

}
static void mix_columns();