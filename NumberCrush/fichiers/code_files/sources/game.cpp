/*!
 * \file game.cpp
 * \brief Contains the main function, every functions of the menu, and high level funtions that run the game.
 * This game is a sort of Candy Crush.
 * The main aim is to form sequences in order to get points and unlock levels.
 * Player can connect with a pseudo, and many informations are save or/and load in/from files.
 *
 * -# Context
 * \n This game is an educational project, made during the first semester at IUT of computer science Gaston Berger.
 * Aix-Marseille Universités.
 *
 * -# Operating system
 * \n the display might have issues if running under an other Os than Linux
 *
 * -# Data
 * \n All saved data must be in the compilation file.
 * Each folder listed in KDirectoriesName, declared at the beggining of main function,
 * must exist.
 *
 * \version 2.2
 * \date 25/01/2018 23:01:33
 * \author Aurélien ROBINEAU
 */

#include "../headers/type.h"
#include "../headers/tools.h"
#include "../headers/boolean_tests.h"
#include "../headers/grid_managing.h"
#include "../headers/sequence_managing.h"
#include "../headers/player_actions.h"
#include "../headers/database.h"
#include "../headers/bot.h"
#include "../headers/struct.h"
#include "../headers/const.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

namespace Menu {
    /*!
     * \brief Display the string in parameter surrounded by characters (depends on OS)
     * \param[in/out] s : string to surround
     * \fn void DisplayStringInABox (const string & s)
     */
    void DisplayStringInABox (const string & s)
    {
        #ifdef __linux__  // tests if OS is linux
            const string horizontalChar  = "═";
            const string verticalChar    = "║";
            const string topLeftChar     = "╔";
            const string topRightChar    = "╗";
            const string bottomLeftChar  = "╚";
            const string bottomRightChar = "╝";
        #else
            const string horizontalChar  = "-";
            const string verticalChar    = "|";
            const string topLeftChar     = "o";
            const string topRightChar    = "o";
            const string bottomLeftChar  = "o";
            const string bottomRightChar = "o";
        #endif

        cout << "  " + topLeftChar;
        for (unsigned i = 0; i < s.size(); ++i)
            cout << horizontalChar;
        cout << topRightChar << endl;

        cout << "  "+ verticalChar + s + verticalChar << endl;

        cout << "  " + bottomLeftChar;
        for (unsigned i = 0; i < s.size(); ++i)
            cout << horizontalChar;
        cout << bottomRightChar << endl;
    } // DisplayStringInABox

    /*!
     * \brief Lunch the main menu
     * \param[in/out] NbChoice : choice of the player
     * \param[in] KPlayer : all infos about the player
     * \fn void EnterMainMenu(unsigned & NbChoice, const PlayerInfos & KPlayer)
     */
    void EnterMainMenu(unsigned & NbChoice, const PlayerInfos & KPlayer)
    {
        string Choice; // used to check if the entered choice is valid
        Console::Color(0,100, 12, 0);
        Choice.resize(0); // reset choice
        while (!IsValidUsignedValue(Choice, 1, 4)) // while entered choice is not valid, display main menu again
        {
            Console::ClearScreen();
            Console::Color(92,100, 12, 0);
            DisplayStringInABox("MENU PRINCIPAL");
            Console::Color(37,100, 15, 0);

            if (KPlayer.PlayerPseudo == "Unconnected")
                cout << "Aucun profil connecte.";
            else
                cout << "Connecte en tant que " << KPlayer.PlayerPseudo << ".";

            cout << endl << endl;

            cout << "1 - Consulter les regles" << endl
                 << "2 - Jouer" << endl
                 << "3 - Connection" << endl
                 << "4 - Quitter" << endl;

            cout << endl << "Choix : ";
            getline(cin, Choice);
        }
        NbChoice = stoul(Choice); // when Choice is valid, turn it into unsigned value
    }//EnterMainMenu

    /*!
     * \brief Lunch the log in menu
     * \param[in|out] Player : all infos about the player
     * \param[in] KParams : all game settings
     * \fn void EnterLogInMenu(PlayerInfos & Player, const GameSettings & KParams)
     */
    void EnterLogInMenu(PlayerInfos & Player, const GameSettings & KParams)
    {
        string PlayerPseudo;

        Console::ClearScreen();

        Console::Color(92,100, 12, 0);
         DisplayStringInABox("CONNEXION");

        Console::Color(37,100, 15, 0);
        cout << "Connectez vous en tant que 'Bot' pour jouer automatiquement des parties !" << endl
             << "Un joueur 'Root' ayant tous les niveaux de debloques devrait exister." << endl;
        cout << "Entrez votre pseudo : ";
        getline(cin, PlayerPseudo);

        /*Case do not matter in file name for the OS so if player writes "Bot" in any way (bOt, bot, BOT,...), it will load file "player_Bot".
          The program sometimes test if PlayerPseudo == "Bot" so the entered string has to be turned into "Bot" no matter its case*/
        if (PlayerPseudo.size() == 3 && toupper(PlayerPseudo[0]) == 'B' && toupper(PlayerPseudo[1]) == 'O' && toupper(PlayerPseudo[2]) == 'T')
            PlayerPseudo = "Bot";

        ReplaceSpaceByUnderscore(PlayerPseudo); // spaces not allowed in pseudo
        Player.PlayerPseudo = PlayerPseudo; // save player pseudo

        // generate the file name for player profile
        Player.PlayerFileName = "../NumberCrush/fichiers/data_files/players/player_" + Player.PlayerPseudo + ".yalm";
        if (!AllDirectoriesExists(KDirectoriesName)) exit(2); // avoid the program to crash if a directory has been deleted
        Player::LoadProfile(Player, KParams);

        WaitForEnter();
    }

