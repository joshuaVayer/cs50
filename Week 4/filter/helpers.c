#include "helpers.h"
#include <math.h>
#include <cs50.h>

// PROTOYPES

RGBTRIPLE blurSidePixels(int i, int j, int height,  int width, RGBTRIPLE image[height][width]);
int checkRgb(int color);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE source = image[i][j];
            // Calculate the average of each color wich will the shade of grey
            int average = round((source.rgbtRed + source.rgbtGreen + source.rgbtBlue) / 3.0);
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
}

// MAIN FUNCTIONS

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE source = image[i][j];
            // Sepia math
            image[i][j].rgbtRed = checkRgb(round(0.393 * source.rgbtRed + 0.769 * source.rgbtGreen + 0.189 * source.rgbtBlue));
            image[i][j].rgbtGreen = checkRgb(round(0.349 * source.rgbtRed + 0.686 * source.rgbtGreen + 0.168 * source.rgbtBlue));
            image[i][j].rgbtBlue = checkRgb(round(0.272 * source.rgbtRed + 0.534 * source.rgbtGreen + 0.131 * source.rgbtBlue));
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        // We loop on half the image
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE *left = &image[i][j]; // Get the pixel from the right side
            RGBTRIPLE *right = &image[i][width - 1 - j]; // Get the coresponding oposite left pixel
            RGBTRIPLE temp = *left; // Store the oposite pixel in a temp variable
            *left = *right;
            *right = temp;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = blurSidePixels(i, j, height, width, image);
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }
}

// MAIN FUNCTIONS

// Check if the color is between 0 and 255
int checkRgb(int color)
{
    if (color > 255)
    {

        return 255;
    }

    if (color < 0)
    {

        return 0;
    }

    return color;
}

// Select the sides pixels and recolor them 
RGBTRIPLE blurSidePixels(int i, int j, int height,  int width, RGBTRIPLE image[height][width])
{
    int red = 0;
    int green = 0;
    int blue = 0;

    int validPixels = 0;

    for (int di = -1; di <= 1; di++)
    {
        for (int dj = -1; dj <= 1; dj++)
        {
            int new_i = i + di;
            int new_j = j + dj;
            if (new_i >= 0 && new_i < height && new_j >= 0 && new_j < width)
            {
                validPixels++;
                red += image[new_i][new_j].rgbtRed;
                green += image[new_i][new_j].rgbtGreen;
                blue += image[new_i][new_j].rgbtBlue;
            }
        }
    }
    RGBTRIPLE output;
    output.rgbtRed = round((float) red / validPixels);
    output.rgbtGreen = round((float) green / validPixels);
    output.rgbtBlue = round((float) blue / validPixels);
    return output;
}

