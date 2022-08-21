#include <stdint.h>
#include <stdio.h>

void expand_key(uint8_t* key, uint8_t* expanded_key);
void aes_encrypt(uint8_t* expand_key, uint8_t* plain_text);


int main()
{
    uint8_t testowy_klucz[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    uint8_t testowy_tekst[16] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    uint8_t extended_klucz[176];

    expand_key(testowy_klucz, extended_klucz);
    aes_encrypt(extended_klucz, testowy_tekst);
    for(uint8_t i = 0; i<16; i++)
    {
        printf("%u", testowy_tekst[i]);
        if(i % 4 ==0)
        {
            printf(" ");
        }
    }
    

    return 0;
}