    /*!
     * \brief Display game rules
     * \fn void DisplayRules()
     */
    void DisplayRules()
    {
        Console::ClearScreen();

        Console::Color(92,100, 13, 0);
        DisplayStringInABox("REGLES");

        Console::Color(37,100, 15, 0);
        if (!AllDirectoriesExists(KDirectoriesName)) exit(2); // avoid the program to crash if a directory has been deleted
        ReadFile("../NumberCrush/fichiers/data_files/docs/rules.txt");

        Console::Color(92,100, 13, 0);
        DisplayStringInABox("POINTS");

        Console::Color(37,100, 15, 0);
        if (!AllDirectoriesExists(KDirectoriesName)) exit(2); // avoid the program to crash if a directory has been deleted
        ReadFile("../NumberCrush/fichiers/data_files/docs/points.txt");

        WaitForEnter();
    }//DisplayRules

    /*!
     * \brief Displays all levels and ask player to pick one
     * \param[in/out] StrLevel : level chose by the player (type string used to check if the value is valid)
     * \param[in] KParams : all game settings
     * \param[in] KPlayer : all infos about the player
     * \fn void ChoseLevel(string & StrLevel, const GameSettings & KParams, const PlayerInfos & KPlayer)
     */
    void ChoseLevel(string & StrLevel, const GameSettings & KParams, const PlayerInfos & KPlayer)
    {
        Console::ClearScreen();

        Console::Color(92,100, 12, 0);
         DisplayStringInABox("CHOIX DU LEVEL");
        Console::Color(37,100, 15, 0);

        if (KPlayer.PlayerPseudo != "Unconnected") // no levels for unconnected players
        {
            for (unsigned UnlockedLevel = 1; UnlockedLevel < KPlayer.MapPlayerUnsigned.at("HigherLevel")+1 && UnlockedLevel <= KParams.KNumberOfLevels; ++UnlockedLevel) // display each unlocked level
                cout << "Niveau " << setw(2) << UnlockedLevel << " : Disponible" << endl;
            for (unsigned LockedLevel = KPlayer.MapPlayerUnsigned.at("HigherLevel")+1; LockedLevel <= KParams.KNumberOfLevels; ++LockedLevel) // display each locked level
                cout << "Niveau " << setw(2) << LockedLevel << " : Bloque" << endl;

            cout << endl << "Choisi un niveau : ";
            getline(cin, StrLevel);
        }
        else // only for unconnected players
            StrLevel = "0"; // level for unconnected players
    }//ChoseLevel

    /*!
     * \brief Display all infos about the selected level
     * \param[in] KParams : all game settings
     * \fn void DisplayLevelInfos (const GameSettings & KParams)
     */
    void DisplayLevelInfos (const GameSettings & KParams)
    {
        cout << endl;

        if (KParams.MapLevelInfosUnsigned.size() != 0 && KParams.MapLevelInfosString.size() != 0) // if a score is saved
            cout << "Meilleur score pour ce niveau : " << KParams.MapLevelInfosUnsigned.at("BestScore") << " par " << KParams.MapLevelInfosString.at("Player") << endl;

        cout << "Alignez au moins " << KParams.MapParamUnsigned.at("HowMany") << " bonbons pour faire une suite." << endl
             << "Vous avez " << KParams.MapParamUnsigned.at("Attempts") << " tentatives." << endl;

        if (KParams.MapParamUnsigned.at("ScoreMin") != 0) // not displayed for unconnected players
            cout << "Faites au moins " << KParams.MapParamUnsigned.at("ScoreMin") << " points pour gagner !" << endl;

        WaitForEnter();
    }//DisplayLevelInfos

