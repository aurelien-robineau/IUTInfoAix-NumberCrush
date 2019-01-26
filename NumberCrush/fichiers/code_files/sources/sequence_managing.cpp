#include "../headers/sequence_managing.h"
#include "../headers/type.h"
#include "../headers/struct.h"
#include "../headers/grid_managing.h"
#include "../headers/const.h"

#include <stdlib.h>

using namespace std;

void FindSequenceInColumn (const CMat & KGrid, CSeqInfos & AlignPosX, const GameSettings & KParams)
{
    CPosition AlignPos; // coordinates of the first box of the sequence
    AlignPosX.resize(0); // forget old sequences

    for (unsigned y = 0; y < KParams.MapParamUnsigned.at("NbOfLines") - (KParams.MapParamUnsigned.at("HowMany")-1); ++y) // for each line where boxes could be the beginning of a sequence
    {
        for (unsigned x = 0; x < KParams.MapParamUnsigned.at("NbOfColumns"); ++x) // for each column
        {
            unsigned NumberOfSameCandy = 0; // number of same candy in the sequence
            /*if the number is not already included in a sequence or that it is not a empty box, a bomb, a block or a wall)*/
            if (((y != 0 &&
                 (KGrid[y][x] != KGrid[y-1][x] && KGrid[y-1][x] != KJoker)) || y == 0) &&
                  KGrid[y][x] != KEmpty &&
                  KGrid[y][x] != KBomb &&
                  KGrid[y][x] != KBlock &&
                  KGrid[y][x] != KWall)
            {
                unsigned NumberOfUniversalCandy = 0; // number of joker at the beginning of the sequence
                unsigned ybis = y; // allows to select next case without changing y value
                while (KGrid [ybis][x] == KJoker) // while jokers are found
                {
                    ++ybis; // next box
                    ++NumberOfUniversalCandy; // add 1 to number of joker
                }
                // box directly following the joker sequence is not a bomb, a block, a wall or an empty box
                if (KGrid [ybis][x] != KBomb && KGrid [ybis][x] != KBlock && KGrid [ybis][x] != KWall && KGrid [ybis][x] != KEmpty)
                {
                    ++NumberOfSameCandy; // sequence is at least 1 of lenght

                    while (ybis + NumberOfSameCandy < KParams.MapParamUnsigned.at("NbOfLines") && // end of grid
                           (KGrid [ybis][x] == KGrid [ybis+NumberOfSameCandy][x] // identical candie
                            || KGrid [ybis+NumberOfSameCandy][x] == KGrid [ybis][x] + KGold // golden candie
                            || KGrid [ybis+NumberOfSameCandy][x] == KGrid [ybis][x] - KGold // normal candie
                            || KGrid [ybis+NumberOfSameCandy][x] == KJoker)) // joker
                        ++NumberOfSameCandy; // add 1 to number of same candy
                }

                /*final size of sequence = jokers at the sequence beginning + sequence of identical candies or universal ones*/
                unsigned SizeOfSequence = NumberOfSameCandy + NumberOfUniversalCandy;

                if (SizeOfSequence >= KParams.MapParamUnsigned.at("HowMany"))
                {
                    /*saving first sequence candy position*/
                    AlignPos.first = y;
                    AlignPos.second = x;
                    /*saving all sequence infos*/
                    AlignPosX.push_back(make_pair(AlignPos, SizeOfSequence));
                }
            }
        }
    }
}//FindSequenceInColumn

