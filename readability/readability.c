//Tristan Shawn Den Ouden 16/2/2022
//Week 2 - Readability.c

//Libraries
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

//Functions
int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

//Main
int main(void)
{
    //Prompt the user for a text
    string text = get_string("Text: ");

    //Count of letters,words,sentences
    float Letters = count_letters(text);
    float Words = count_words(text);
    float Sentences = count_sentences(text);

    //Letters per 100 words
    float L = (Letters / Words) * 100.00;
    //Sentences per 100 words
    float S = (Sentences / Words) * 100.00;

    //Coleman-Liau index
    //Note: Rounds the output and converts it to an integer.
    int index = (round(0.0588 * L - 0.296 * S - 15.8));
    //Prints the Grade level: 16+, 1-16, Before Grade 1
    if (index > 16)
    {
        //If the grade level is above 16, it will print 16+
        printf("Grade 16+\n");
    }
    else if (index <= 16 && index >= 1)
    {
        //Prints the grade level if between 1-16
        printf("Grade %i\n", index);
    }
    else
    {
        //If the grade level is lower than 1, prints 'Before Grade 1'
        printf("Before Grade 1\n");
    }
}

//Function that counts all the letters in a text.
int count_letters(string text)
{
    //Gets length of the text
    int text_length = strlen(text);
    //Letter count
    int letter_count = 0;
    //For every character in the text:
    for (int i = 0; i < text_length; i++)
    {
        //If the letter is between capital A and capital Z:
        if (text[i] >= 'A' && text[i] <= 'Z')
        {
            letter_count += 1;
        }
        //If the letter is between lowercase a and lowercase z:
        if (text[i] >= 'a' && text[i] <= 'z')
        {
            letter_count += 1;
        }
    }
    //printf("%i Letter(s)\n",letter_count);
    //Return the letter count
    return letter_count;
}

//Function that counts all the words in a text.
int count_words(string text)
{
    //Gets length of the text
    int text_length = strlen(text);
    //Word count
    int word_count = 0;
    //For every character in the text:
    for (int i = 0; i < text_length; i++)
    {
        //Every space = +1 New word.
        if (text[i] == ' ')
        {
            word_count += 1;
        }
    }
    //The very last word doesn't have a space after the punctuation.
    word_count += 1;
    //printf("%i Word(s)\n",word_count);
    //Return the word count
    return word_count;
}

//Function that counts all the sentences in a text.
int count_sentences(string text)
{
    //Gets length of the text
    int text_length = strlen(text);
    //Sentence count
    int sentence_count = 0;
    //For every character in the text:
    for (int i = 0; i < text_length; i++)
    {
        //Every . ! ? = +1 to the sentence count.
        //Because these symbols are usually used at the end of a sentence.
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentence_count += 1;
        }
    }
    //printf("%i Sentence(s)\n",sentence_count);
    //Return the sentence count
    return sentence_count;
}