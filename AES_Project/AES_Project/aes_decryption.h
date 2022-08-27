#pragma once
#include "aes_common.h"

void aes_decrypt(uint8_t* expand_key, uint8_t* plain_text);
void i_add_round_key(uint8_t* expand_key, uint8_t* round_number, uint8_t* state_matrix);
void i_substitute_bytes(uint8_t* state_matrix);
void i_rotate_left(uint8_t matrix[4][4], uint8_t row);
void i_shift_rows(uint8_t* state_matrix);
void i_mix_columns(uint8_t* state_matrix);