void FindSequenceInLine (const CMat & KGrid, CSeqInfos & AlignPosY, const GameSettings & KParams)
{
    /*works the same way as FindSequenceInColumn*/

    CPosition AlignPos;
    AlignPosY.resize(0);

    for (unsigned y = 0; y < KParams.MapParamUnsigned.at("NbOfLines"); ++y)
    {
        for (unsigned x = 0; x < KParams.MapParamUnsigned.at("NbOfColumns") - (KParams.MapParamUnsigned.at("HowMany")-1); ++x)
        {
            unsigned NumberOfSameCandy = 0;
            if (((x != 0 &&
                 (KGrid[y][x] != KGrid[y][x-1] && KGrid[y][x-1] != KJoker)) || x == 0) &&
                  KGrid[y][x] != KEmpty &&
                  KGrid[y][x] != KBomb &&
                  KGrid[y][x] != KBlock &&
                  KGrid[y][x] != KWall)
            {
                unsigned NumberOfUniversalCandy = 0;
                unsigned xbis = x;
                while (KGrid [y][xbis] == KJoker)
                {
                    ++xbis;
                    ++NumberOfUniversalCandy;
                }
                if (KGrid [y][xbis] != KBomb && KGrid [y][xbis] != KBlock && KGrid [y][xbis] != KWall && KGrid [y][xbis] != KEmpty)
                {
                    ++NumberOfSameCandy;
                    while (xbis + NumberOfSameCandy < KParams.MapParamUnsigned.at("NbOfColumns") &&
                           (KGrid [y][xbis] == KGrid [y][xbis+NumberOfSameCandy]
                            || KGrid [y][x] == KGrid [y][xbis+NumberOfSameCandy] + KGold
                            || KGrid [y][x] == KGrid [y][xbis+NumberOfSameCandy] - KGold
                            || KGrid [y][xbis+NumberOfSameCandy] == KJoker))
                        ++NumberOfSameCandy;
                }

                unsigned SizeOfSequence = NumberOfSameCandy + NumberOfUniversalCandy;
                if (SizeOfSequence >= KParams.MapParamUnsigned.at("HowMany"))
                {
                    AlignPos.first = y;
                    AlignPos.second = x;
                    AlignPosY.push_back(make_pair(AlignPos, SizeOfSequence));
                }
            }
        }
    }
}//FindSequenceInLine

void ChangeInColumn (CMat & Grid, const CSeqInfos & KAlignPosX, const bool & KRemove, const unsigned & KNbCandies)
{
    if (KAlignPosX.size() == 0) return; // if no sequences, nothing to change

    unsigned NewValue; // new value of the box
    unsigned SizeOfSequence;

    for (unsigned i = 0; i < KAlignPosX.size(); i++) // for each sequence
    {
        SizeOfSequence = KAlignPosX[i].second;
        for (unsigned j = 0 ; j < SizeOfSequence; ++j) // for each box of the sequence
        {
            /*if Remove, delete box, else, give it a new random value*/
            NewValue = ((KRemove) ? KEmpty : (rand() % KNbCandies) + 1);
            Grid[KAlignPosX[i].first.first+j][KAlignPosX[i].first.second] = NewValue;
        }
    }
}//ChangeInColum

void ChangeInLine (CMat & Grid, const CSeqInfos & KAlignPosY, const bool & KRemove, const unsigned & KNbCandies)
{
    /*works the same way as ChangeInColumn*/

    if (KAlignPosY.size() == 0) return;

    unsigned NewValue;
    unsigned SizeOfSequence;

    for (unsigned i = 0; i < KAlignPosY.size(); i++)
    {
        SizeOfSequence = KAlignPosY[i].second;
        for (unsigned j = 0 ; j < SizeOfSequence; ++j)
        {
           NewValue = ((KRemove) ? KEmpty : (rand() % KNbCandies) + 1);
           Grid[KAlignPosY[i].first.first][KAlignPosY[i].first.second+j] = NewValue;
        }
    }
}//ChangeInLine

bool FindActiveGoldenCandies (const SequencesInfos & KSeq, const GameSettings & KParams, unsigned & PointCounter, CMat & Grid)
{
    /*trigger in lines*/
    unsigned FindInLines = 0;
    for (unsigned i = 0; i < KSeq.Auto.AlignY.size(); ++i)
    {
        unsigned y = KSeq.Auto.AlignY[i].first.first;
        for (unsigned x = KSeq.Auto.AlignY[i].first.second; x < (KSeq.Auto.AlignY[i].first.second + KSeq.Auto.AlignY[i].second); ++x)
        {
            if (Grid[y][x] > KGold && Grid[y][x] < 30)
            {
                ++FindInLines;
                DeleteCandies(Grid, KParams, PointCounter, Grid[y][x] - KGold);
            }
        }
    }

    /*trigger in columns*/
    unsigned FindInColumns = 0;
    for (unsigned i = 0; i < KSeq.Auto.AlignX.size(); ++i)
    {
        unsigned x = KSeq.Auto.AlignX[i].first.second;
        for (unsigned y = KSeq.Auto.AlignX[i].first.first; y < (KSeq.Auto.AlignX[i].first.first + KSeq.Auto.AlignX[i].second); ++y)
        {
            if (Grid[y][x] > KGold && Grid[y][x] < 30)
            {
                ++FindInColumns;
                DeleteCandies(Grid, KParams, PointCounter, Grid[y][x] - KGold);
            }
        }
    }
    if (FindInColumns > 0 || FindInLines > 0)
        return true;

    return false;
}//FindActiveGoldenCandies
