/*
 * AES_Project.c
 *
 * Created: 18.08.2022 18:41:32
 * Author : 101
 */ 

#include <avr/io.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

#include "aes_encryption.h"
#include "aes_decryption.h"
#include "aes_test_vector.h"


int main(void)
{

	srand((unsigned int)time(NULL));
	
    while (1) 
    {
		uint8_t test_key_vector[16];
		uint8_t test_input_vector[16];
		uint8_t plain_text[16];

		generate_random_block_values(test_key_vector);
		generate_random_block_values(test_input_vector);
		memcpy(plain_text, test_input_vector, sizeof(uint8_t)*16);

		uint8_t extended_key[176];
		expand_key(test_key_vector, extended_key);
		aes_encrypt(extended_key, test_input_vector);
		aes_decrypt(extended_key, test_input_vector);
		
		check_cipher_symmetry(plain_text, test_input_vector);
    }
}

