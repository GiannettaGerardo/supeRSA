# supeRSA
RSA encryption tool

********************************** IMPORTANT ************************************
If you do not want to download and use the executable directly, 
below is the procedure for manually compiling the code from the linux terminal:
(obviously you must have previously installed the GMP library)
gcc supeRSA.c generics.c rsa_gmp.c -o supeRSA -lgmp
*********************************************************************************
If you want to use the executable directly, you will have to download it and 
you will have to set the permissions for execution (only linux OS):
chmod +rx supeRSA
*********************************************************************************
This program has been tested on ubuntu 20.04 and Windows 10
*********************************************************************************

Author: Gerardo Giannetta - Italy - Date: 2020 - Version: 1.1;
supeRSA is a program writed in C to encrypt or decrypt (with RSA) an entire file or directly a string.

The syntax is: ./supeRSA -[option1[=number]] -[option2=file_path] [[file_input]or[-s='string']] [file_output]

-option1 is used to choose whether to encrypt or decrypt;
-option2 is used to choose whether to generate new keys in a file or use an existing file with keys;

Each option can be used once:
option1 choose if encrypt or decrypt;\nOption2 choose where keys need to be taken from;
If option2 is -g, the path specify the file where the keys generated will be saved;
If option1 is -d, option2 need to be -f=file_keys
If option1 is -e, there is the possibility of choose a number of bits for encrypt, just put it next the -e with the syntax "-e=number",
if there isn't a number, the option1 -e will choose a stadard number of bits (640);
It isn't possibile to select the number of bits if option2 is -f;
Instead of file_input, there may be -s='string' where 'string' is the string to encrypt;
If option1 is -d, there must be a file_input and not -s;
If there isn't a file_output path, standard file_output will be created in the directory of ./supeRSA,
The files will be: outputE_supeRSA.txt if option1 is -e
                   outputD_supeRSA.txt if option1 is -d
Do not use ~ (to indicate user on linux distributions) symbol in the file path.
-------------------------------------------------------------------------------
number of bits: -e allows you to choose the number of digits to encrypt with;
                if the number is omitted, automatic it is set to 640;
*recommended range (prefers speed):             [300 - 500]
*recommended range (good speed - safety ratio): [600 - 750]
*recommended range (prefers safety):            [1000 - 1000+]
-------------------------------------------------------------------------------
option1: --help            shows description and options
         -e                encrypt a file
         -e=bit            encrypt a file with this number of bit
         -d                decrypt a file
-------------------------------------------------------------------------------
option2: -g=path_file     generate keys in the file "path_file";
                          do not use . or ./
         -f=path_file     take keys from a file "path_file";
                          do not use . or ./
-------------------------------------------------------------------------------
examples: ./supeRSA -e -f=keys.txt home/user/Desktop/file.txt saveEncryptedData.txt
          ./supeRSA -d -f=/home/user/keys.txt file.txt /media/usb/saveDecryptedData.txt
          ./supeRSA -e=340 -g=/home/user/saveKeys /home/user/file.txt
          ./supeRSA -d -f=/home/user/keys.txt file.txt
          ./supeRSA -e -g=keys.txt -s='encrypt this' file_output.txt
          ./supeRSA -e=1024 -g=keys.txt -s='hello world'
          
          
