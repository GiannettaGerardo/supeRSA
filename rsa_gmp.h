/**
 *  @file rsa_gmp
 *
 *  RSA algorithm documentation: https://simple.wikipedia.org/wiki/RSA_algorithm
 *  GNU MP (GMP) documentation:  https://gmplib.org/manual/
 *
 *  Procedure:
 *  - rsa_keys;
 *  - publicKey;
 *  - privateKey;
 *  - encrypt;
 *  - decrypt;
 *
 *  @version 1.1
 *
 *  @date  08/10/2020
 *
 *  @author Gerardo Giannetta
 *
 */

/// Guard:
#ifndef RSA_GMP_H
#define RSA_GMP_H

/// Iclude util libraries:
#include <stdio.h>
#include <gmp.h>

/// Constant definition:
#define MAX_MESSAGE 5000
//#define BIT 1024 //310


/// Prototypes definition:

/**
 * Procedure to calculate public and private keys.
 *
 * @param n general key (empty). 
 * @param d private key (empty).
 * @param p first prime.
 * @param q second prime.
 * @param e public key (empty).
 */
void rsa_keys(mpz_t n, mpz_t d, const mpz_t p, const mpz_t q, mpz_t e);

/**
 * Procedure to calculate public keys.
 *
 * @param p      first prime.
 * @param p_1    first prime - 1 (empty).
 * @param q      second prime.
 * @param q_1    second prime - 1 (empty).
 * @param n      general key.
 * @param e      public key (empty).
 * @param lambda Least Common Multiple between p-1 and q-1 (empty).
 */
void publicKey(const mpz_t p, mpz_t p_1, const mpz_t q, mpz_t q_1, mpz_t n, mpz_t e, mpz_t lambda);


/**
 * Procedure to calculate private keys.
 *
 * @param d      private key (empty).
 * @param e      public key.
 * @param lambda Least Common Multiple between p-1 and q-1.
 */
void privateKey(mpz_t d, mpz_t e, const mpz_t lambda);


/**
 * Procedure to encrypt a char.
 *
 * @param encrypted the encrypted number (empty).
 * @param message   ASCII value in mpz_t type (empty).
 * @param e         public key.
 * @param n         general key.
 * @param value     ASCII value of a char.
 * @param out       output file pointer.
 */
void encrypt(mpz_t encrypted, mpz_t message, const mpz_t e, const mpz_t n, int value, FILE* out);


/**
 * Procedure to decrypt a char.
 *
 * @param original      the original ASCII number.
 * @param encrypted     the encrypted value in mpz_t type (empty).
 * @param encryptedChar one encrypted char.
 * @param d             private key.
 * @param n             general key.
 * @param out           output file pointer.
 */
void decrypt(mpz_t original, mpz_t encrypted, char * encryptedChar, const mpz_t d, const mpz_t n, FILE* out);


#endif