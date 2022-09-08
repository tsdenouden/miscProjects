//Tristan Shawn Den Ouden 30/1/2022
//Week 1 - Hello.C

#include <stdio.h>
//CS50 Library
#include <cs50.h>

int main(void)
{
    //Receives a string from the user
    //Assigns this string to a variable called userName
    string userName = get_string("What is your name?\n");
    //Greets the user
    //%s is a string placeholder for the userName variable.
    printf("Hello, %s\n", userName);
}