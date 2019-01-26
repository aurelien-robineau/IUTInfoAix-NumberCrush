/*!
 * \file struct.h
 * \brief All structure declarations.
 *
 * \version 1.2
 * \date 24/01/2018 17:39:55
 * \author Aurélien ROBINEAU
 */

#ifndef STRUCTS_H
#define STRUCTS_H

#include "type.h"

#include <string>
#include <vector>

/*!
 * \brief All global game settings
 * \struct GameSettings
 */
struct GameSettings
{
    unsigned       Level;                  // number of choosen level

    std::string    LevelScoreFileName;     // name of the file which contains the top score of the level
    std::string    LevelConfigFileName;    // name of the file which contains level configuration

    std::map <std::string, unsigned> MapParamUnsigned;                 // contains all unsigned parameters
    std::map <std::string, bool> MapParamBool;                         // contains all boolean parameters
    std::map <std::string, std::vector<CPosition>> MapParamCVPosition; // contains all wall positions

    std::map <std::string, unsigned> MapLevelInfosUnsigned;  // contains all unsigned parameters
    std::map <std::string, std::string> MapLevelInfosString; // contains all unsigned parameters

    const unsigned KNumberOfLevels = 10;   // number of levels

    const unsigned KNbUnsignedParams = 6;  // number of unsigned parameters
    const unsigned KNbBoolParams = 4;      // number of boolean parameters
    const unsigned KNbVCPosParams = 1;     // number of vector<CPosition> parameters

    const unsigned KNbBestScoreParams = 2; // number of parameters for level top score
};//GameSettings

/*!
 * \brief All infos about the player
 * \struct PlayerInfos
 */
struct PlayerInfos
{
    std::string PlayerPseudo = "Unconnected",   // pseudo of the current player
                PlayerFileName; // name of the file which contains player infos

    std::map <std::string, unsigned> MapPlayerUnsigned; // contains all unsigned parameters

    const unsigned KNbUnsignedParams = 1; // number of unsigned parameters
};//PlayerInfos

/*!
 * \brief All authorized key for each map
 * \struct AuthorizedKey
 */
struct AuthorizedKey {
    const std::vector <std::string> VParamUnsigned {"NbOfColumns", "NbOfLines", "HowMany", "NbCandies", "Attempts", "ScoreMin"};
    const std::vector <std::string> VParamBool {"BombsActivated", "JokersActivated", "BlocksActivated", "GoldenCandiesActivated"};
    const std::vector <std::string> VParamCVPosition {"WallPositions"};

    const std::vector <std::string> VPlayerUnsigned {"HigherLevel"};

    const std::vector <std::string> VLevelInfosUnsigned {"BestScore"};
    const std::vector <std::string> VLevelInfosString {"Player"};
};//AuthorizedKey

const AuthorizedKey KAuthorizedKey;

/*!
 * \brief All player's move infos
 * \struct MoveInfos
 */
struct MoveInfos
{
    std::string Coord,              // used to check if the entered value is a coordinate
                Direction;          // used to check if the entered value is a direction
    CPosition   Pos,                // coordinates of the selected candy
                ArrivalPosition;    // arrival box position
    unsigned    MovedCaseValue = 0; // number of the selected candy (used to trigger bombs)
    char        Dir;                // direction of the movement
};//MoveInfos

/*!
 * \brief All infos about found sequences
 * \struct SequencesInfos
 */
struct SequencesInfos
{
    /*!
     * \brief Infos about found sequences
     * \struct Automatic
     */
    struct Automatic
    {
        CSeqInfos AlignX; // contains the coordinates of the beggining of each column sequence and their size
        CSeqInfos AlignY; // contains the coordinates of the beggining of each line sequence and their size
    };//SequenceInfos.Auto

    /*!
     * \brief Infos about the sequences directly formed by the player's move.
     * \struct Manual
     */
    struct Manual
    {
        CSeqInfos AlignX; // allows to display only column sequences deleted by player's move
        CSeqInfos AlignY; // allows to display only line sequences deleted by player's move
    };//SequenceInfos.Manual

    Automatic Auto;
    Manual Manu;
};//SequencesInfos

#endif // STRUCTS_H
