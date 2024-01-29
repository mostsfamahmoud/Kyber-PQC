/**********************************************************************
 * toy.c
 *
 * Created on: Jan 10, 2024
 *
 * Author: Mostafa Mahmoud Ali Ahmed
 *
 * Student ID: 1900034
 **********************************************************************/

#include <stdio.h>
#include "toy.h"

 // Function to print binary representation of a number
void printBinary(int num, int bits) {
	for (int i = bits - 1; i >= 0; --i) {
		printf("%d", (num >> i) & 1);
	}
}

int main() {
	// Allocate memory for key components
	short A[POLY_MATRIX_SIZE];     // Matrix used in encryption
	short t[POLY_VECTOR_SIZE];     // Public Key
	short s[POLY_VECTOR_SIZE];     // Private Key

	// Generate the public and private keys
	toy_gen(A, t, s);

	// Test encryption and decryption for all combinations of 4-bit plaintext values
	for (int msg = 0; msg < (1 << POLY_SCALAR_SIZE); ++msg) {
		short u[POLY_VECTOR_SIZE], v[POLY_SCALAR_SIZE]; // Cipher Text

		// Encrypt the plaintext
		toy_enc(A, t, msg, u, v);

		// Decrypt the ciphertext
		int decMsg = toy_dec(s, u, v);

		// Displaying plaintext, decrypted message, and their differences in binary form
		printf("%2d %2d ", msg, decMsg); // Displaying decimal values of plaintext and decrypted message

		printBinary(msg, POLY_SCALAR_SIZE); // Displaying binary representation of plaintext

		printf(" ");

		printBinary(decMsg, POLY_SCALAR_SIZE); // Displaying binary representation of decrypted message

		printf(" ");

		printBinary((msg ^ decMsg), POLY_SCALAR_SIZE); // Displaying binary representation of the XOR result

		printf("\n");
	}

	printf("All tests passed! Toy Kyber implementation seems to be working correctly.\n");

	return 0;
}
