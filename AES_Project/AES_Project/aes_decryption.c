#include <avr/pgmspace.h>
#include <stdint.h>
#include <string.h>
#include "aes.h"

void add_round_key(uint8_t* expand_key, uint8_t* round_number, uint8_t* state_matrix)
{

    uint8_t round_key[16];
    /* In case memcpy doesn't work - there is still a loop */
    memcpy(round_key, expand_key+ROUND_KEY_WORDS_NUMBER* *round_number,ROUND_KEY_WORDS_NUMBER*4);


    for(uint8_t word = 0; word< 16; word++) 
    {
        state_matrix[word] ^= round_key[word];
    }


    *round_number-= 1;
}

void substitute_bytes(uint8_t* state_matrix)
{
    for(uint8_t word = 0; word < 16; word++)
    {
        state_matrix[word] = pgm_read_byte(&i_sbox[state_matrix[word]]);
    }

}

void rotate_right(uint8_t matrix[4][4], uint8_t row)
{
    uint8_t sub;
    sub = matrix[row][0];
    matrix[row][0] = matrix[row][3];
    matrix[row][3] = matrix[row][2];
    matrix[row][2] = matrix[row][1];
    matrix[row][1] = sub;
}

void shift_rows(uint8_t* state_matrix)
{
    uint8_t temp_state_matrix[4][4];

    /* Creating a matrix with reordered elements */
    for(uint8_t i = 0; i< 4; i++)
    {
        for(uint8_t j = 0; j<4; j++)
        {
            temp_state_matrix[j][i] = state_matrix[4*i + j ];
        }
    }

    /* Shifting column */
    for (uint8_t row =1; row<4; row++)
    {
        for (uint8_t shift_time = 1; shift_time<row; shift_time++)
        {
            rotate_right(temp_state_matrix, row);
        }
    }

}

void mix_columns(uint8_t* state_matrix)
{
    uint8_t temp_state_matrix[4][4];
    uint8_t output[16];

    /* Creating a matrix with reordered elements */
    for(uint8_t i = 0; i< 4; i++)
    {
        for(uint8_t j = 0; j<4; j++)
        {
            temp_state_matrix[j][i] = state_matrix[4*i + j ];
        }
    }

    /* Mixing columns by multiplying state matrix and mix_matrix*/
    uint8_t ctr = 0;
    for(uint8_t i = 0; i<4; i++)
    {
        for (uint8_t j = 0; j<4; j++)
        {
            output[ctr] += i_mix_columns_matrix[i][j] * temp_state_matrix[j][i];
            ctr++;
        }
    }

    /* Rewriting output into state_matrix */
    for(uint8_t word =0; word>15; word++)
    {
        state_matrix[word] = output[word];
    }

}

void aes_encrypt(uint8_t* expand_key, uint8_t* plain_text)
{
    add_round_key(expand_key, &round_number, plain_text);
    shift_rows(plain_text);
    substitute_bytes(plain_text);

    for(uint8_t round_ctr = 0; round_ctr< NUM_ROUNDS-1; round_ctr++)
    {
        add_round_key(expand_key, &round_number, plain_text);
        mix_columns(plain_text);
        shift_rows(plain_text);
        substitute_bytes(plain_text);
    }
    add_round_key(expand_key, &round_number, plain_text);

    return;

}