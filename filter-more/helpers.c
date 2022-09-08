#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            float colour_Red = image[h][w].rgbtRed;
            float colour_Green = image[h][w].rgbtGreen;
            float colour_Blue = image[h][w].rgbtBlue;
            int colour_avg = round((colour_Red + colour_Green + colour_Blue) / 3);
            image[h][w].rgbtRed = colour_avg;
            image[h][w].rgbtGreen = colour_avg;
            image[h][w].rgbtBlue = colour_avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //Temporary pixel
    RGBTRIPLE temp_pixel;
    int real_width = width - 1;
    //For division
    float wid = width - 1;
    int middle_column = round(wid / 2);

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < middle_column; w++)
        {
            if (w != middle_column)
            {
                //Put the current pixel in the temp pixel
                temp_pixel = image[h][w];
                //Swap the last pixel to the current pixel
                image[h][w] = image[h][real_width - w];
                //Swap the temp pixel to the last pixel
                image[h][real_width - w] = temp_pixel;
            }
        }
    }
    return;
}
// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //Variables used to determine the size of the box around the pixel
    //x = loop starts on this value, y = loop ends before this value
    //h = height, w = width
    int pos_h_x;
    int pos_h_y;
    int pos_w_x;
    int pos_w_y;

    //Pixel count
    float pixel_count = 9.0;
    //The colour averages
    float avg_red;
    float avg_green;
    float avg_blue;
    //The blurred image
    RGBTRIPLE blur_image[height][width];
    //Temporary pixel
    float temp_red;
    float temp_green;
    float temp_blue;

    //Go through every middle pixel (not at the corner or at the edge)
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            //Reset the variables to the default values
            pos_h_x = -1;
            pos_h_y = 1;
            pos_w_x = -1;
            pos_w_y = 1;
            pixel_count = 9.0;
            //Checks where this pixel is located and
            //makes adjustments to pos_h and pos_w (variables used to determine the size of the box around the pixel)
            //Only 3 sizes -> 9 pixels, 6 pixels (edge), 4 pixels (corner)
            if (h == 0)
            {
                pos_h_x = 0;
                pos_h_y = 1;
                pixel_count -= 3.0;
            }
            if (w == 0)
            {
                pos_w_x = 0;
                pos_w_y = 1;
                pixel_count -= 3.0;
            }
            if (h == height - 1)
            {
                pos_h_x = -1;
                pos_h_y = 0;
                pixel_count -= 3.0;
            }
            if (w == width - 1)
            {
                pos_w_x = -1;
                pos_w_y = 0;
                pixel_count -= 3.0;
            }
            //Reset the colour averages to 0
            avg_red = 0;
            avg_green = 0;
            avg_blue = 0;
            //Go through every pixel in the box (9,6,4 pixels)
            for (int x = pos_h_x; x <= pos_h_y; x++)
            {
                for (int y = pos_w_x; y <= pos_w_y; y++)
                {
                    temp_red = image[h + x][w + y].rgbtRed;
                    temp_green = image[h + x][w + y].rgbtGreen;
                    temp_blue = image[h + x][w + y].rgbtBlue;
                    avg_red += temp_red;
                    avg_green += temp_green;
                    avg_blue += temp_blue;
                }
            }
            //When it is a corner, the pixel count (9) gets subtracted by 6 = 3
            //Just here to bump it to 4 if it's a corner
            if (pixel_count <= 3.0)
            {
                pixel_count = 4.0;
            }
            //Get the colour averages
            int colour_avg_red = round(avg_red / pixel_count);
            int colour_avg_green = round(avg_green / pixel_count);
            int colour_avg_blue = round(avg_blue / pixel_count);
            //Copy this into the blurred image
            blur_image[h][w].rgbtRed = colour_avg_red;
            blur_image[h][w].rgbtGreen = colour_avg_green;
            blur_image[h][w].rgbtBlue = colour_avg_blue;
        }
    }

    //Replace the image with the blurred version
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w] = blur_image[h][w];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //Variables used to determine the size of the box around the pixel
    //x = loop starts on this value, y = loop ends before this value
    //h = height, w = width
    int pos_h_x;
    int pos_h_y;
    int pos_w_x;
    int pos_w_y;
    //This is NOT gx & gy
    //Used to find the pixel's corresponding index in the matrices
    int gx = 0;
    int gy = 0;
    //GX & GY MATRICES
    int gx_multiply[3][3] =
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    int gy_multiply[3][3] =
    {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };
    //GX & GY FOR EVERY COLOUR
    int red_x = 0;
    int red_y = 0;
    int green_x = 0;
    int green_y = 0;
    int blue_x = 0;
    int blue_y = 0;

    //The final result
    int sobel_red = 0;
    int sobel_green = 0;
    int sobel_blue = 0;
    //The new image
    RGBTRIPLE edge_image[height][width];
    //Temporary pixel
    RGBTRIPLE temp;

    //Go through every pixel in the image
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            //Reset the variables to the default values
            pos_h_x = -1;
            pos_h_y = 1;
            pos_w_x = -1;
            pos_w_y = 1;
            red_x = 0;
            red_y = 0;
            green_x = 0;
            green_y = 0;
            blue_x = 0;
            blue_y = 0;
            temp = image[h][w];
            gx = 0;
            gy = 0;
            sobel_red = 0;
            sobel_green = 0;
            sobel_blue = 0;
            //Checks where this pixel is located and
            //makes adjustments to pos_h and pos_w (variables used to determine the size of the box around the pixel)
            //Only 3 sizes -> 9 pixels, 6 pixels (edge), 4 pixels (corner)
            if (h == 0)
            {
                pos_h_x = 0;
                pos_h_y = 1;
            }
            if (w == 0)
            {
                pos_w_x = 0;
                pos_w_y = 1;
            }
            if (h == height - 1)
            {
                pos_h_x = -1;
                pos_h_y = 0;
            }
            if (w == width - 1)
            {
                pos_w_x = -1;
                pos_w_y = 0;
            }
            //Go through every pixel in the box
            for (int x = pos_h_x; x <= pos_h_y; x++)
            {
                for (int y = pos_w_x; y <= pos_w_y; y++)
                {
                    //x & y is used to offset the current values of h,w (index of the current pixel)
                    //to look at all the other pixels within a 1 pixel radius
                    temp = image[h + x][w + y];
                    int temp_red = temp.rgbtRed;
                    int temp_green = temp.rgbtGreen;
                    int temp_blue = temp.rgbtBlue;
                    //Get this pixel's corresponding index in the gx,gy matrices
                    //Note: 1,1 is the middle pixel of the matrix
                    gx = 1 + x;
                    gy = 1 + y;
                    //GX
                    red_x += temp_red * gx_multiply[gx][gy];
                    green_x += temp_green * gx_multiply[gx][gy];
                    blue_x += temp_blue * gx_multiply[gx][gy];
                    //GY
                    red_y += temp_red * gy_multiply[gx][gy];
                    green_y += temp_green * gy_multiply[gx][gy];
                    blue_y += temp_blue * gy_multiply[gx][gy];
                }
            }
            //Final results for each channel
            sobel_red = round(sqrt((red_x * red_x) + (red_y * red_y)));
            sobel_green = round(sqrt((green_x * green_x) + (green_y * green_y)));
            sobel_blue = round(sqrt((blue_x * blue_x) + (blue_y * blue_y)));

            //Copy into new image
            edge_image[h][w].rgbtRed = (sobel_red > 255) ? 255 : sobel_red;
            edge_image[h][w].rgbtGreen = (sobel_green > 255) ? 255 : sobel_green;
            edge_image[h][w].rgbtBlue = (sobel_blue > 255) ? 255 : sobel_blue;
        }
    }

    //Replace the old image with the new image
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w] = edge_image[h][w];
        }
    }
    return;
}