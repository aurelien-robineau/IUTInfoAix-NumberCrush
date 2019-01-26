#include "../headers/bot.h"
#include "../headers/type.h"
#include "../headers/tools.h"
#include "../headers/boolean_tests.h"
#include "../headers/grid_managing.h"
#include "../headers/sequence_managing.h"
#include "../headers/player_actions.h"
#include "../headers/database.h"
#include "../headers/struct.h"
#include "../headers/const.h"

#include <iostream>
#include <vector>
#include <time.h>
#include <unistd.h>

using namespace std;

/**
 * @brief Counts the points when the bot plays
 * @param[in] KAlignPosX : coordinates and lenght of each sequence found in columns
 * @param[in] KAlignPosY : coordinates and lenght of each sequence found in lines
 * @param[in|out] PointCounter : points of the bot
 * @param[in] KHowMany : minimal size of a sequence
 */
void BotCountPoints (const CSeqInfos & KAlignPosX, const CSeqInfos & KAlignPosY, unsigned & PointCounter, const unsigned & KHowMany)
{
    for (unsigned i = 0; i < KAlignPosX.size(); ++i)
    {
        PointCounter += 100 + ((KAlignPosX[i].second - (KHowMany)) * (100/KHowMany) * (KAlignPosX[i].second - (KHowMany)));
    }
    for (unsigned j = 0; j < KAlignPosY.size(); ++j)
    {
        PointCounter += 100 + ((KAlignPosY[j].second - (KHowMany)) * (100/KHowMany) * (KAlignPosY[j].second - (KHowMany)));
    }
    if (KAlignPosX.size() !=0 || KAlignPosY.size() !=0) PointCounter = PointCounter + (KAlignPosX.size() + KAlignPosY.size() - 1)*100;
}//BotCountPoints

