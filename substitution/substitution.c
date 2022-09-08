//Tristan Shawn Den Ouden 17/2/2022
//Week 2 - Substitution

//Libraries
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

//Functions
int checkKeyValidity(int arg_count, string key);
int invalidCharCheck(string key);
int checkForDuplication(string key);
void encrypt(string text, string key);

//Main
int main(int argc, string argv[])
{
    //Checks if the key is valid
    int valid_key = checkKeyValidity(argc, argv[1]);
    //If not, return 1 (error)
    if (valid_key == 1)
    {
        return 1;
    } // Else, get a text from the user and encrypt it
    else
    {
        string plain_text = get_string("plaintext: ");
        encrypt(plain_text, argv[1]);
    }
}

//Checks if they key is valid
int checkKeyValidity(int arg_count, string key)
{
    //First Check: (Checks if argument count is less or greater than 2)
    if (arg_count < 2 || arg_count > 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    //Second Check: (Checks if the key doesn't have 26 characters)
    int key_length = strlen(key);
    if (key_length != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    //Third Check: (Checks if the key has any invalid characters)
    int invalid_check = invalidCharCheck(key);
    if (invalid_check == 1)
    {
        printf("They key has invalid characters.\n");
        return 1;
    }

    //Fourth Check: (Checks if the key has any duplicate characters)
    int duplicate_check = checkForDuplication(key);
    if (duplicate_check == 1)
    {
        printf("The key must not have any duplicate characters.\n");
        return 1;
    }

    return 0;

}


//Checks if the key has any invalid characters.
int invalidCharCheck(string key)
{
    //Length of the key
    int char_count = strlen(key);
    //Goes through every character in the key
    for (int i = 0; i < char_count; i++)
    {
        //Uppercases the letter
        char upper_letter = toupper(key[i]);
        //If the letter is less than A (65) or more than Z (90), it is considered invalid
        if (upper_letter < 65 || upper_letter > 90)
        {
            return 1;
        }
    }
    //If no invalid characters were spotted, return a 0
    return 0;

}

//Checks if the key has any duplicate letters.
int checkForDuplication(string key)
{
    //Length of the key
    int char_count = strlen(key);
    //A counter for each letter in the alphabet
    int counter[26] = {0};

    //Counts how many times each letter shows up
    for (int i = 0; i < char_count; i++)
    {
        //Converts the letter to a capital
        char letter = toupper(key[i]);
        //printf("%c\n",letter);
        //Subtracts by A (65), so you get its position in the alphabet
        int position = letter - 65;
        //printf("%i\n",position);

        //Adds to the counter for that letter.
        /*E.g A is the first letter in the key,
        it will add +1 to counter[0]
        */
        counter[position] += 1;
    }

    /*for (int i = 0; i < 26; i++)
    {
        printf("%i ",counter[i]);
    }
    printf("\n");*/

    //Checks if any of the counters are greater than or equal to 2
    for (int i = 0; i < char_count; i++)
    {
        if (counter[i] >= 2)
        {
            return 1;
        }
    }
    return 0;
}

//Encrypts the text using the cipher key.
void encrypt(string text, string key)
{
    //The length of the original text
    int text_length = strlen(text);
    //The encrypted text
    char encrypted_text[text_length];
    //The alphabetical position of the current letter
    int current_letter;
    for (int i = 0; i < text_length; i++)
    {
        if (text[i] >= 'a' && text[i] <= 'z')
        {
            //Subtracts the letter by 'a' (97) to get it's alphabetical position.
            //E.g a = 0
            current_letter = text[i] - 97;
            /*The position of the letter in the alphabet is then used
            to find the index of the equivalent letter in the key.
            */
            encrypted_text[i] = tolower(key[current_letter]);
            //printf("%c\n", encrypted_text[i]);
        }
        else if (text[i] >= 'A' && text[i] <= 'Z')
        {
            //Subtracts the letter by 'A' (65) to get it's alphabetical position.
            //E.g A = 0
            current_letter = text[i] - 65;
            /*The position of the letter in the alphabet is then used
            to find the index of the equivalent letter in the key.
            */
            encrypted_text[i] = toupper(key[current_letter]);
            //printf("%c\n", encrypted_text[i]);
        }
        else
        {
            encrypted_text[i] = text[i];
        }
    }
    //encrypted_text[text_length] = '\0';
    printf("ciphertext: %s\n", encrypted_text);
}