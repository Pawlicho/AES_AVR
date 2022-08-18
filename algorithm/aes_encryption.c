#pragma once
#include <avr/pgmspace.h>
#include <stdint.h>
#include "aes_encryption.h"

void expand_key(uint8_t* key, uint8_t* expanded_key)
{

    uint8_t temp[4];
    uint8_t sub;

    /* Initial copy of original 16 bytes of key */
    memcpy(expanded_key, key, ROUND_KEY_WORDS_NUMBER*4);

    for (int8_t i = ROUND_KEY_WORDS_NUMBER; i < 4 * (NUM_ROUNDS + 1); i++)
    {

        /* Copying last 4 bytes of expanded key into temporary vector and rotating */
        temp[0] = expanded_key[4*i - 4];
        temp[1] = expanded_key[4*i - 3];
        temp[2] = expanded_key[4*i - 2];
        temp[3] = expanded_key[4*i - 1];

        sub = temp[3];

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

void aes_encrypt(uint8_t* expand_key, uint8_t* plain_text, uint8_t encrypted_text)
{

    /* Initiating */
    uint8_t round_number = 0;
    add_round_key(&expand_key, &round_number, &state_matrix);

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



    /* ADD ROUND KEY NEDD TO BE IMPLEMENTED - keep in mind 4x4 matrix */

    *round_number++;
}

static void substitute_bytes()
{
    /* To też nie takie proste */
    // for(uint8_t i = 0; i<ROUND_KEY_WORDS_NUMBER; i++) 
    // {
    //     state_matrix[i] = pgm_read_byte(&sbox[state_matrix[i]]);
    // }
}
static void shift_raws();
static void mix_columns();