    /*!
     * \brief Lunch the level menu
     * \param[in|out] Params : all game settings
     * \param[in] KPlayer : all infos about the player
     * \fn void EnterLevelMenu(GameSettings & Params, const PlayerInfos & KPlayer)
     */
    void EnterLevelMenu(GameSettings & Params, const PlayerInfos & KPlayer)
    {
        Console::ClearScreen();
        string StrLevel = "0"; // used to check if the entered level is valid

        if (KPlayer.PlayerPseudo != "Unconnected")
            while (!IsValidUsignedValue(StrLevel, 1, KPlayer.MapPlayerUnsigned.at("HigherLevel"))) // while entered value is not an unlocked level
                ChoseLevel(StrLevel, Params, KPlayer);

        Params.Level = stoul(StrLevel); // when choosen level is valid, turn it into unsigned value
        // generate file name to save top score
        Params.LevelScoreFileName = "../NumberCrush/fichiers/data_files/levels/top_score/top_score_Level" + StrLevel + ".yalm";
        // generate file name to load level
        Params.LevelConfigFileName = "../NumberCrush/fichiers/data_files/levels/config/config_Level" + StrLevel + ".yalm";

        if (!AllDirectoriesExists(KDirectoriesName)) exit(2); // avoid the program to crash if a directory has been deleted
        Level::LoadLevelParams (Params);

        if (!AllDirectoriesExists(KDirectoriesName)) exit(2); // avoid the program to crash if a directory has been deleted
        Level::ReadBestScore(Params); // read choosen mode best score

        if (KPlayer.PlayerPseudo != "Bot")
            DisplayLevelInfos(Params);
    }//EnterLevelMenu

    /*!
     * \brief Displays a goodbye message for the player.
     * \param[in] KPlayer : all infos about the player
     * \fn void SayGoodbye (const PlayerInfos & KPlayer)
     */
    void SayGoodbye (const PlayerInfos & KPlayer)
    {
        Console::ClearScreen();
        cout << "Merci d'avoir joue" << ((KPlayer.PlayerPseudo != "Unconnected") ? " " : "") << KPlayer.PlayerPseudo << "." << endl;
        cout << "A bientot !" << endl << endl;
    }//SayGoodbye

    /*!
     * \brief Manage all functions in namespace Menu
     * \param[in|out] Params : all game settings
     * \param Player[in|out] : all infos about the player
     * \param LeaveGame[in/out] : boolean used to leave game
     * \fn void LoadMenu (GameSettings & Params, PlayerInfos & Player, bool & LeaveGame)
     */
    void LoadMenu (GameSettings & Params, PlayerInfos & Player, bool & LeaveGame)
    {
        unsigned NbChoice; // player choice in Main Menu

        while (true) // displays menu, leaved when player chose to play
        {
            EnterMainMenu(NbChoice, Player);

            if (NbChoice == 1) // if choice is to display rules
                DisplayRules();
            else if (NbChoice == 3) // if choice is to log in
                EnterLogInMenu(Player, Params);
            else if (NbChoice == 4) // if choice is to leave game
            {
                SayGoodbye(Player);
                LeaveGame = true;
                return;
            }
            else break; // if player's choice is a to play
        }//end of main loop

        EnterLevelMenu(Params, Player);
    }//Menu
}//namespace Menu

/*!
 * \brief Add points due to the sequences
 * \param[in] KSeq : all infos about sequences
 * \param[in|out] PointCounter : points of the player
 * \param[in] KHowMany : minimal size a sequence
 * \fn void CountPoints (const SequencesInfos & KSeq, unsigned & PointCounter, const unsigned & KHowMany)
 */
void CountPoints (const SequencesInfos & KSeq, unsigned & PointCounter, const unsigned & KHowMany)
{
    CSeqInfos AlignPos;
    for (unsigned WhichOne = 0; WhichOne < 2; ++WhichOne) // One time for columns, one time for lines
    {
        if (WhichOne == 0) AlignPos = KSeq.Auto.AlignX; // if first time --> columns
        else AlignPos = KSeq.Auto.AlignY; // if second time --> lines

        for (unsigned i = 0; i < AlignPos.size(); ++i) // for each sequence
        {
            /*each sequence add 100 points and each additional number add (100/minimal sequence lenght)*(number of additional number) points*/
            PointCounter += 100 + ((AlignPos[i].second - (KHowMany)) * (100/KHowMany) * (AlignPos[i].second - (KHowMany)));
        }
    }
    /*each additional sequence add 100 points*/
    if (KSeq.Auto.AlignX.size() !=0 || KSeq.Auto.AlignY.size() !=0)
        PointCounter = PointCounter + (KSeq.Auto.AlignX.size() + KSeq.Auto.AlignY.size() - 1)*100;
}//CountPoints

/*!
 * \brief Displays each removed sequences, their coordinates and lenght.
 * \param[in] KSeq : all infos about sequences
 * \fn void DisplayRemovedSequences (const SequencesInfos & KSeq)
 */
