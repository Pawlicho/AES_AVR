#pragma once

#include "aes_common.h"

void aes_encrypt(uint8_t* expand_key, uint8_t* plain_text);
void add_round_key(uint8_t* expand_key, uint8_t* round_number, uint8_t* state_matrix);
void substitute_bytes(uint8_t* state_matrix);
void rotate_left(uint8_t matrix[4][4], uint8_t row);
void shift_rows(uint8_t* state_matrix);
void mix_columns(uint8_t* state_matrix);

void expand_key(uint8_t* key, uint8_t* expanded_key);