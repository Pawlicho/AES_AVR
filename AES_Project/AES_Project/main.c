/*
 * AES_Project.c
 *
 * Created: 18.08.2022 18:41:32
 * Author : 101
 */ 

#include <avr/io.h>
#include "aes_encryption.h"
#include "aes_decryption.h"

int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
		uint8_t testowy_klucz[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
		uint8_t testowy_tekst[16] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
		uint8_t extended_klucz[176];
		expand_key(testowy_klucz, extended_klucz);
		aes_encrypt(extended_klucz, testowy_tekst);
		aes_decrypt(extended_klucz, testowy_tekst);
    }
}

