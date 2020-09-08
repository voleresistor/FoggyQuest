/* util.c */
#include "util.h"
#include <time.h>
#include <stdlib.h>

/*
    randi()

    Return a random int.
*/
int rand_i(int min, int max)
{
    /* Can return a negative */
    if(min < 0)
    {
        return (rand() % (max * 2) - max);
    }
    
    return (rand() % (max - min)) + min;
}

/*
    randf()

    Return a random floating point number.
*/
float rand_f(float min, float max)
{
    int precision   = 100000;
    int f_min       = min * precision;
    int f_max       = max * precision;

    /* Can return a negative */
    if(min < 0.0)
    {
        return (float)((rand() % (f_max * 2) - f_max) / precision);
    }

    return (float)((rand() % (f_max - f_min) + f_min) / precision);
}

/*
    map()

    Map an input that lies within the given input range
    to an equivalent output in the given output range.

    Seems dumb to take floats and return an int. Fix this.
*/
int map(float input, float min_i, float max_i, float min_o, float max_o)
{
    if(input > max_i || input < min_i)
    {
        return -1;
    }

    return (int)(((input / (max_i - min_i)) * (max_o - min_o)) + 0.5);
}

/*
    largest_i()

    Return the largest of two given ints, either if they're
    equal, or 0 if something went wrong.
*/
int largest_i(int x, int y)
{
    if(x < y)
    {
        return y;
    }
    else
    {
        return x;
    }

    return 0;
}

/*
    smallest_i()

    Return the smallest of two given ints, either if they're
    equal, or 0 if something went wrong.
*/
int smallest_i(int x, int y)
{
    if(x > y)
    {
        return y;
    }
    else
    {
        return x;
    }

    return 0;
}

/*
    largest_f()

    Return the largest of two given floats, either if they're
    equal, or 0 if something went wrong.
*/
float largest_f(float x, float y)
{
    if(x < y)
    {
        return y;
    }
    else
    {
        return x;
    }

    return 0;
}

/*
    smallest_f()

    Return the smallest of two given floats, either if they're
    equal, or 0 if something went wrong.
*/
float smallest_f(float x, float y)
{
    if(x > y)
    {
        return y;
    }
    else
    {
        return x;
    }

    return 0;
}