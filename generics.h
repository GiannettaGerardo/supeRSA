/**
 *  @file generics
 *
 *  RSA algorithm documentation: https://simple.wikipedia.org/wiki/RSA_algorithm
 *  GNU MP (GMP) documentation:  https://gmplib.org/manual/
 *
 *  Procedures:
 *  - randomBigPrime
 *  - controlOptions
 *  - setBIT
 *  - controlBIT
 *  - deleteOutputFile
 *  - help
 *
 *  @version 1
 *
 *  @date  08/10/2020
 *
 *  @author Gerardo Giannetta
 *
 */

/// Guard:
#ifndef GENERICS_H
#define GENERICS_H

/// Iclude util libraries:
#include <stdio.h>
#include <gmp.h>


/// Prototypes definition:

/**
 * Procedure to generate a random big prime. 
 *
 * @param string big string (empty).
 * @param prime  big prime number (empty).
 * @param bit    length of the big prime.
 */
void randomBigPrime(char * string, mpz_t prime, const int bit);


/**
 * Procedure to control some options and prevent some errors at the beginning of the esecution.
 *
 * @param arg  number of argument of the main program.
 * @param args strings array, argument of the main program.
 */
short controlOptions(int arg, char *args[]);


/**
 * Function to control some options and prevent some errors at the beginning of the esecution.
 *
 * @param args1 the argument of the main program in the first position.
 */
int setBIT(char * args1);


/**
 * Function to control the correct use of function setBIT().
 *
 * @param args strings array, argument of the main program.
 */
int controlBIT(char *args[]);


/**
 * Procedure to delete an output file
 *
 * @param arg  number of argument of the main program.
 * @param args strings array, argument of the main program.
 */
void deleteOutputFile(int arg, char * args);


/**
 * Procedure to print in output a documentation to help the users
 */
void help(void);


#endif