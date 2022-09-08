//Tristan Shawn Den Ouden 30/1/2022
//Week 1 - Mario.C

#include <stdio.h>
//CS50 Library
#include <cs50.h>

//Functions
//Adds spaces around the bricks to centre the pyramid
void centrePyramid(int height, int currentLayer);
//Lays the bricks
void layBricks(int width);

int main(void)
{
    //This integer will be used as the height of the pyramid.
    int pyr_height = 0;

    //Prompts the user for an integer
    //The integer will be assigned to int pyr_height
    //If outside the range 1-8, it will send a prompt again.
    while (pyr_height <= 0 || pyr_height >= 9)
    {
        pyr_height = get_int("How tall is the pyramid? (1-8)");
    }


    //x is used as the width and the current layer.
    //e.g when x = 2, the layer will be 2 and width of the pyramid will be 2.
    for (int x = 0; x < pyr_height; x++ )
    {
        centrePyramid(pyr_height,x);
        layBricks(x);
        printf("  ");
        layBricks(x);

        printf("\n");
    }
}

//Adds spaces around the bricks to centre the pyramid
void centrePyramid(int height, int currentLayer)
{
    /*Subtracts the currentLayer from height e.g 8 - 7 = 1
    This means that in a Pyramid with the height of 8, in the 7th layer, there will be 1 space.

    This difference is assigned to the spaces variable.
    */
    int spaces = height - currentLayer;

    //Prints a space until y is no longer less than the spaces variable
    //y starts at 1, so if there are 0 spaces it won't print any.
    for (int y=1; y<spaces; y++)
    {
        printf(" ");
    }

}

//Lays a brick y times, y being the width of the current layer.
void layBricks(int width)
{
    for (int y=0; y<=width; y++)
    {
        printf("#");
    }
}

