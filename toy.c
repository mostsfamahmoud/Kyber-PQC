/**********************************************************************
 * toy.c
 *
 * Created on: Dec 17, 2023
 *
 * Author: Mostafa Mahmoud Ali Ahmed
 *
 * Student ID: 1900034
 **********************************************************************/

#include "toy.h"
#include <stdlib.h>  // rand()
#include "stdio.h"

// polynomial multiplication in Z97[X]/(X^4+1)
static void toy_polmul_naive(short* dst, const short* a, const short* b, int add_to_dst) {
	short temp[POLY_SCALAR_SIZE]; // Temporary array to hold the result before deciding whether to add or replace dst

	// Perform polynomial multiplication using coefficients of a and b
	temp[0] = (a[0] * b[0] - a[3] * b[1] - a[2] * b[2] - a[1] * b[3]) % TK_Q;
	temp[1] = (a[1] * b[0] + a[0] * b[1] - a[3] * b[2] - a[2] * b[3]) % TK_Q;
	temp[2] = (a[2] * b[0] + a[1] * b[1] + a[0] * b[2] - a[3] * b[3]) % TK_Q;
	temp[3] = (a[3] * b[0] + a[2] * b[1] + a[1] * b[2] + a[0] * b[3]) % TK_Q;

	// Ensure results are within the modulo range
	for (int i = 0; i < POLY_SCALAR_SIZE; ++i) {
		if (temp[i] < 0)
			temp[i] += TK_Q;  // If the result is negative, adjust it to fit within the range [0, 96]
	}

	// Decide whether to add or replace dst based on the add_to_dst flag
	if (add_to_dst) {
		for (int i = 0; i < POLY_SCALAR_SIZE; ++i) {
			dst[i] += temp[i];  // Add the result to the existing contents of dst
			dst[i] %= TK_Q;     // Ensure the result remains within the modulo range
		}
	}
	else {
		for (int i = 0; i < POLY_SCALAR_SIZE; ++i)
			dst[i] = temp[i];   // Replace the contents of dst with the result
	}
}


void multiplyAndAccumulate(const short* A, const short* s, short A_RowCount, short* result) 
{
    // Perform matrix multiplication and accumulation for each row in A
    for (int i = 0; i < A_RowCount; ++i) 
	{
        // Initialize the elements in the result array for the current row to zero
        for (int j = 0; j < POLY_SCALAR_SIZE; ++j) 
		{
            result[j + i * POLY_SCALAR_SIZE] = 0;
        }

        // Perform multiplication and accumulation in chunks of 4 from each row in A
        for (int j = 0; j < 12; j += 4) 
		{
            short temp[POLY_SCALAR_SIZE] = { 0 }; // Temporary array to store the multiplication result

            // Perform polynomial multiplication for a chunk of 4 elements
            toy_polmul_naive(temp, &A[j + 12 * i], &s[j], 1);

            // Accumulate every 3 multiplications into the result array
            for (int k = 0; k < POLY_SCALAR_SIZE; ++k) 
			{
                result[k + i * POLY_SCALAR_SIZE] += temp[k]; // Add the temporary result to the corresponding result element
                result[k + i * POLY_SCALAR_SIZE] %= TK_Q;   // Ensure the result remains within the modulo range
            }
        }
    }
}

// Generate small gaussian random numbers
static void toy_fillSmall(short* buf, short size)
{
    short val;
    for (int i = 0; i < size; ++i)
    {
        val = rand() & 3;
        val = (val & 1) - ((val >> 1) & 1);

        if (val < 0)
            val += TK_Q;

        buf[i] = val;
    }
}   

// Transpose a 3D array
static void transpose3D(const short* input, short* output, int rows, int cols, int depth) 
{
    for (int current_row = 0; current_row < rows; ++current_row) 
	{
        for (int current_col = 0; current_col < cols; ++current_col) 
		{
            for (int current_depth = 0; current_depth < depth; ++current_depth) 
			{
                // Calculate indices for the transposed array
                int transposed_index = (current_col * rows * depth) + (current_row * depth) + current_depth;

                // Copy the element from input to its transposed position in output
                output[transposed_index] = input[current_row * cols * depth + current_col * depth + current_depth];
            }
        }
    }
}

