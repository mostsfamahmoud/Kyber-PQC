# Toy Cryptosystem - README

This document outlines the functionality and structure of the `toy.c` file, which implements a basic toy cryptosystem for educational purposes. The system utilizes matrix operations and polynomial multiplication in a finite field for encryption and decryption.

**Please note:** This is a simplified cryptosystem designed for learning and should not be used for real-world security applications.

## File Structure

- `toy.c`: Main implementation file containing the cryptosystem code.
- `toy.h`: Header file declaring constants and function prototypes.

## Cryptosystem Overview

The toy cryptosystem showcases basic encryption and decryption concepts using matrices and polynomials. It operates through three main functions:

1. **`toy_gen` (Key Generation):** Generates public and private keys for the system.
2. **`toy_enc` (Encryption):** Encrypts plaintext messages using the public key.
3. **`toy_dec` (Decryption):** Decrypts ciphertexts using the private key.

## Functions

### `toy_gen`

- **Purpose:** Generates keys and parameters for the cryptosystem.
- **Parameters:**
    - `A`: Matrix used in the system.
    - `t`: Vector used in the system.
    - `s`: Secret key vector.
- **Details:**
    1. Generates a random matrix `A` with values modulo `TK_Q`.
    2. Generates small random vectors `s` and `e`.
    3. Performs matrix-vector multiplication and vector addition to obtain `t`.

### `toy_enc`

- **Purpose:** Encrypts plaintext messages using the public key.
- **Parameters:**
    - `A`: Matrix used in the system.
    - `t`: Vector used in the system.
    - `plain`: Plaintext message to be encrypted.
    - `u`: Vector result of encryption.
    - `v`: Vector result of encryption with added noise.
- **Details:**
    1. Generates random vectors `e1`, `e2`, and `r`.
    2. Performs matrix-vector multiplication and vector addition to get `u`.
    3. Performs matrix-vector multiplication, vector addition, and adds noise to get `v`.

### `toy_dec`

- **Purpose:** Decrypts ciphertexts using the private key.
- **Parameters:**
    - `s`: Secret key vector.
    - `u`: Vector from encryption.
    - `v`: Vector from encryption with added noise.
- **Details:**
    1. Performs matrix-vector multiplication to get `suResult`.
    2. Calculates the difference between `v` and `suResult`.
    3. Reconstructs the plaintext from the decrypted values.

## Helper Functions

- **`toy_polmul_naive`:** Performs polynomial multiplication in Z97[X]/(X^4+1).
- **`multiplyAndAccumulate`:** Performs matrix multiplication and accumulation for each row in matrix `A`.
- **`toy_fillSmall`:** Generates small Gaussian random numbers.
- **`transpose3D`:** Transposes a 3D array.

## Constants

- **`TK_Q`:** Modulus for finite field operations.
- **`POLY_SCALAR_SIZE`, `POLY_VECTOR_SIZE`, `POLY_MATRIX_SIZE`:** Sizes of polynomial scalars, vectors, and matrices.
- **`TK_K`, `TK_N`:** Parameters defining the dimensions of the matrix.

## Cryptosystem Workflow

1. **Key Generation:**
    - Generate random matrix `A`.
    - Generate small random vectors `s` and `e`.
    - Perform matrix-vector multiplication and vector addition to obtain `t`.
2. **Encryption:**
    - Generate random vectors `e1`, `e2`, and `r`.
    - Perform matrix-vector multiplication and vector addition to get `u`.
    - Perform matrix-vector multiplication, vector addition, and add noise to get `v`.
3. **Decryption:**
    - Perform matrix-vector multiplication to get `suResult`.
    - Calculate the difference between `v` and `suResult`.
    - Reconstruct the plaintext from the decrypted values.

## Main Function

The main function tests the encryption and decryption functions for all possible 4-bit plaintext values. It iterates through each value, encrypts it, decrypts the ciphertext, and compares the results. The output verifies the functionality of the cryptosystem for all tested values.
