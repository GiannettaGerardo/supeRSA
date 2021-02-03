/**
 *  @file generics
 *
 *  @version 1
 *
 *  @date  08/10/2020
 *
 *  @author Gerardo Giannetta
 *
 */

/// Include the header file for this .c:
#include "generics.h"

/// Include util libraries:
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

/// Constant definition:
#define STANRDARD_BIT 640


/// Procedure implementation:

/**
 * Procedure to generate a random number of bit-digits. 
 * Next calculate the next prime from the start number.
 *
 * @pre prime, string == null
 *      bit != null > 0
 * 
 * @post prime != null && > 0
 */
void randomBigPrime(char * string, mpz_t prime, const int bit) {

    const char numbers[11] = "0123456789";
    mpz_t op;
    mpz_inits(op, NULL);
    int i = 0;
    
    string[0] = numbers[(rand() % 9) + 1];
    i = 1;

    while (i < bit) {

        string[i] = numbers[rand() % 10];
        i++;
    }
    string[i] = '\0';

    int flag = mpz_set_str(op, string, 10);
    assert(flag == 0);

    mpz_nextprime(prime, op);
}


/**
 * Procedure to control some options and prevent some errors at the beginning of the esecution.
 *
 * @pre arg, args != null
 */
short controlOptions(int arg, char *args[]) {

    if (arg == 1) {
        fprintf(stderr, "Incorrect syntax. Try ./supeRSA --help\n");        
        return 0;
    }

    /// check the --help option
    if (arg == 2 && (strcmp(args[1], "--help") == 0)) {
        help();
        return 0;
    }
    else if ((strcmp(args[1], "--help") == 0) && arg > 2) {
        fprintf(stderr, "Incorrect syntax. Try ./supeRSA --help\n");        
        return 0;
    }

    /// check the number of arguments
    if (arg > 5 || arg < 4) {
        fprintf(stderr, "Incorrect syntax. Try ./supeRSA --help\n");        
        return 0;
    }

    /// prevent the error: ./supeRSA -d -g
    if ((strcmp(args[1], "-d") == 0) && (args[2][1] == 'g')) {
        fprintf(stderr, "Incorrect syntax. Try ./supeRSA --help\n");
        return 0;
    }

    /// prevent the error: ./supeRSA error -g=chiavi.txt input.txt (first generated the keys, next gave the error)
    if ((strcmp(args[1], "-d") != 0) && (strcmp(args[1], "-e") != 0)) {

        if ((args[1][0] == '-') && (args[1][1] == 'e') && (args[1][2] == '=')) {

            if (isdigit(args[1][3]) == 0) {
                fprintf(stderr, "Incorrect syntax. Try ./supeRSA --help\n");
                return 0;
            }
        }
        else {
            fprintf(stderr, "Incorrect syntax. Try ./supeRSA --help\n");
            return 0;
        }
    }

    return 1;
}


/**
 * Function to control some options and prevent some errors at the beginning of the esecution.
 *
 * @pre args1 != null
 * 
 * @post 1 if bit <= 0
 *       standard bit if bit is not a number
 *       n > 0 if bit > 0
 */
int setBIT(char * args1) {
    
    int bit = 0;
    char * remaining_string = 0;
    char * token = NULL;
    
    token = strtok(args1, "=");
    token = strtok(NULL, "=");  /// token now is the number of bit

    // convert string to integer if there is an integer to convert
    bit = (int)strtol(token, &remaining_string, 0);

    /// if the remaining string is empty, the conversion was a success
    if (strcmp(remaining_string, "") == 0) {
        if (bit <= 0) {
            fprintf(stderr, "-e = %d is not supported, the number will be setted to 1.\n", bit);
            return 1;
        }
        return bit;
    }
    
    return STANRDARD_BIT;
}


/**
 * Function to control the correct use of function setBIT().
 *
 * @pre args != null
 * 
 * @post 1 if bit <= 0
 *       standard bit if bit is not a number or args is not -e=number
 *       n > 0 if bit > 0
 */