void DisplayRemovedSequences (const SequencesInfos & KSeq)
{
    CSeqInfos AlignPos;

    for (unsigned WhichOne = 0; WhichOne < 2; ++WhichOne) // One time for columns, one time for lines
    {
        if (WhichOne == 0) AlignPos = KSeq.Manu.AlignX; // if first time --> columns
        else AlignPos = KSeq.Manu.AlignY; // if second time --> lines

        if (AlignPos.size() == 1) cout <<"1 suite a ete supprimee "; // if only one sequence found
        else cout << AlignPos.size() << " suites ont ete supprimees "; // if several sequences found
        cout << "dans les ";
        cout << ((WhichOne == 0) ? "colonnes" : "lignes") << " directement grace a votre mouvement"; // case 1 --> columns, case 2 --> lines
        cout << endl;

        for (unsigned i = 0; i < AlignPos.size(); ++i) // display each sequence found
            cout << "La " << i+1 << "e demarrait en "
                 << UnsignedCoordToChar(AlignPos[i].first.second)
                 << ";"
                 << UnsignedCoordToChar(AlignPos[i].first.first)
                 << " et mesurait " << AlignPos[i].second << " cases." << endl;

        cout << endl;
    }
}//DisplayRemovedSequences

/*!
 * \brief Display grid and all game infos
 * \param[in] KGrid : grid of the game
 * \param[in] KParams : all game settings
 * \param[in] KSeq : all infos about sequences
 * \param[in] KCurrentAttempts : remaining attempts before the game ends
 * \param[in] KPointCounter : points of the player
 * \param[in] KFirstTime : used to display differents thing before the first move of the player
 * \fn void DisplayGame (const CMat           & KGrid,
                         const GameSettings   & KParams,
                         const SequencesInfos & KSeq,
                         const unsigned       & KCurrentAttempts,
                         const unsigned       & KPointCounter,
                         const bool           & KFirstTime)
 */
void DisplayGame (const CMat           & KGrid,
                  const GameSettings   & KParams,
                  const SequencesInfos & KSeq,
                  const unsigned       & KCurrentAttempts,
                  const unsigned       & KPointCounter,
                  const bool           & KFirstTime)
{
    Console::ClearScreen();
    DisplayGrid (KGrid, KParams);
    if (!KFirstTime) DisplayRemovedSequences(KSeq);
    cout << "Tentatives restantes : " << KCurrentAttempts << endl;
    cout << "Points actuels : " << KPointCounter;
    if (KParams.MapParamUnsigned.at("ScoreMin") != 0) cout << " / " << KParams.MapParamUnsigned.at("ScoreMin"); // not displayed for unconnected players
    cout << endl;
}//DisplayGame

/*!
 * \brief Ask to wait and makes game sleep
 * \param[in/out] MsDuration : time in milliseconds to wait
 * \fn void WaitAnimationMs (unsigned MsDuration)
 */
void WaitAnimationMs (unsigned MsDuration)
{
    cout << endl << "Wait..." << endl;
    MsDuration *= 1000; // turn milliseconds into microseconds
    usleep(MsDuration);
}//WaitAnimationMs

/*!
 * \brief Do everything that is automatic between two player's move.
 * \param[in|out] Grid : grid of the game
 * \param[in|out] Seq : all infos about sequences
 * \param[in] KCurrentAttempts : remaining attempts before the game ends
 * \param[in|out] PointCounter : points of the player
 * \param[in|out] FirstTime : used to display differents thing before the first move of the player
 * \param[in|out] Remove : true to delete sequences, false to replace sequences with new numbers
 * \param[in] KParams : all game settings
 * \param[in/out] ContinueAnimation : used to continue automatic stuff while it have to
 * \fn void RunAutomaticGridManaging (CMat               & Grid,
                                      SequencesInfos     & Seq,
                                      const unsigned     & KCurrentAttempts,
                                      unsigned           & PointCounter,
                                      const bool         & FirstTime,
                                      bool               & Remove,
                                      const GameSettings & KParams,
                                      bool               & ContinueAnimation)
 */