void PlayBot(const GameSettings & KParams)
{
    MoveInfos BotMove;
    SequencesInfos Seq;

    /*Other*/
    bool Remove = true; // Used to specify if sequence should be removed or replace by other numbers
    unsigned PointCounter = 0; // Points earned by the player

    string StrNbGame;
    while (!IsValidUsignedValue(StrNbGame, 0, 1000000))
    {
        Console::ClearScreen();
        cout << "Jouez 1 seule partie pour voir le bot jouer !" << endl;
        cout << "Nombre de parties a effectuer : ";
        getline(cin, StrNbGame);
        cout << endl << "Parties en cours";
    }
    unsigned NbGame = stoul(StrNbGame);

    unsigned NbGameLeft = NbGame;
    while (NbGameLeft-- > 0)
    {
        CMat Grid (KParams.MapParamUnsigned.at("NbOfLines"), vector <unsigned> (KParams.MapParamUnsigned.at("NbOfColumns")));
        unsigned CurrentAttempts = KParams.MapParamUnsigned.at("Attempts");

        InitGrid (Grid, KParams);
        SetWalls (Grid, KParams);

        while (CurrentAttempts > 0)
        {
            CMat TestGrid;
            unsigned TestPointsCounter = 0;
            vector <pair<CPosition,pair<unsigned,char>>> Points;
            for (unsigned y = 0; y < KParams.MapParamUnsigned.at("NbOfLines"); ++y)
            {
                for (unsigned x = 0; x < KParams.MapParamUnsigned.at("NbOfColumns"); ++x)
                {
                    BotMove.Pos.first = y;
                    BotMove.Pos.second = x;

                    if (Grid [y][x] != KBlock && Grid [y][x] != KWall)
                    {
                        BotMove.Dir = 'Z';
                        if (BotMove.Pos.first > 0 && ArrivalBoxValue(Grid, BotMove) != KBlock && ArrivalBoxValue(Grid, BotMove) != KWall)
                        {
                            TestGrid = Grid;
                            BotMove.MovedCaseValue = Grid[BotMove.Pos.first][BotMove.Pos.second];
                            MakeAMove(TestGrid, BotMove);
                            if (BotMove.MovedCaseValue == KBomb) TriggerBomb(TestGrid, BotMove.Pos, BotMove.Dir, TestPointsCounter, KParams);
                            FindSequenceInLine(TestGrid, Seq.Auto.AlignY, KParams);
                            FindActiveGoldenCandies(Seq, KParams, TestPointsCounter, TestGrid);
                            BotCountPoints(Seq.Auto.AlignX, Seq.Auto.AlignY, TestPointsCounter, KParams.MapParamUnsigned.at("HowMany"));
                            Points.push_back(make_pair(make_pair(BotMove.Pos.first, BotMove.Pos.second), make_pair(TestPointsCounter, BotMove.Dir)));
                            TestPointsCounter = 0;
                        }
                        BotMove.Dir = 'Q';
                        if (BotMove.Pos.second > 0 && ArrivalBoxValue(Grid, BotMove) != KBlock && ArrivalBoxValue(Grid, BotMove) != KWall)
                        {
                            TestGrid = Grid;
                            BotMove.MovedCaseValue = Grid[BotMove.Pos.first][BotMove.Pos.second];
                            if (BotMove.MovedCaseValue == KBomb) TriggerBomb(TestGrid, BotMove.Pos, BotMove.Dir, TestPointsCounter, KParams);
                            FindSequenceInLine(TestGrid,Seq.Auto.AlignY, KParams);
                            FindActiveGoldenCandies(Seq, KParams, TestPointsCounter, TestGrid);
                            BotCountPoints(Seq.Auto.AlignX, Seq.Auto.AlignY, TestPointsCounter, KParams.MapParamUnsigned.at("HowMany"));
                            Points.push_back(make_pair(make_pair(BotMove.Pos.first, BotMove.Pos.second), make_pair(TestPointsCounter, BotMove.Dir)));
                            TestPointsCounter = 0;
                        }
                    }
                }
            }
            unsigned BestPoints = 0;
            for (unsigned i = 1; i < Points.size(); ++i)
            {
                if (Points[i].second.first > Points[BestPoints].second.first) BestPoints = i;
            }

            srand(time(NULL));
            if (Points[BestPoints].second.first == 0) BestPoints = rand()%Points.size();

            BotMove.Pos.first = Points[BestPoints].first.first;
            BotMove.Pos.second = Points[BestPoints].first.second;
            BotMove.MovedCaseValue = Grid[BotMove.Pos.first][BotMove.Pos.second];
            BotMove.Dir = Points[BestPoints].second.second;
            MakeAMove(Grid, BotMove);

            if (NbGame == 1)
            {
                DisplayGrid(Grid, KParams);
                cout << endl << Points[BestPoints].second.first;
                usleep(500000);
            }

            if (BotMove.MovedCaseValue == KBomb) TriggerBomb(Grid, BotMove.Pos, BotMove.Dir, PointCounter, KParams);//si la case choisie est une bombe (valeur 10), on déclenche la bombe

            if (NbGame == 1)
            {
                DisplayGrid(Grid, KParams);
                usleep(500000);
            }

            FindSequenceInColumn(Grid, Seq.Auto.AlignX, KParams);
            FindSequenceInLine(Grid, Seq.Auto.AlignY, KParams);
            FindActiveGoldenCandies(Seq, KParams, PointCounter, Grid);

            BotCountPoints(Seq.Auto.AlignX, Seq.Auto.AlignY, PointCounter, KParams.MapParamUnsigned.at("HowMany"));
            Remove = true;

            ChangeInColumn(Grid, Seq.Auto.AlignX, Remove, KParams.MapParamUnsigned.at("NbCandies"));
            ChangeInLine(Grid, Seq.Auto.AlignY, Remove, KParams.MapParamUnsigned.at("NbCandies"));
            if (NbGame == 1)
            {
                DisplayGrid(Grid, KParams);
                usleep(500000);
            }

            GoUpEmptyBoxes(Grid, KParams);

            if (NbGame == 1)
            {
                DisplayGrid(Grid, KParams);
                usleep(500000);
            }

            FillGrid(Grid, KParams);

            if (NbGame == 1)
            {
                DisplayGrid(Grid, KParams);
                usleep(500000);
            }

            --CurrentAttempts;
        }
    }
    if (NbGame == 1)
    {
        cout << "Score du bot : " << PointCounter << endl;
    }
    else
    {
        unsigned AVGPoints = PointCounter/NbGame;
        Console::ClearScreen();
        cout << "Nombre de parties : " << NbGame << endl;
        cout << "Moyenne des scores : " << AVGPoints;
    }
    cout << endl << "Appuyez sur 'Entrer' ...";
    string WaitForEnter;
    getline(cin, WaitForEnter);
}//PlayBot
