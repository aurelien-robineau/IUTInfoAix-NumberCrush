#include "../headers/grid_managing.h"
#include "../headers/type.h"
#include "../headers/sequence_managing.h"
#include "../headers/player_actions.h"
#include "../headers/tools.h"
#include "../headers/boolean_tests.h"
#include "../headers/struct.h"
#include "../headers/const.h"

#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

void InitGrid (CMat & Grid, const GameSettings & KParams)
{
    CSeqInfos AlignPosX;
    CSeqInfos AlignPosY;
    const bool Remove = false; // indicate to replace found sequence and not to delete then (used in ChangeIn... functions)
    srand(time(NULL));

    for (unsigned y=0 ; y < KParams.MapParamUnsigned.at("NbOfLines") ; ++y) // for each line
    {
        for (unsigned x=0 ; x < KParams.MapParamUnsigned.at("NbOfColumns") ; ++x) // for each column
        {
            Grid [y][x] = (rand() % KParams.MapParamUnsigned.at("NbCandies")) + 1; // each box is filled with an random number between 1 and Nbcandies
        }
    }
    /*find each preformed sequence*/
    FindSequenceInColumn(Grid, AlignPosX, KParams);
    FindSequenceInLine(Grid, AlignPosY, KParams);

    while (AlignPosX.size() != 0 || AlignPosY.size() != 0) // while preformed sequences are found
    {
        /*replace each number of the sequence by another random number*/
        ChangeInColumn(Grid, AlignPosX, Remove, KParams.MapParamUnsigned.at("NbCandies"));
        ChangeInLine(Grid, AlignPosY, Remove, KParams.MapParamUnsigned.at("NbCandies"));
        /*find again each preformed sequence*/
        FindSequenceInColumn(Grid, AlignPosX, KParams);
        FindSequenceInLine(Grid, AlignPosY, KParams);
    }
}//InitGrid

void SetWalls (CMat & Grid, const GameSettings & KParams)
{
    vector <CPosition> WallPositions = KParams.MapParamCVPosition.at("WallPositions");

    for (unsigned i = 0; i < WallPositions.size(); ++i)
    {
        if (WallPositions[i].first < KParams.MapParamUnsigned.at("NbOfLines") &&
            WallPositions[i].first < KParams.MapParamUnsigned.at("NbOfColumns"))
        Grid [WallPositions[i].first][WallPositions[i].second] = KWall;
    }
}//SetWalls

unsigned ArrivalBoxValue (const CMat & KGrid, const MoveInfos & KMove)
{
    CPosition ArrivalPos = KMove.Pos;
    switch (KMove.Dir)
    {
        case 'Z' : // if move up
            --ArrivalPos.first;
            break;
        case 'S' : // if move down
            ++ArrivalPos.first;
            break;
        case 'Q' : // if move left
            /*swap current box value with column previous box value*/
            --ArrivalPos.second;
            break;
        case 'D' : // if move right
            ++ArrivalPos.second;
    }
    unsigned ArrivalBoxValue = KGrid [ArrivalPos.first][ArrivalPos.second];
    return ArrivalBoxValue;
}//ArrivalBoxValue

CPosition ArrivalBoxPosition (const MoveInfos & KMove)
{
    CPosition ArrivalPos = KMove.Pos;
    switch (KMove.Dir)
    {
        case 'Z' : // if move up
            --ArrivalPos.first;
            break;
        case 'S' : // if move down
            ++ArrivalPos.first;
            break;
        case 'Q' : // if move left
            /*swap current box value with column previous box value*/
            --ArrivalPos.second;
            break;
        case 'D' : // if move right
            ++ArrivalPos.second;
    }
    return ArrivalPos;
}//ArrivalBoxValue

void GoUpEmptyBoxes (CMat & Grid, const GameSettings & KParams)
{
    unsigned ybis; // allows to go to another y value without changing y value
    MoveInfos AutoMove; // position of the empty box
    AutoMove.Dir = 'Z'; // direction of the move

    srand(time(NULL));
    for (unsigned y=0 ; y < KParams.MapParamUnsigned.at("NbOfLines") ; ++y) // for each line
    {
        for (unsigned x=0 ; x < KParams.MapParamUnsigned.at("NbOfColumns") ; ++x) // for each column
        {
            if (Grid[y][x] == KEmpty) // if the box is empty
            {
                AutoMove.Pos.second = x;
                AutoMove.Pos.first = y;
                ybis = y;
                while (ybis > 0) // while not at the top of the grid
                {
                    AutoMove.Pos.first = ybis;
                    if (ArrivalBoxValue(Grid, AutoMove) == KWall) break;
                    MakeAMove (Grid, AutoMove); // make box go up once
                    --ybis;
                }
            }
        }
    }
}//GoUpEmptyBoxes

