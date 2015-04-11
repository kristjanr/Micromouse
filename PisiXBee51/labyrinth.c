/*
 * labyrinth.c
 *
 * Created: 11.04.2015 18:08:04
 *  Author: Kristjan Roosild
 */
#define NOT_VISITED 'n'
#define INTERSECTION 'i'
#define WALL 'w'
int lab_length = 12;

char *build_labyrinth()
{
    int array_length = lab_length + lab_length - 1;
    char arr[array_length][array_length];
    char buff[100];
    for (int row = 0; row < array_length; row++)
    {
        for (int column = 0; column < array_length; column++)
        {
            if (column % 2 == 0 &&  row % 2 ==0)
            {
                arr[column][NOT_VISITED];
            }
            else if (column % 2 == 1 && row % 2 == 1)
            {
                arr[column][INTERSECTION];
            }
            else
            {
                arr[column][WALL];
            }
        }
    }
    return (char *)arr;
}