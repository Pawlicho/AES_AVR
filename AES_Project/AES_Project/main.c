/*
 * AES_Project.c
 *
 * Created: 18.08.2022 18:41:32
 * Author : 101
 */ 

#include <avr/io.h>
#include <stdint.h>

void expand_key(uint8_t* key, uint8_t* expanded_key);
void aes_encrypt(uint8_t* expand_key, uint8_t* plain_text);
uint8_t testowy_klucz[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t testowy_tekst[16] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
uint8_t extended_klucz[176];

    expand_key(testowy_klucz, extended_klucz);
    aes_encrypt(extended_klucz, testowy_tekst);

int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
    }
}