void RunAutomaticGridManaging(CMat               & Grid,
                              SequencesInfos     & Seq,
                              const unsigned     & KCurrentAttempts,
                              unsigned           & PointCounter,
                              const bool         & FirstTime,
                              bool               & Remove,
                              const GameSettings & KParams,
                              bool               & ContinueAnimation)
{
    CMat TestGrid;
    while (true) // while automatically formed sequences are found
    {
        DisplayGame (Grid, KParams, Seq, KCurrentAttempts, PointCounter, FirstTime);

        TestGrid = Grid;
        ChangeInColumn(TestGrid, Seq.Auto.AlignX, Remove, KParams.MapParamUnsigned.at("NbCandies"));
        ChangeInLine(TestGrid, Seq.Auto.AlignY, Remove, KParams.MapParamUnsigned.at("NbCandies"));

        if (TestGrid != Grid) // show "animation" only if grid has changed
        {
            WaitAnimationMs(400);

            /*remove sequence*/
            Remove = true;
            ChangeInColumn(Grid, Seq.Auto.AlignX, Remove, KParams.MapParamUnsigned.at("NbCandies"));
            ChangeInLine(Grid, Seq.Auto.AlignY, Remove, KParams.MapParamUnsigned.at("NbCandies"));

            DisplayGame (Grid, KParams, Seq, KCurrentAttempts, PointCounter, FirstTime);
            WaitAnimationMs (400);
        }

        TestGrid = Grid;
        GoUpEmptyBoxes (TestGrid, KParams);

        if (TestGrid != Grid) // show "animation" only if grid has changed
        {
            GoUpEmptyBoxes (Grid, KParams);

            DisplayGame (Grid, KParams, Seq, KCurrentAttempts, PointCounter, FirstTime);
            WaitAnimationMs (400);
        }

        TestGrid = Grid;
        FillGrid (TestGrid, KParams);

        if (TestGrid != Grid) // show "animation" only if grid has changed
            FillGrid (Grid, KParams);

        DisplayGame(Grid, KParams, Seq, KCurrentAttempts, PointCounter, FirstTime);

        /*find sequences*/
        FindSequenceInColumn(Grid, Seq.Auto.AlignX, KParams);
        FindSequenceInLine(Grid, Seq.Auto.AlignY, KParams);

        if (FindActiveGoldenCandies(Seq, KParams, PointCounter, Grid))
        {
            DisplayGame (Grid, KParams, Seq, KCurrentAttempts, PointCounter, FirstTime);
            system("aplay ../NumberCrush/fichiers/data_files/sounds/golden_candy.wav 2> /dev/null");
        }

        if (Seq.Auto.AlignX.size() == 0 && Seq.Auto.AlignY.size() == 0) // stop "animation" when no more automatically formed sequences are found
        {
            ContinueAnimation = false;
            break;
        }
        CountPoints(Seq, PointCounter, KParams.MapParamUnsigned.at("HowMany")); // counts points due to all automatically formed sequences
    }
}//RunAutomaticGridManaging

/*!
 * \brief Asks to the player coordinates for column.
 * \param[in|out] PlayerMove : all infos about the player's move
 * \param[in] KCharColumnNumber : char value of the last column, used to check if the entered value is correct
 * \param[in] KGrid : grid of the game
 * \param[in] KParams : all game settings
 * \param[in] KSeq : all infos about sequences
 * \param[in] KCurrentAttempts : remaining attempts before the game ends
 * \param[in] KPointCounter : points of the player
 * \param[in] KFirstTime : used to display differents thing before the first move of the player
 * \fn void AskXCoord (MoveInfos            & PlayerMove,
                       const char           & KCharColumnNumber,
                       const CMat           & KGrid,
                       const GameSettings   & KParams,
                       const SequencesInfos & KSeq,
                       const unsigned       & KCurrentAttempts,
                       const unsigned       & KPointCounter,
                       const bool           & KFirstTime)
 */
void AskXCoord (MoveInfos            & PlayerMove,
                const char           & KCharColumnNumber,
                const CMat           & KGrid,
                const GameSettings   & KParams,
                const SequencesInfos & KSeq,
                const unsigned       & KCurrentAttempts,
                const unsigned       & KPointCounter,
                const bool           & KFirstTime)
{
    while (true) // ask X position
    {
        cout << endl << "Selectionnez une case !" << endl << "Colonne : ";
        getline(cin, PlayerMove.Coord);

        if (IsValidLetter(PlayerMove.Coord, 'A', KCharColumnNumber-1)) // induces that number of columns can not be more than 26
        {
            char CoordLetter = toupper(PlayerMove.Coord[0]); // case does not matter
            PlayerMove.Pos.second = CharCoordToUnsigned(CoordLetter); // turn choice into unsigned
            break;
        }

        DisplayGame (KGrid, KParams, KSeq, KCurrentAttempts, KPointCounter, KFirstTime);
        cerr << "SAISIE INVALIDE !";
    }
}//AskXCoord

/*!
 * \brief Asks to the player coordinates for line.
 * \param[in|out] PlayerMove : all infos about the player's move
 * \param[in] KCharLineNumber : char value of the last line, used to check if the entered value is correct
 * \param[in] KGrid : grid of the game
 * \param[in] KParams : all game settings
 * \param[in] KSeq : all infos about sequences
 * \param[in] KCurrentAttempts : remaining attempts before the game ends
 * \param[in] KPointCounter : points of the player
 * \param[in] KFirstTime : used to display differents thing before the first move of the player
 * \fn void AskYCoord (MoveInfos            & PlayerMove,
                       const char           & KCharLineNumber,
                       const CMat           & KGrid,
                       const GameSettings   & KParams,
                       const SequencesInfos & KSeq,
                       const unsigned       & KCurrentAttempts,
                       const unsigned       & KPointCounter,
                       const bool           & KFirstTime)
 */
