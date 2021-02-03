#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <gmp.h>
#include "rsa_gmp.h"
#include "generics.h"


int main(int arg, char *args[]) {

    /// some controls on program arguments
    if (controlOptions(arg, args) == 0) {
        return EXIT_FAILURE;
    }
    
    FILE *fileKeysPtr = NULL;                   /// keys file pointer
    FILE *fileInputPtr = NULL;                  /// input file pointer
    FILE *fileOutputPtr = NULL;                 /// output file pointer
    char *tokenString = NULL;                   /// string to encrypt with the option -s
    char *tokenPathFileKeys = NULL;             /// name of keys file
    char encryptedChar[MAX_MESSAGE] = {0};      /// the encrypted char taken from the file
    mpz_t n, d, e;                              /// the keys
    mpz_t encrypted, decrypted, message;        /// some gmp variables
    mpz_inits(n, d, e, encrypted, decrypted, message, NULL);

    // args[2] - keys
    if ((strlen(args[2]) >= 4) && ((args[2][0] == '-') && (args[2][1] == 'g') && (args[2][2] == '=') && (isgraph(args[2][3]) != 0))) {
        tokenPathFileKeys = strtok(args[2], "=");
        tokenPathFileKeys = strtok(NULL, "=");     /// now tokenPathFileKeys is the file path: -g=file_path

        mpz_t p, q;
        mpz_inits(p, q, NULL);
        srand(time(NULL));

        const int BIT = controlBIT(args);
        if (BIT == -1) {
            fprintf(stderr, "Incorrect syntax. Try ./supeRSA --help\n");
            return EXIT_FAILURE;
        }
        
        char generalPrime[BIT+5];   /// string for generate a random bing number

        /// generate random big prime numbers
        do {

            randomBigPrime(generalPrime, p, BIT);
            randomBigPrime(generalPrime, q, BIT);

        } while (mpz_cmp(p, q) == 0);    /// if p == q, repeat the random generation of p and q

        /// generate public and private keys
        rsa_keys(n, d, p, q, e);
        mpz_clears(p, q, NULL);

        if ((fileKeysPtr = fopen(tokenPathFileKeys, "w")) == NULL) {
            fprintf(stderr, "Error opening keys file.\n");
            return EXIT_FAILURE;
        }         
        
        /// save the keys on a file
        fprintf(fileKeysPtr, "%s ", mpz_get_str(NULL, 0, n));
        fprintf(fileKeysPtr, "%s ", mpz_get_str(NULL, 0, e));
        fprintf(fileKeysPtr, "%s", mpz_get_str(NULL, 0, d));
            
        fclose(fileKeysPtr);        
    }
    else if ((strlen(args[2]) >= 4) && ((args[2][0] == '-') && (args[2][1] == 'f') && (args[2][2] == '=') && (isgraph(args[2][3]) != 0))) {
        tokenPathFileKeys = strtok(args[2], "=");
        tokenPathFileKeys = strtok(NULL, "=");    /// now tokenPathFileKeys is the file path: -f=file_path

        int flag = 0; /// flag for assert test

        if ((fileKeysPtr = fopen(tokenPathFileKeys, "r")) == NULL) {
            fprintf(stderr, "Error opening keys file.\n");
            return EXIT_FAILURE;
        }

        /// prelevate the keys by a file
        fscanf(fileKeysPtr, "%s", encryptedChar);
        flag = mpz_set_str(n, encryptedChar, 10);
        assert (flag == 0);
        fscanf(fileKeysPtr, "%s", encryptedChar);
        flag = mpz_set_str(e, encryptedChar, 10);
        assert (flag == 0);
        fscanf(fileKeysPtr, "%s", encryptedChar);
        flag = mpz_set_str(d, encryptedChar, 10);
        assert (flag == 0);

        fclose(fileKeysPtr);
    }
    else {
        fprintf(stderr, "Incorrect syntax. Try ./supeRSA --help\n");
        return EXIT_FAILURE;
    }
    
    // args[1]
    if (strcmp(args[1], "-e") == 0) { // encryption starts

        if (arg < 5) {
            if ((fileOutputPtr = fopen("outputE_supeRSA.txt", "w+")) == NULL) {
                fprintf(stderr, "Error opening output file.\n");
                return EXIT_FAILURE;
            }
        } 
        else {
            if ((fileOutputPtr = fopen(args[4], "w+")) == NULL) {
                fprintf(stderr, "Error opening output file.\n");
                return EXIT_FAILURE;
            }
        } 

        /// if there is -s='string'
        if ((args[3][0] == '-') && (args[3][1] == 's') && (args[3][2] == '=') && (isgraph(args[3][3]) != 0)) {
            tokenString = strtok(args[3], "=");
            tokenString = strtok(NULL, "=");

            short i = 0;
            while (i < strlen(tokenString)) {
                int value = toascii(tokenString[i]);
                encrypt(encrypted, message, e, n, value, fileOutputPtr);
                i++;
            }
        }
        else {
            if ((fileInputPtr = fopen(args[3], "r+")) == NULL) {
                deleteOutputFile(arg, args[4]);
                fprintf(stderr, "Error opening input file.\n");
                return EXIT_FAILURE;
            }
            
            /// encrypt all the chars of the file
            while (1) {
                char ch = getc(fileInputPtr);
                if (ch == -1) {
                    break;
                }
                int value = toascii(ch);
                encrypt(encrypted, message, e, n, value, fileOutputPtr);
            }
            fclose(fileInputPtr);
        }

        fclose(fileOutputPtr);

    }
    else if (strcmp(args[1], "-d") == 0) {  // decryption starts

        if ((args[3][0] == '-') && (args[3][1] == 's') && (args[3][2] == '=') && (isgraph(args[3][3]) != 0)) {
            fprintf(stderr, "Incorrect syntax. Try ./supeRSA --help\n");
            return EXIT_FAILURE;
        }

        if ((fileInputPtr = fopen(args[3], "r")) == NULL) {
            printf("Error opening input file.\n");
            return EXIT_FAILURE;
        }

        if (arg < 5) {
            if ((fileOutputPtr = fopen("outputD_supeRSA.txt", "w+")) == NULL) {
                fclose(fileInputPtr);
                printf("Error opening output file.\n");
                return EXIT_FAILURE;
            }
        }
        else {
            if ((fileOutputPtr = fopen(args[4], "w+")) == NULL) {
                fclose(fileInputPtr);
                printf("Error opening output file.\n");
                return EXIT_FAILURE;
            }
        }

        /// decrypt all the big numbers of the file
        while (1) {
            if (fscanf(fileInputPtr, "%s", encryptedChar) == -1) {
                break;
            }
            decrypt(decrypted, encrypted, encryptedChar, d, n, fileOutputPtr);
        }
        fclose(fileInputPtr);
        fclose(fileOutputPtr);
    }
    else {
        fprintf(stderr, "Incorrect syntax. Try ./supeRSA --help\n");
        return EXIT_FAILURE;
    }

    mpz_clears(encrypted, decrypted, d, e, n, NULL);

    return EXIT_SUCCESS;
}