void FillGrid (CMat & Grid, const GameSettings & KParams)
{
    unsigned RandomNumber;
    CPosition CurrentPosition;

    bool BombsActivated = KParams.MapParamBool.at("BombsActivated");
    bool JokersActivated = KParams.MapParamBool.at("JokersActivated");
    bool BlocksActivated = KParams.MapParamBool.at("BlocksActivated");
    bool GoldenCandiesActivated = KParams.MapParamBool.at("GoldenCandiesActivated");

    srand(time(NULL));

    for (unsigned y=0 ; y < KParams.MapParamUnsigned.at("NbOfLines") ; ++y) // for each line
    {
        CurrentPosition.first = y;
        for (unsigned x=0 ; x < KParams.MapParamUnsigned.at("NbOfColumns") ; ++x) // for each column
        {
            CurrentPosition.second = x;
            if (Grid[y][x] == KEmpty && !IsUnderAWall (Grid, CurrentPosition)) // replace box if it is empty and not under a wall
            {
                RandomNumber = rand() % 40; // pick a random number

                if (BombsActivated && (RandomNumber == 0 || RandomNumber == 1))
                    Grid [y][x] = KBomb; // 1 chance on 20 for the box to be a bomb
                else if (JokersActivated && (RandomNumber > 1 && RandomNumber < 6))
                    Grid [y][x] = KJoker; // 1 chance on 10 for the box to be a joker
                else if (BlocksActivated && (RandomNumber > 5 && RandomNumber < 10))
                    Grid [y][x] = KBlock; // 1 chance on 10 for the box to be a block
                else if (GoldenCandiesActivated && (RandomNumber == 11))
                    Grid [y][x] = KGold + (rand() % KParams.MapParamUnsigned.at("NbCandies")) + 1; // 1 chance on 40 for the box to be golden
                else
                    Grid[y][x] = (rand() % KParams.MapParamUnsigned.at("NbCandies")) + 1; // if not a bomb or joker, box got a random number
            }
        }
    }
}//FillGrid

void DisplayXIndicator (const unsigned & KNbOfColumns)
{
    char Indicator = 'A'; // grid begins to A

    cout << "  "; // marging left
    for (unsigned i=0; i < KNbOfColumns; ++i) // for each column of the grid
    {
        cout << " " << Indicator++; // display next indicator
    }
    cout << endl << "   "; // marging left
    for (unsigned i=0; i < KNbOfColumns; ++i) // for each column of the grid
    {
        cout << "--"; // display separation between indicator and grid
    }
    cout << endl;
}//DisplayXIndicator

void DisplayGrid (const CMat & KGrid, const GameSettings & KParams)
{
    Console::ClearScreen();
    Console::Color(37,100,15,0);
    unsigned Value; // used to set colors
    char Indicator = 'A'; // grid begins to A

    DisplayXIndicator (KParams.MapParamUnsigned.at("NbOfColumns")); // display columns indicator
    for (unsigned y=0 ; y < KParams.MapParamUnsigned.at("NbOfLines") ; ++y) // for each line
    {      
        cout << Indicator++ << " |"; // display indicator for Y (lines) coordinates

        for (unsigned x=0 ; x < KParams.MapParamUnsigned.at("NbOfColumns") ; ++x) // for each column
        {
            if (KGrid [y][x] <= KParams.MapParamUnsigned.at("NbCandies") && KGrid [y][x] > 0) // if the box is a classic candy (number)
            {
                Value = KGrid [y][x]; // set Value to box value (gives each number a different color)
                Console::Color(0, Value+40, 0, Value); // set background color for the box
                cout << KGrid [y][x] << " "; // display the box
            }
            else if (KGrid [y][x] == KEmpty) // if the box is empty
            {
                Console::Color(37,100, 15, 0);
                cout << "  ";
            }
            else if (KGrid [y][x] == KJoker) // if the box is a joker
            {
                Console::Color(32,100, 2, 0);
                cout << "* ";
            }
            else if (KGrid [y][x] == KWall) // if the box is a wall
            {
                Console::Color(30,40, 7, 7);
                cout << "  ";
            }
            else if (KGrid [y] [x] > KGold && KGrid [y] [x] < 30) // if the box is a golden candy
            {
                Console::Color(33,40, 14, 0);
                cout << KGrid [y] [x] - KGold << ' ';
            }
            else // same color for each following value
            {
                Console::Color(31,100, 12, 0);

                if (KGrid [y][x] == KBomb) // if the box is a bomb
                    cout << "B!";
                else if (KGrid [y][x] == KBlock) // if the box is a block
                    cout << "[]";
                else // if the box is a unvalid number
                    cout << "ER";
            }

            Console::Color(37,100, 15, 0); // reset colors
        }
        cout << endl;
    }
    cout << endl;
}//DisplayGrid

void DeleteCandies (CMat & Grid, const GameSettings & KParams, unsigned & PointCounter, const unsigned & KValue)
{
    unsigned NbOfDeletedCandies = 0;
    for ( unsigned y=0; y < KParams.MapParamUnsigned.at("NbOfLines") ; ++y )
    {
        for ( unsigned x=0; x <KParams.MapParamUnsigned.at("NbOfColumns") ; ++x )
        {
            if (Grid [y][x] == KValue)
            {
                Grid [y][x] = KEmpty;
                ++NbOfDeletedCandies;
            }
        }
    }
    PointCounter += (NbOfDeletedCandies*(100/KParams.MapParamUnsigned.at("HowMany")))/2; // add points due to explosion
}//DeleteCandies
