
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "aes_x86.h"

void myDebugPrint(uint8_t* plain_text)
{
    for(uint8_t i = 0; i<16; i++)
    {
        printf("\n%u", plain_text[i]);
    }
}

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
                temp[j] = sbox[temp[j]];
            }

            /* "left" vector byte XOR 2^(round_number - 1) */
            temp[0] ^= rcon_values[i/ROUND_KEY_WORDS_NUMBER]; //dunno if this operation is even legal
        }

        /* Temp vector XOR last 4 bytes befor i(round) bytes before key end */
        expanded_key[4*i] = temp[0] ^ expanded_key[4*i - 4*ROUND_KEY_WORDS_NUMBER];
        expanded_key[4*i + 1] = temp[1] ^ expanded_key[4*i - 4*ROUND_KEY_WORDS_NUMBER + 1];
        expanded_key[4*i + 2] = temp[2] ^ expanded_key[4*i - 4*ROUND_KEY_WORDS_NUMBER + 1];
        expanded_key[4*i + 3] = temp[3] ^ expanded_key[4*i - 4*ROUND_KEY_WORDS_NUMBER + 3];

    }

}

void add_round_key(uint8_t* expand_key, uint8_t* round_number, uint8_t* state_matrix)
{

    uint8_t round_key[16];
    /* In case memcpy doesn't work - there is still a loop */
    memcpy(round_key, expand_key+ROUND_KEY_WORDS_NUMBER* *round_number,ROUND_KEY_WORDS_NUMBER*4);


    for(uint8_t word = 0; word< 16; word++) 
    {
        state_matrix[word] ^= round_key[word];
    }


    *round_number+= 1;
}

void substitute_bytes(uint8_t* state_matrix)
{
    for(uint8_t word = 0; word < 16; word++)
    {
        state_matrix[word] = sbox[state_matrix[word]];
    }

}

void rotate_left(uint8_t matrix[4][4], uint8_t row)
{
    uint8_t sub;
    sub = matrix[row][3];
    matrix[row][3] = matrix[row][0];
    matrix[row][0] = matrix[row][1];
    matrix[row][1] = matrix[row][2];
    matrix[row][2] = sub;
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
            rotate_left(temp_state_matrix, row);
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
            output[ctr] += mix_columns_matrix[i][j] * temp_state_matrix[j][i];
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

    /* Initiating */
    uint8_t round_number = 0;
    add_round_key(expand_key, &round_number, plain_text);
    printf("%u", round_number);
    

    for(uint8_t round_ctr = 0; round_ctr< NUM_ROUNDS-1; round_ctr++)
    {
        substitute_bytes(plain_text);
        shift_rows(plain_text);
        mix_columns(plain_text);
        add_round_key(expand_key, &round_number, plain_text);
    }

    substitute_bytes(plain_text);
    shift_rows(plain_text);
    add_round_key(expand_key, &round_number, plain_text);

    return;

}