void AskYCoord (MoveInfos            & PlayerMove,
                const char           & KCharLineNumber,
                const CMat           & KGrid,
                const GameSettings   & KParams,
                const SequencesInfos & KSeq,
                const unsigned       & KCurrentAttempts,
                const unsigned       & KPointCounter,
                const bool           & KFirstTime)
{
    while (true) // ask Y position
    {
        cout << "Ligne : ";
        getline(cin, PlayerMove.Coord);

        if (IsValidLetter(PlayerMove.Coord, 'A', KCharLineNumber-1)) // induces that number of lines can not be more than 26
        {
            char CoordLetter = toupper(PlayerMove.Coord[0]); // case does not matter
            PlayerMove.Pos.first = CharCoordToUnsigned(CoordLetter); // turn choice into unsigned
            if (KGrid [PlayerMove.Pos.first][PlayerMove.Pos.second] != KBlock && KGrid [PlayerMove.Pos.first][PlayerMove.Pos.second] != KWall)
                break; // blocks and walls can not be moved
        }

        DisplayGame (KGrid, KParams, KSeq, KCurrentAttempts, KPointCounter, KFirstTime);
        cout << "Colonne : " << UnsignedCoordToChar(PlayerMove.Pos.second) << endl; // remind entered X value
        cerr << "SAISIE INVALIDE !" << endl;
    }
}//AskYCoord

/*!
 * \brief Asks to the player the direction of the move.
 * \param[in|out] PlayerMove : all infos about the player's move
 * \param[in] KGrid : grid of the game
 * \param[in] KParams : all game settings
 * \param[in] KSeq : all infos about sequences
 * \param[in] KCurrentAttempts : remaining attempts before the game ends
 * \param[in] KPointCounter : points of the player
 * \param[in] KFirstTime : used to display differents thing before the first move of the player
 * \fn void AskDirection (MoveInfos           & PlayerMove,
                         const CMat           & KGrid,
                         const GameSettings   & KParams,
                         const SequencesInfos & KSeq,
                         const unsigned       & KCurrentAttempts,
                         const unsigned       & KPointCounter,
                         const bool           & KFirstTime)
 */
void AskDirection (MoveInfos           & PlayerMove,
                  const CMat           & KGrid,
                  const GameSettings   & KParams,
                  const SequencesInfos & KSeq,
                  const unsigned       & KCurrentAttempts,
                  const unsigned       & KPointCounter,
                  const bool           & KFirstTime)
{
    while (true) // ask direction
    {
        cout << "Case a echanger ? (Haut = Z, Bas = S, Droite = D, Gauche = Q) : ";
        getline(cin, PlayerMove.Direction);

        if (isalpha(PlayerMove.Direction[0]))
        {
            PlayerMove.Dir = toupper(PlayerMove.Direction[0]);
            if ((PlayerMove.Dir == 'Z' && PlayerMove.Pos.first > 0) || // to move up, box must not be at the top of the grid
                (PlayerMove.Dir == 'S' && PlayerMove.Pos.first < KParams.MapParamUnsigned.at("NbOfLines")-1) || // to move down, box must not be at the bottom of the grid
                (PlayerMove.Dir == 'D' && PlayerMove.Pos.second < KParams.MapParamUnsigned.at("NbOfColumns")-1) || // to move right, box must not be at the very right of the grid
                (PlayerMove.Dir == 'Q' && PlayerMove.Pos.second > 0))  // to move left, box must not be at the very left of the grid
                    if (ArrivalBoxValue(KGrid, PlayerMove) != KBlock && ArrivalBoxValue (KGrid, PlayerMove) != KWall)
                        break; // blocks and walls can not be moved
        }

        DisplayGame (KGrid, KParams, KSeq, KCurrentAttempts, KPointCounter, KFirstTime);
        cout << "Colonne : " << UnsignedCoordToChar(PlayerMove.Pos.second) << endl; // remind entered X value
        cout << "Ligne : " << UnsignedCoordToChar(PlayerMove.Pos.first) << endl; // remind entered Y value
        cerr << "SAISIE INVALIDE !" << endl;
    }
}//AskDirection