// Generate keys and parameters for the cryptosystem
void toy_gen(short* A, short* t, short* s)
{
    // Fill K*K-matrix A with uniformly random numbers mod q
    for (int i = 0; i < (POLY_MATRIX_SIZE); ++i)
    {
        A[i] = rand() % TK_Q;
    }

    short e[POLY_VECTOR_SIZE];

    // Generate small random numbers mod q for e, and s simultaneously
    toy_fillSmall(s, POLY_VECTOR_SIZE);
    toy_fillSmall(e, POLY_VECTOR_SIZE);

    short AsResult[POLY_VECTOR_SIZE] = { 0 };

    // Perform matrix-vector multiplication: AsResult = A.s
    multiplyAndAccumulate(A, s, 3, AsResult);

    // Perform vector addition: t = AsResult + e
    for (int i = 0; i < (POLY_VECTOR_SIZE); ++i) {
        t[i] = 0;
        t[i] += (AsResult[i] + e[i]) % TK_Q; // Ensure the AsResult remains within the modulo range
    }
}

// Encrypt plaintext
void toy_enc(const short* A, const short* t, int plain, short* u, short* v) 
{
    short e1[POLY_VECTOR_SIZE];
    short e2[POLY_SCALAR_SIZE];
    short r[POLY_VECTOR_SIZE];

    // Generate small random numbers mod q for e1, e2, and r simultaneously
    toy_fillSmall(e1, POLY_VECTOR_SIZE);
    toy_fillSmall(r, POLY_VECTOR_SIZE);
    toy_fillSmall(e2, POLY_SCALAR_SIZE);

	// Transpose matrix A
	short A_transpose[TK_K * TK_K * TK_N];
	transpose3D(A, A_transpose, TK_K, TK_K, POLY_SCALAR_SIZE);

    // Perform matrix-vector multiplication: u = A_transpose.r + e1
    multiplyAndAccumulate(A_transpose, r, 3, u);
    for (int i = 0; i < (POLY_VECTOR_SIZE); ++i)
        u[i] = (u[i] + e1[i]) % TK_Q; // Add e1 and ensure modulo range
  
	// Transpose matrix t
	short t_transpose[POLY_VECTOR_SIZE];
	transpose3D(t, t_transpose, 1, TK_K, POLY_SCALAR_SIZE);

    // Perform matrix-vector multiplication: v = t_transpose.r + e2 + Round(plain * q/2)
    multiplyAndAccumulate(t_transpose, r, 1, v);

    for (int i = 0; i < POLY_SCALAR_SIZE; ++i) 
	{
        short msg_bit = (plain >> i) & 1;
        v[i] = (v[i] + e2[i] + (msg_bit * (TK_Q >> 1))) % TK_Q; // Add e2 and ensure modulo range
    }
}


// Decrypt ciphertext
int toy_dec(const short* s, const short* u, const short* v) 
{
    // Initialize an array to store the result of the matrix-vector multiplication s_transpose.u
    short suResult[POLY_SCALAR_SIZE] = { 0 };

	// Transpose matrix s
	short s_transpose[POLY_VECTOR_SIZE];
	transpose3D(s, s_transpose, 1, TK_K, POLY_SCALAR_SIZE);

    // Perform matrix-vector multiplication: suResult = s_transpose.u
    multiplyAndAccumulate(s, u, 1, suResult);

    // Array to hold intermediate values before determining plaintext bits
    short p[POLY_SCALAR_SIZE];

    // Calculate the difference between v and suResult, considering modular arithmetic
    for (int i = 0; i < POLY_SCALAR_SIZE; ++i) 
	{
        p[i] = (v[i] - suResult[i]) % TK_Q;

        // Adjust the result to fit within the range [-TK_Q/2, TK_Q/2)
        if (p[i] > (TK_Q / 2))
            p[i] -= TK_Q;

        //printf("%2d ", p[i]);
    }

    int plain = 0, bit = 0;

    // Reconstruct the plaintext from the decrypted values in p
    for (int i = 0; i < POLY_SCALAR_SIZE; ++i) 
	{
        // Determine the bit value based on the absolute value of p[i] compared to TK_Q/4
        bit = abs(p[i]) > (TK_Q / 4);

        // Set the plaintext bit at the correct position
        plain |= bit << i;
    }

    return plain; // Return the reconstructed plaintext
}
