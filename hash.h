#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <string>
#include <ctime>
#include <cstdlib>
#include <ctype.h>
#include <algorithm>

typedef std::size_t HASH_INDEX_T;

using namespace std;

unsigned long long powerOf36(int exponent)
{
    unsigned long long result = 1;
    for (int i = 0; i < exponent; i++)
    {
        result *= 36;
    }
    return result;
}
struct MyStringHash
{
    HASH_INDEX_T rValues[5]{983132572, 1468777056, 552714139, 984953261, 261934300};

    MyStringHash(bool debug = true)
    {
        if (false == debug)
        {
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string &k) const
    {
        int k_int;
        char k_char;
        string k_copy = k;

        // initialize array to hold letter values
        int a[6];
        for (int j = 0; j < 6; j++)
        {
            a[j] = 0;
        }

        // initialize 64 bit integers array
        unsigned long long w[5];
        for (unsigned long long x = 0; x < 5; x++)
        {
            w[x] = 0;
        }
        int six_set_num = 0;
        int i;

        while (!k_copy.empty())
        {
            if (k_copy.length() > 6)
            {
                i = k_copy.length() - 6;
            }
            else
            {
                i = 0;
            }

            // loop through letters in sets of 6
            for (int j = 0; j < 6; j++)
            {
                if (i < k_copy.length())
                {                           // letters remaining
                    if (isalpha(k_copy[i])) // if alphabet
                    {
                        k_char = tolower(k_copy[i]);
                        k_int = k_char - 97;
                    }
                    else if (isdigit(k_copy[i])) // if numeric
                    {
                        k_int = k_copy[i] - '0' + 26;
                    }
                    else
                    {
                        k_int = 0; // error handling
                    }
                    // placement in character's array
                    if (k_copy.length() >= 6)
                    {
                        a[j] = k_int;
                    }
                    else
                    {
                        a[6 - (k_copy.length()) + j] = k_int;
                    }

                    i++;
                }
                else // no more letters left, leading 0
                {
                    a[5 - j] = 0;
                }
            }

            unsigned long long w_placeholder = (powerOf36(5) * a[0]) + (powerOf36(4) * a[1]) + (powerOf36(3) * a[2]) + (powerOf36(2) * a[3]) + (powerOf36(1) * a[4]) + (powerOf36(0) * a[5]);
            // placement in w array
            if (six_set_num < 5)
            {
                w[4 - six_set_num] = w_placeholder;
            }
            // erasing letters used
            if (k_copy.length() > 6)
            {
                k_copy.erase(k_copy.length() - 6);
            }
            else
            {
                k_copy.clear();
            }

            six_set_num++;
        }

        unsigned long long hashed_result = (rValues[0] * w[0]) + (rValues[1] * w[1]) + (rValues[2] * w[2]) + (rValues[3] * w[3]) + (rValues[4] * w[4]);

        return hashed_result;
    }

    void generateRValues()
    {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator(seed);

        for (int i{0}; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};
#endif