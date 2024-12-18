// ConsoleApplication10.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <math.h>
using namespace std;
string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
string decription(string Str,int a,int b)
{
    int c=0;
    int p=0;
    int a_inv;
    for (int k = 0;k < 26;k++)                  //find a inverse
    {
        if ((a * k) % 26 == 1)
        {
            a_inv = k;
            break;
        }
        else
            continue;
    }
    string massage = "";
    for (int i = 0;i < Str.length();i++)
    {
        if(Str[i]>=65&&Str[i]<=90)       //if the letter is capital find its dycreption
        {
            c=Str[i]-65;
            p = (a_inv * (c - b)) % 26;

            if (p > -25 && p < 0)
                p += 26;
            p+=65;
            massage = massage + (char)p;
        }
        if(Str[i]>=97&&Str[i]<=122)     //if the letter is small find its dycreption
        {
            c=Str[i]-97;
            p = (a_inv * (c - b)) % 26;

            if (p > -25 && p < 0)
                p += 26;
            p+=97;
            massage = massage + (char)p;

        }

    }

    return massage;

}

