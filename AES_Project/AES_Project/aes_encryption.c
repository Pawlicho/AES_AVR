#include <string.h>
#include "aes_encryption.h"

const uint8_t sbox[256] PROGMEM ALIGN =
{
	0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
	0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
	0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
	0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
	0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
	0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
	0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
	0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
	0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
	0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
	0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
	0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
	0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
	0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
	0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
	0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

const uint8_t mix_columns_matrix[4][4] PROGMEM = 
{
    {0x02, 0x03, 0x01, 0x01}, {0x01, 0x02, 0x03, 0x01}, {0x01, 0x01, 0x02, 0x03}, {0x03, 0x01, 0x01, 0x02}
};

const uint8_t rcon_values[11] PROGMEM = {0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};

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
        state_matrix[word] = pgm_read_byte(&sbox[state_matrix[word]]);
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

void expand_key(uint8_t* key, uint8_t* expanded_key)
{

    uint8_t temp[4], swap;

    /* Initial copy of original 16 bytes of key */
    memcpy(expanded_key, key, ROUND_KEY_WORDS_NUMBER*4);

    for (int8_t i = ROUND_KEY_WORDS_NUMBER; i < 4 * (NUM_ROUNDS + 1); i++)
    {

        /* Copying last 4 bytes of expanded key into temporary vector and rotating */
        temp[0] = expanded_key[4*i - 4];
        temp[1] = expanded_key[4*i - 3];
        temp[2] = expanded_key[4*i - 2];
        temp[3] = expanded_key[4*i - 1];


        //need for change
        if (!(i % ROUND_KEY_WORDS_NUMBER))
        {
            /* Substituting bytes from sbox */
            swap = temp[3];
            temp[3] = pgm_read_byte(&sbox[temp[0]]);
            temp[0] = pgm_read_byte(&sbox[temp[1]]) ^ pgm_read_byte(&rcon_values[i/ROUND_KEY_WORDS_NUMBER]);
            temp[1] = pgm_read_byte(&sbox[temp[2]]);
            temp[2] = pgm_read_byte(&sbox[swap]);

        }

        /* Temp vector XOR last 4 bytes befor i(round) bytes before key end */
        expanded_key[4*i] = temp[0] ^ expanded_key[4*i - 4*ROUND_KEY_WORDS_NUMBER];
        expanded_key[4*i + 1] = temp[1] ^ expanded_key[4*i - 4*ROUND_KEY_WORDS_NUMBER + 1];
        expanded_key[4*i + 2] = temp[2] ^ expanded_key[4*i - 4*ROUND_KEY_WORDS_NUMBER + 2];
        expanded_key[4*i + 3] = temp[3] ^ expanded_key[4*i - 4*ROUND_KEY_WORDS_NUMBER + 3];

    }

}

