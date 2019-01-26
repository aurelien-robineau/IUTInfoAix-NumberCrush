#include "../headers/player_actions.h"
#include "../headers/grid_managing.h"
#include "../headers/type.h"
#include "../headers/const.h"

#include <iostream>
#include <string>

void TriggerBomb (CMat & Grid, const CPosition & KBombPos, const char & KDir, unsigned & PointCounter, const GameSettings & KParams)
{
    MoveInfos Move;
    Move.Pos = KBombPos;
    Move.Dir = KDir;

    CPosition NewBombPos = ArrivalBoxPosition(Move); // arrival bomb position
    const unsigned KLineNumber = NewBombPos.first; // arrival bomb line number
    unsigned ColumnNumber; // arrival bomb column number
    unsigned CptDistroyCandies = 0; // count the number of distroy candies

    ColumnNumber = NewBombPos.second;

    Grid [KLineNumber][ColumnNumber] = KEmpty; // delete bomb
    ++CptDistroyCandies; // 1 candy has been distroyed

    /*delete boxes at the right of the bomb while it is not a wall*/
    while (ColumnNumber+1 < KParams.MapParamUnsigned.at("NbOfColumns") && Grid [KLineNumber][ColumnNumber+1] != KWall)
    {
        Grid [KLineNumber][ColumnNumber+1] = KEmpty; // delete the box
        ++ColumnNumber;
        ++CptDistroyCandies; // 1 candy has been distroyed
    }

    /*delete boxes at the left of the bomb while it is not a wall*/
    ColumnNumber = NewBombPos.second;
    while (int(ColumnNumber-1) > -1 && Grid [KLineNumber][ColumnNumber-1] != KWall)
    {
        Grid [KLineNumber][ColumnNumber-1] = KEmpty; // delete the box
        --ColumnNumber;
        ++CptDistroyCandies; // 1 candy has been distroyed
    }
    PointCounter += CptDistroyCandies*(100/KParams.MapParamUnsigned.at("HowMany")); // add points due to explosion
}//TriggerBomb

void MakeAMove (CMat & Grid, const MoveInfos & KMove)
{
    unsigned PosNb = Grid [KMove.Pos.first][KMove.Pos.second]; // number at the entered position

    switch (KMove.Dir)
    {
        case 'Z' : // if move up
            /*swap current box value with line previous box value*/
            Grid [KMove.Pos.first][KMove.Pos.second] = Grid [KMove.Pos.first-1][KMove.Pos.second];
            Grid [KMove.Pos.first-1][KMove.Pos.second] = PosNb;
            break;
        case 'S' : // if move down
            /*swap current box value with line next box value*/
            Grid [KMove.Pos.first][KMove.Pos.second] = Grid [KMove.Pos.first+1][KMove.Pos.second];
            Grid [KMove.Pos.first+1][KMove.Pos.second] = PosNb;
            break;
        case 'Q' : // if move left
            /*swap current box value with column previous box value*/
            Grid [KMove.Pos.first][KMove.Pos.second] = Grid [KMove.Pos.first][KMove.Pos.second-1];
            Grid [KMove.Pos.first][KMove.Pos.second-1] = PosNb;
            break;
        case 'D' : // if move right
            /*swap current box value with column next box value*/
            Grid [KMove.Pos.first][KMove.Pos.second] = Grid [KMove.Pos.first][KMove.Pos.second+1];
            Grid [KMove.Pos.first][KMove.Pos.second+1] = PosNb;
            break;
    }
}//MakeAMove

void WaitForEnter()
{
    std::string Str;

    std::cout << std::endl << "Appuyez sur 'Entrer' ...";
    std::getline(std::cin, Str); // wait for the player to press 'Enter'
}//WaitForEnter