/*!
 * \brief Asks to the player the coordinates for column and line, and the direction of the move.
 * \param[in|out] PlayerMove : all infos about the player's move
 * \param[in] KCharColumnNumber : char value of the last column, used to check if the entered value is correct
 * \param[in] KCharLineNumber : char value of the last line, used to check if the entered value is correct
 * \param[in] KGrid : grid of the game
 * \param[in] KParams : all game settings
 * \param[in] KSeq : all infos about sequences
 * \param[in] KCurrentAttempts : remaining attempts before the game ends
 * \param[in] KPointCounter : points of the player
 * \param[in] KFirstTime : used to display differents thing before the first move of the player
 * \fn void AskPlayerMoveInfos(MoveInfos            & PlayerMove,
                               const char           & KCharColumnNumber,
                               const char           & KCharLineNumber,
                               const CMat           & KGrid,
                               const GameSettings   & KParams,
                               const SequencesInfos & KSeq,
                               const unsigned       & KCurrentAttempts,
                               const unsigned       & KPointCounter,
                               const bool           & KFirstTime)
 */
void AskPlayerMoveInfos(MoveInfos            & PlayerMove,
                        const char           & KCharColumnNumber,
                        const char           & KCharLineNumber,
                        const CMat           & KGrid,
                        const GameSettings   & KParams,
                        const SequencesInfos & KSeq,
                        const unsigned       & KCurrentAttempts,
                        const unsigned       & KPointCounter,
                        const bool           & KFirstTime)
{
    AskXCoord(PlayerMove,
              KCharColumnNumber,
              KGrid,
              KParams,
              KSeq,
              KCurrentAttempts,
              KPointCounter,
              KFirstTime);

    AskYCoord(PlayerMove,
              KCharLineNumber,
              KGrid,
              KParams,
              KSeq,
              KCurrentAttempts,
              KPointCounter,
              KFirstTime);

    AskDirection(PlayerMove,
                 KGrid,
                 KParams,
                 KSeq,
                 KCurrentAttempts,
                 KPointCounter,
                 KFirstTime);
}//AskPlayerMoveInfos

/*!
 * \brief Plays the game with all sets parameters
 * \param[in|out] Grid : grid of the game
 * \param[in] KParams : all game settings
 * \param[in|out] PointCounter : points of the player
 * \fn void PlayGame(CMat & Grid, const GameSettings & KParams, unsigned & PointCounter)
 */
void PlayGame(CMat & Grid, const GameSettings & KParams, unsigned & PointCounter)
{
    MoveInfos PlayerMove;
    SequencesInfos Seq;

    bool FirstTime = true; // used to display "animation" only from the first move
    bool ContinueAnimation = false; // used to display the "animation" while sequences are found
    bool Remove = true; // used to specify if sequence should be removed or replace by other numbers

    unsigned CurrentAttempts = KParams.MapParamUnsigned.at("Attempts");
    char CharColumnNumber = UnsignedCoordToChar(KParams.MapParamUnsigned.at("NbOfColumns"));
    char CharLineNumber = UnsignedCoordToChar(KParams.MapParamUnsigned.at("NbOfLines"));

    /*stop game when player has no more attemps and no automatically formed sequences are found*/
    while (CurrentAttempts > 0 || (CurrentAttempts == 0 && ContinueAnimation))
    {

        if (FirstTime) // do not display "animations" the first time
            DisplayGame (Grid, KParams, Seq, CurrentAttempts, PointCounter, FirstTime);
                else // if not first time, display "animations"
            RunAutomaticGridManaging(Grid,
                                     Seq,
                                     CurrentAttempts,
                                     PointCounter,
                                     FirstTime,
                                     Remove,
                                     KParams,
                                     ContinueAnimation);

        if (CurrentAttempts != 0) // ask player move only if he still has attemps
        {

            AskPlayerMoveInfos(PlayerMove,
                               CharColumnNumber,
                               CharLineNumber,
                               Grid,
                               KParams,
                               Seq,
                               CurrentAttempts,
                               PointCounter,
                               FirstTime);

            PlayerMove.MovedCaseValue = Grid[PlayerMove.Pos.first][PlayerMove.Pos.second]; // save choosen box value
            PlayerMove.ArrivalPosition = ArrivalBoxPosition(PlayerMove); // save choosen box position

            MakeAMove(Grid, PlayerMove);

            ContinueAnimation = false;

            // if moved case is a bomb, trigger it
            if (PlayerMove.MovedCaseValue == KBomb)
            {
                TriggerBomb(Grid, PlayerMove.Pos, PlayerMove.Dir, PointCounter, KParams);
                DisplayGame(Grid, KParams, Seq, CurrentAttempts, PointCounter, FirstTime);

                system("aplay ../NumberCrush/fichiers/data_files/sounds/bomb.wav 2> /dev/null");
            }

            /*find sequences*/
            FindSequenceInColumn(Grid, Seq.Auto.AlignX, KParams);
            FindSequenceInLine(Grid, Seq.Auto.AlignY, KParams);

            if (FindActiveGoldenCandies(Seq, KParams, PointCounter, Grid))
            {
                DisplayGame (Grid, KParams, Seq, CurrentAttempts, PointCounter, FirstTime);
                system("aplay ../NumberCrush/fichiers/data_files/sounds/golden_candy.wav 2> /dev/null");
            }

            // if moved case is a bomb or sequences are found
            if (PlayerMove.MovedCaseValue == KBomb || (Seq.Auto.AlignX.size() != 0 || Seq.Auto.AlignY.size() != 0)) ContinueAnimation = true;

            /*to display only sequences made by player's move*/
            Seq.Manu.AlignX = Seq.Auto.AlignX;
            Seq.Manu.AlignY = Seq.Auto.AlignY;

            CountPoints(Seq, PointCounter, KParams.MapParamUnsigned.at("HowMany"));

            FirstTime = false;
            --CurrentAttempts;
        }
    }
}//PlayGame

