#include <stdint.h>
#include <stdio.h>

void expand_key(uint8_t* key, uint8_t* expanded_key);
void aes_encrypt(uint8_t* expand_key, uint8_t* plain_text);
void aes_decrypt(uint8_t* expand_key, uint8_t* plain_text);
void add_round_key(uint8_t* expand_key, uint8_t* round_number, uint8_t* state_matrix);
void substitute_bytes(uint8_t* state_matrix);
void rotate_left(uint8_t matrix[4][4], uint8_t row);
void shift_rows(uint8_t* state_matrix);
void mix_columns(uint8_t* state_matrix);
void i_add_round_key(uint8_t* expand_key, uint8_t* round_number, uint8_t* state_matrix);
void i_substitute_bytes(uint8_t* state_matrix);
void rotate_right(uint8_t matrix[4][4], uint8_t row);
void i_shift_rows(uint8_t* state_matrix);
void i_mix_columns(uint8_t* state_matrix);


int main()
{
    /*Each function must be tested separately */ 

    uint8_t testowy_klucz[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    uint8_t testowy_tekst[16] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    uint8_t extended_klucz[176];
    expand_key(testowy_klucz, extended_klucz);
    aes_encrypt(extended_klucz, testowy_tekst);
    
    //sub jest na pewno zly, mix columns check, shift rows też dobrze, add round key też
    uint8_t ctr = 0;
    uint8_t i_ctr = 11;
    printf("\n\n");
    for(uint8_t i = 0; i<16; i++)
    {

        if(i % 4 ==0)
        {
            printf(" ");
        }
        printf("%x", testowy_tekst[i]);
        
    }
    mix_columns(testowy_tekst);
    printf("\n\n");
    for(uint8_t i = 0; i<16; i++)
    {

        if(i % 4 ==0)
        {
            printf(" ");
        }
        printf("%x", testowy_tekst[i]);
        
    }

    i_mix_columns(testowy_tekst);

    printf("\n\n");
    for(uint8_t i = 0; i<16; i++)
    {

        if(i % 4 ==0)
        {
            printf(" ");
        }
        printf("%x", testowy_tekst[i]);
        
    }
    
    // aes_decrypt(extended_klucz, testowy_tekst);

    // printf("\n\n");
    // for(uint8_t i = 0; i<16; i++)
    // {

    //     if(i % 4 ==0)
    //     {
    //         printf(" ");
    //     }
    //     printf("%x", testowy_tekst[i]);
        
    // }

    return 0;
}