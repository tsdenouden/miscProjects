//Tristan Shawn Den Ouden 31/1/2022
//Week 1 - Credit.C

#include <stdio.h>
//CS50 Library
#include <cs50.h>

//Functions
int luhn(int credit_card[], int card_size); //Luhn's Algorithm
void checkCard(int credit_card[], int card_size, int card_sum); //Checks the card validity & type
int checkLength(long number); //Checks the length of the card number

int main (void)
{
    int i = 0;
    //Receives the credit card number from the user
    long number = get_long("Enter Credit Card Number: ");
    //Checks the length of the credit card number
    int card_length = checkLength(number);

    //Array for the digits in the credit card number
    int card_number[card_length];

    //Splits the credit card number into individual digits and appends each digit to the list.
    while (number > 0)
    {
        int modulo = number % 10;
        card_number[i] = modulo;
        number /= 10;
        i++;
    }
    int sum_of_card = luhn(card_number, card_length);
    checkCard(card_number,card_length,sum_of_card);
}

//Checks the length of the credit card number
int checkLength(long number)
{
    int i = 0;
    while (number > 0)
    {
        number /= 10;
        i++;
    }
    return i;
}

int luhn(int credit_card[], int card_size)
{
    //Variables
    int sum = 0; //The sum of the products' digits
    int modulo = 0; //Used to get the modulus of multiply2
    int multiply2 = 0; //Stores the double of the current digit
    /*The numbers are arranged in reverse
    i starts at the second last number at index 1
    i is incremented by 2 every loop
    */
    for (int i=1; i<card_size; i+=2)
    {

        //Multiply the number by 2
        multiply2 = credit_card[i] * 2;

        //If this number is greater than 10, split it into 2 digits.
        if (multiply2 >= 10)
        {
            for (int x=0;x<2;x++) {
                //Modulo operation
                modulo = multiply2 % 10;
                //Add the single digit to the sum.
                sum += modulo;
                multiply2 /= 10;
            }
        }
        //If this number is less than 10, add it to the sum.
        else
        {
            sum += multiply2;
        }
    }

    //Add the remaining digits that weren't multiplied by 2.
    for (int i=0; i<card_size; i+=2)
    {
        sum += credit_card[i];
    }
    //Returns the sum
    return sum;
}

//Checks the validity & type of the card.
void checkCard(int credit_card[], int card_size, int card_sum)
{
    int firstCardNumber = credit_card[card_size - 1];
    if (card_sum%10 == 0)
    {
        if (firstCardNumber == 3)
        {
            printf("AMEX\n");
        }
        else if (firstCardNumber == 4)
        {
            printf("VISA\n");
        }
        else
        {
            printf("MASTERCARD\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}