int controlBIT(char *args[]) {
    
    if ((args[1][0] == '-') && (args[1][1] == 'e') && (args[1][2] == '=') && (isdigit(args[1][3]) != 0)) {
        return setBIT(args[1]);
    }
    
    return STANRDARD_BIT;
}


/**
 * Procedure to delete an output file
 *
 * @pre arg, args != null
 */
void deleteOutputFile(int arg, char *args) {

    if (arg < 5) {
        if (remove("outputE_supeRSA.txt") != 0) {
            fprintf(stderr, "Error. Cannot remove the output file.\n");
        }
    }
    else {
        if (remove(args) != 0) {
            fprintf(stderr, "Error. Cannot remove the output file.\n");
        }
    }
}


/**
 * Procedure to print in output a documentation to help the users
 */
void help(void) {
    puts("Author: Gerardo Giannetta - Italy - Date: 2020 - Version: 1.1;");
    puts("supeRSA is a program writed in C to encrypt or decrypt (with RSA) an entire file or directly a string.\n");
    puts("The syntax is: ./supeRSA -[option1[=number]] -[option2=file_path] [[file_input]or[-s='string']] [file_output]\n");
    puts("-option1 is used to choose whether to encrypt or decrypt;");
    puts("-option2 is used to choose whether to generate new keys in a file or use an existing file with keys;\n");
    puts("Each option can be used once:");
    puts("Option1 choose if encrypt or decrypt;\nOption2 choose where keys need to be taken from;");
    puts("If option2 is -g, the path specify the file where the keys generated will be saved;");
    puts("If option1 is -d, option2 need to be -f=file_keys");
    puts("If option1 is -e, there is the possibility of choose a number of bits for encrypt, just put it next the -e with the syntax \"-e=number\",");
    puts("if there isn't a number, the option1 -e will choose a stadard number of bits (640);");
    puts("It isn't possibile to select the number of bits if option2 is -f;");
    puts("Instead of file_input, there may be -s='string' where 'string' is the string to encrypt;");
    puts("If option1 is -d, there must be a file_input and not -s;");
    puts("If there isn't a file_output path, standard file_output will be created in the directory of ./supeRSA,");
    puts("The files will be: outputE_supeRSA.txt if option1 is -e");
    puts("                   outputD_supeRSA.txt if option1 is -d");
    puts("Do not use ~ (to indicate user on linux distributions) symbol in the file path.");
    puts("-------------------------------------------------------------------------------");
    puts("number of bits: -e allows you to choose the number of digits to encrypt with;");
    puts("                if the number is omitted, automatic it is set to 640;");
    puts("*recommended range (prefers speed):             [300 - 500]");
    puts("*recommended range (good speed - safety ratio): [600 - 750]");
    puts("*recommended range (prefers safety):            [1000 - 1000+]");
    puts("-------------------------------------------------------------------------------");
    puts("option1: --help            shows description and options");
    puts("         -e                encrypt a file");
    puts("         -e=bit            encrypt a file with this number of bit");
    puts("         -d                decrypt a file");
    puts("-------------------------------------------------------------------------------");
    puts("option2: -g=path_file     generate keys in the file \"path_file\";");
    puts("                          do not use . or ./");
    puts("         -f=path_file     take keys from a file \"path_file\";");
    puts("                          do not use . or ./");
    puts("-------------------------------------------------------------------------------");
    puts("examples: ./supeRSA -e -f=keys.txt home/user/Desktop/file.txt saveEncryptedData.txt");
    puts("          ./supeRSA -d -f=/home/user/keys.txt file.txt /media/usb/saveDecryptedData.txt");
    puts("          ./supeRSA -e=340 -g=/home/user/saveKeys /home/user/file.txt");
    puts("          ./supeRSA -d -f=/home/user/keys.txt file.txt");
    puts("          ./supeRSA -e -g=keys.txt -s='encrypt this' file_output.txt");
    puts("          ./supeRSA -e=1024 -g=keys.txt -s'hello world'");
    puts("");
}