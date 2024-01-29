/**********************************************************************
 * toy.h
 *
 * Created on: Dec 17, 2023
 *
 * Author: Mostafa Mahmoud Ali Ahmed
 * 
 * ID: 1900034
 **********************************************************************/


#ifndef _TOY_H

//toy Post-Quantum Public-Key Cryptosystem
#define TK_K 3  
#define TK_N 4  // Coefficient modulus
#define TK_Q 97  // Field modulus
	
#define POLY_SCALAR_SIZE     (TK_N)
#define POLY_VECTOR_SIZE     (TK_K * POLY_SCALAR_SIZE)
#define POLY_MATRIX_SIZE     (TK_K * POLY_VECTOR_SIZE)

void toy_gen(short* A, short* t, short* s);
void toy_enc(const short* A, const short* t, int plain, short* u, short* v);
int toy_dec(const short* s, const short* u, const short* v);

#endif // !_TOY_H