/*!
 * \brief Displays end game informations and updates database.
 * \param[in] KGrid : grid of the game
 * \param[in|out] Params : all game settings
 * \param[in|out] Player : all player infos
 * \param[in] KPointCounter : points of the player
 * \fn void EndGame(const CMat & KGrid, GameSettings & Params, PlayerInfos & Player, const unsigned & KPointCounter)
 */
void EndGame(const CMat & KGrid, GameSettings & Params, PlayerInfos & Player, const unsigned & KPointCounter)
{
    DisplayGrid (KGrid, Params);
    cout << endl << "Tu as utilise tes " << Params.MapParamUnsigned.at("Attempts") << " tentatives !" << endl;

    /*save score if it is the best*/
    if (!AllDirectoriesExists(KDirectoriesName)) exit(2); // avoid the program to crash if a directory has been deleted
    Level::ReadBestScore(Params);
    if (Params.MapLevelInfosUnsigned.size() == 0 || KPointCounter > Params.MapLevelInfosUnsigned.at("BestScore"))
    {
        if (!AllDirectoriesExists(KDirectoriesName)) exit(2); // avoid the program to crash if a directory has been deleted
        Level::SaveScore(Params.LevelScoreFileName, Player.PlayerPseudo, KPointCounter);

        cout << "*NOUVEAU MEILLEUR SCORE*" << endl;
    }

    cout << "Ton score final est de " << KPointCounter;
    if (Params.MapParamUnsigned.at("ScoreMin") != 0) cout << " / " << Params.MapParamUnsigned.at("ScoreMin"); // not displayed for unconnected players
    cout << '.' << endl;

    if (Params.MapParamUnsigned.at("ScoreMin") != 0) // if player is log in
    {
        /*save level if it is completed*/
        if (KPointCounter >= Params.MapParamUnsigned.at("ScoreMin"))
        {
            if (!AllDirectoriesExists(KDirectoriesName)) exit(2); // avoid the program to crash if a directory has been deleted
            Player::SaveLevel(Player, Params.Level);
            cout << "Tu as reussi le niveau " << Params.Level << ", bravo !" << endl;
            system("aplay ../NumberCrush/fichiers/data_files/sounds/win.wav 2> /dev/null");
        }
        else
        {
            cout << "Dommage, tu fera mieux la prochaine fois !" << endl;
            system("aplay ../NumberCrush/fichiers/data_files/sounds/lose.wav 2> /dev/null");
        }
    }
    WaitForEnter();
}//EndGame

/*!
 * \brief Check if there everything the game needs to work correctly.
 * \param[in] KDirectoriesName : vector containing all directories name needed to correctly run the game
 * \fn void LunchGame (const vector <const char *> & KDirectoriesName)
 */
void LunchGame (const vector <const char *> & KDirectoriesName)
{
    if (!AllDirectoriesExists(KDirectoriesName))
    {
       cerr << "Le jeu n'a pas pu etre lance." << endl;
       cout << "Les dossiers suivants manquent :" << endl;

       for (const char * dir : KDirectoriesName) // display DirectoriesName
           cout << dir << endl;

       exit(2);
    }
}//LunchGame

/*!
 * \brief main function : runs the program
 * \return 0
 * \fn int ppal()
 */
int ppal()
{
    LunchGame(KDirectoriesName);

    GameSettings Params;
    PlayerInfos Player;

    bool LeaveGame = false; // used to leave game
    unsigned PointCounter = 0; // points earned by the player

    while(true) // main loop, plays game until player wants to leave
    {
        Menu::LoadMenu (Params, Player, LeaveGame);

        if (LeaveGame) break;

        PointCounter = 0; // reset PointCounter at each game beginning
        CMat Grid (Params.MapParamUnsigned.at("NbOfLines"), CVUInt (Params.MapParamUnsigned.at("NbOfColumns")));

        InitGrid (Grid, Params);

        SetWalls (Grid, Params);

        if (Player.PlayerPseudo == "Bot")
            PlayBot(Params);

        else // if player is not the bot
        {
            PlayGame(Grid, Params, PointCounter);

            EndGame(Grid, Params, Player, PointCounter);
        }
    }//main loop
    return 0;
}//game
