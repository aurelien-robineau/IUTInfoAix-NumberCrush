#include "../headers/tools.h"

#include <iostream>
#include <cctype>
#include <string>

#ifdef _WIN32 // tests if OS is windows
    #include <windows.h>
#endif

using namespace std;

namespace Console {
    void ClearScreen () // clears console
    {
        #ifdef _WIN32 // tests if OS is windows
            system("cls");
        #elif __linux__  // tests if OS is linux
            cout << "\033[H\033[2J";
        #elif __APPLE__ // tests if OS is an Apple OS
            system("clear");
        #else // if runnning under unknown OS
            cout << "Unable to clear screen" << endl;
        #endif
    }//ClearScreen

    void Color (const unsigned & KLinuxTextColor, const unsigned & KLinuxBackgroundColor, const int & KWindowsTextColor, const int & KWindowsBackgroundColor) // changes console color
    {
        #ifdef _WIN32 // tests if OS is windows
            HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(H,KWindowsBackgroundColor*16+KWindowsTextColor);
        #elif __linux__  // tests if OS is linux
            cout << "\033[" << KLinuxTextColor <<"m";
            cout << "\033[" << KLinuxBackgroundColor <<"m";
        #elif __APPLE__ // tests if OS is an Apple OS
            cout << "\x1b[" + KLinuxTextColor + "m";
            cout << "\x1b[" + KLinuxBackgroundColor + "m";
        #else // if runnning under unknown OS
            cout << "";
        #endif
    }//Color
}//namespace Console

unsigned CharCoordToUnsigned (const char & KLetter)
{
    char Comparator = 'A'; // used to stop when Letter is reached
    unsigned LetterValue = 0; // unsigned Letter value
    while (Comparator != KLetter)
    {
        ++Comparator;
        ++LetterValue;
    }
    return LetterValue;
}//CharCoordToUnsigned

char UnsignedCoordToChar (const unsigned & KNumber)
{
    char Letter = 'A';
    for (unsigned i = 0; i < KNumber; ++i)
    {
        ++Letter;
    }
    return Letter;
}//UnsignedCoordToChar

void ReplaceSpaceByUnderscore (string & String)
{
    for (unsigned i = 0; i < String.size(); ++i)
        if (isspace(String[i]))
            String[i] = '_';
}//ReplaceSpaceByUnderscore
