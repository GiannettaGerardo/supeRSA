/**
 *  @file rsa_gmp
 *
 *  @version 1
 *
 *  @date  06/10/2020
 *
 *  @author Gerardo Giannetta
 *
 */

/// Include the header file for this .c:
#include "rsa_gmp.h"

/// Iclude util libraries:
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>


/// Procedure implementation:

/**
 * Procedure to calculate public and private keys.
 *
 * @pre p, q != null && > 0
 *      n, d, e == null
 * 
 * @post n, e, d != null && > 0
 */
void rsa_keys(mpz_t n, mpz_t d, const mpz_t p, const mpz_t q, mpz_t e) {

    mpz_t p_1, q_1, lambda, mul, mod;
    mpz_inits(p_1, q_1, lambda, mul, mod, NULL);
    
    mpz_mul(n, p, q);   /// n = p * q

    publicKey(p, p_1, q, q_1, n, e, lambda);    /// (n, e)

    privateKey(d, e, lambda);     /// (n, d)

    mpz_clears(p_1, q_1, mul, mod, lambda, NULL);
}


/**
 * Procedure to calculate public keys.
 *
 * @pre p, q, n != null && > 0
 *      p_1, q_1, e, lambda == null
 * 
 * @post n, e != null && > 0
 */
void publicKey(const mpz_t p, mpz_t p_1, const mpz_t q, mpz_t q_1, mpz_t n, mpz_t e, mpz_t lambda) {

    mpz_t GCD;
    gmp_randstate_t state;
    mpz_inits(GCD, NULL);

    mpz_sub_ui(p_1, p, 1);          // p_1 = p - 1
    mpz_sub_ui(q_1, q, 1);          /// q_1 = q - 1
    mpz_lcm(lambda, p_1, q_1);      /// lambda = LCM(p-1, q-1)

    gmp_randinit_mt(state);         /// randomize state with gmp randomize function

    do {

        mpz_urandomm(e, state, lambda);      /// e = rand() % lambda
        mpz_gcd(GCD, e, lambda);             /// calculate the Greatest Common Divisor (GCD)

    } while (mpz_cmp_ui(GCD, 1) != 0);       /// if the GCD is not 1, repeat the random generation of e

    /// test with assert
    assert(mpz_cmp_ui(GCD, 1) == 0);
    assert(mpz_cmp_ui(e, 1) > 0);
    assert(mpz_cmp(lambda, e) > 0);

    /// now i have the public keys (n, e)

    mpz_clears(GCD, NULL);
}


/**
 * Procedure to calculate private keys.
 *
 * @pre e, lambda != null && > 0
 *      d == null
 * 
 * @post d != null && > 0
 */
void privateKey(mpz_t d, mpz_t e, const mpz_t lambda) {

    mpz_t mul, mod;
    mpz_inits(mul, mod, NULL);

    /// calculate d
    mpz_invert(d, e, lambda);

    /// e * d MOD lambda must be 1
    mpz_mul(mul, e, d);
    mpz_mod(mod, mul, lambda);

    /// test with assert
    assert(mpz_cmp_ui(mod, 1) == 0);

    mpz_clears(mul, mod, NULL);

    /// now i have the private keys (n, d)
}


/**
 * Procedure to encrypt a char and save it into a file.
 */
void encrypt(mpz_t encrypted, mpz_t message, const mpz_t e, const mpz_t n, int value, FILE* out) {

    mpz_set_ui(message, value);
    mpz_powm(encrypted, message, e, n);
    fprintf(out, "%s ", mpz_get_str(NULL, 0, encrypted));
}

/**
 * Procedure to decrypt a char and save it into a file.
 */
void decrypt(mpz_t original, mpz_t encrypted, char * encryptedChar, const mpz_t d, const mpz_t n, FILE* out) {

    int decipher;
    int flag = 0;
    char * remaining_string = 0; 

    flag = mpz_set_str(encrypted, encryptedChar, 10);
    assert (flag == 0);
    mpz_powm(original, encrypted, d, n);    // decrypt with the private keys
    strcpy(encryptedChar, mpz_get_str(NULL, 0, original)); 

    // convert string to integer if there is an integer to convert
    decipher = (int)strtol(encryptedChar, &remaining_string, 0);

    /// if the remaining string is empty, the conversion was a success
    if (strcmp(remaining_string, "") == 0) {
        fprintf(out, "%c", decipher);
    }
    else {
        fprintf(stderr, "Unexpected error in the string conversion fase.\n");
        fprintf(stderr, "Please stop and exit the program.\n");
        while (1) { } //infinite loop
    }
    
}