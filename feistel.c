#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define BLOCK_SIZE 16 
#define KEY 0xABAB

int initial_permutation[] = {6, 9, 0, 3, 2, 11, 15, 12, 1, 4, 13, 5, 10, 7, 8, 14};
int inverse_permutation[] = {2, 8, 4, 3, 9, 11, 0, 13, 14, 1, 12, 5, 7, 10, 15, 6};

void permutation_function(uint8_t* block)
{
    uint8_t* newblock = calloc(BLOCK_SIZE, sizeof(uint8_t));
    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        newblock[i] = block[initial_permutation[i]];
    }
    memcpy(block, newblock, BLOCK_SIZE);
    free(newblock);
}

void inverse_permutation_function(uint8_t* block)
{
    uint8_t* newblock = calloc(BLOCK_SIZE, sizeof(uint8_t));
    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        newblock[i] = block[inverse_permutation[i]];
    }
    memcpy(block, newblock, BLOCK_SIZE);
    free(newblock);
}

void switching_function(uint8_t* block)
{
    uint8_t tmp;
    for (int i = 0; i < BLOCK_SIZE/2; i++)
    {
        tmp = block[BLOCK_SIZE/2 + i];
        block[BLOCK_SIZE/2 + i] = block[i];
        block[i] = tmp;
    }
}

void xor(uint8_t* block, uint8_t* key)
{
    for (int i = 0; i < BLOCK_SIZE/2; i++)
    {
        block[i] ^= *key;
    }
}

uint8_t* round_function(uint8_t* block, uint8_t* key)
{
    uint8_t* right = calloc(BLOCK_SIZE/2, sizeof(uint8_t));
    uint8_t* right_copy = calloc(BLOCK_SIZE/2, sizeof(uint8_t));
    memcpy(right, (block + BLOCK_SIZE/2), BLOCK_SIZE/2);
    memcpy(right_copy, (block + BLOCK_SIZE/2), BLOCK_SIZE/2);
    xor(right, key);
    memcpy((block + BLOCK_SIZE/2), right, BLOCK_SIZE/2);
    free(right);
    return right_copy;
}

void block_function(uint8_t* block, uint8_t* key)
{
    uint8_t* right_copy = round_function(block, key);
    uint8_t* left = calloc(BLOCK_SIZE/2, sizeof(uint8_t));
    uint8_t* right = calloc(BLOCK_SIZE/2, sizeof(uint8_t));
    memcpy(left, block, BLOCK_SIZE/2);
    memcpy(right, (block + BLOCK_SIZE/2), BLOCK_SIZE/2);
    xor(left, right);
    memcpy(block, left, BLOCK_SIZE/2);
    memcpy((block + BLOCK_SIZE/2), right_copy, BLOCK_SIZE/2);
    free(left);
    free(right);
}

void encrypt_chunk(uint8_t* block, uint16_t* key)
{ 
    uint8_t* key_right = calloc(BLOCK_SIZE/2, sizeof(uint8_t));
    uint8_t* key_left = calloc(BLOCK_SIZE/2, sizeof(uint8_t));
    memcpy(key_right, key + BLOCK_SIZE/2, BLOCK_SIZE/2);
    memcpy(key_left, key, BLOCK_SIZE/2);
    permutation_function(block);
    block_function(block, key_right);
    switching_function(block);
    block_function(block, key_left);
    inverse_permutation_function(block);
    free(key_right);
    free(key_left);
}

void decrypt_chunk(uint8_t* block, uint16_t* key)
{   
    uint8_t* key_right = calloc(BLOCK_SIZE/2, sizeof(uint8_t));
    uint8_t* key_left = calloc(BLOCK_SIZE/2, sizeof(uint8_t));
    memcpy(key_right, (key + BLOCK_SIZE/2), BLOCK_SIZE/2);
    memcpy(key_left, key, BLOCK_SIZE/2);
    permutation_function(block);
    block_function(block, key_left);
    switching_function(block);
    block_function(block, key_right);
    inverse_permutation_function(block);
    free(key_right);
    free(key_left);
}

int main(int argc, char* argv[])
{
    // if (argc != 3)
    // {
    //     fprintf(stderr, "Usage: feistel infile outfile\n");
    //     return 1;
    // }

    uint16_t key = KEY;
    uint8_t block[BLOCK_SIZE];
    printf("%zu\n", sizeof(block));
    memset(block, 0, sizeof(block));

    // Read in binary for encryption
    FILE* in = fopen("test.txt", "rb");
    FILE* out = fopen("encrypted.txt", "wb");
    
    // Error checking
    if (!in || !out)
    {
        fprintf(stderr, "Could not open infile or outfile.\n");
        return 1;
    }
    
    // bytes read from file.
    size_t bytes_read;
    while ((bytes_read = fread(block, 1, BLOCK_SIZE, in)) > 0)
    {
        if (bytes_read < BLOCK_SIZE)
        {
            memset((block + bytes_read), 0, (BLOCK_SIZE - bytes_read));
        }

        encrypt_chunk(block, &key);
        fwrite(block, 1, BLOCK_SIZE, out);
    }

    fclose(in);
    fclose(out);

    // Decryption
    FILE* enc_in = fopen("encrypted.txt", "rb");
    FILE* dec_out = fopen("decrypted.txt", "wb");

    if (!enc_in || !dec_out)
    {
        fprintf(stderr, "Error opening file(s)!");
        return 2;
    }

    while ((bytes_read = fread(block, 1, BLOCK_SIZE, enc_in)) > 0)
    {
        if (bytes_read < BLOCK_SIZE)
        {
            memset((block + bytes_read), 0, (BLOCK_SIZE - bytes_read));
        }

        decrypt_chunk(block, &key);
        fwrite(block, 1, BLOCK_SIZE, dec_out);
    }

    fclose(enc_in);
    fclose(dec_out);

    return 0;
}
