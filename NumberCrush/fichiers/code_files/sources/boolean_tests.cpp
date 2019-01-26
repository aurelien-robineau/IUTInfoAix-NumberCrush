#include "../headers/boolean_tests.h"
#include "../headers/type.h"
#include "../headers/struct.h"
#include "../headers/const.h"

#include <cctype>
#include <string>

using namespace std;

bool IsValidUsignedValue (const string & KData, const unsigned & KMin, const unsigned & KMax) // check if KData is an unsigned between KMin and KMax
{
    unsigned Number;
    unsigned Digit = 0;

    if (KData == "") return false; // if KData is an empty string, then false
    for (unsigned i = 0; i < KData.size(); ++i) // for each char of KData
    {
        if (isdigit(KData[i])) ++Digit; // if the char is a number
    }
    if (Digit == KData.size()) Number = stoul(KData); // if each char of KData are numbers, turn KData into unsigned long
    else return false; // if not every char of KData are numbers, then false
    if (Number < KMin || Number > KMax) return false; // if Number is not between KMin and KMax, then false
    return true;
}//IsValidUsignedKData

bool IsValidLetter (const string & KData, const char & KMin, const char & KMax) // check if KData is char between KMin and KMax
{
    if (KData == "" || KData.size() > 1) return false; // if KData is an empty string or has more than 1 char, then false

    char Letter = toupper(KData[0]); // case does not matter
    if (!isalpha(Letter) || Letter < KMin || Letter > KMax) return false; // if Letter is not an alpha between KMin and KMax, then false
    else return true;
}//IsValidLetter

bool IsUnderAWall (const CMat & KGrid, const CPosition & KPosition)
{
    if (KPosition.first == 0) return false; // if box is at the top of the grid
    for (unsigned y = KPosition.first; int(y) != -1; --y) // for each box above tested box
        if (KGrid[y][KPosition.second] == KWall)
            return true; // if box is a wall

    return false;
}//IsUnderAWall
