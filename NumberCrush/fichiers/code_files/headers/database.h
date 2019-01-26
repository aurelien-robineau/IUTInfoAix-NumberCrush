/*!
 * \file database.h
 * \brief All functions that are used to read or write in files.
 * It can save or load parameters, or test if files exists.
 *
 * \version 2.1
 * \date 24/01/2018 17:28:35
 * \author Aurélien ROBINEAU
 */

#ifndef DATABASE_H
#define DATABASE_H

#include "struct.h"

#include <string>
#include <vector>

namespace Level {
    /*!
     * \brief Save the player score as the best score for the current level.
     * \param[in] KScoreFileName : name of the file wich contains the best score of the current level
     * \param[in] KPlayerPseudo : pseudo of the current player
     * \param[in] KPoints : score to save
     * \fn void SaveScore (const std::string & KScoreFileName, const std::string & KPlayerPseudo, const unsigned & KPoints)
     */
    void SaveScore (const std::string & KScoreFileName, const std::string & KPlayerPseudo, const unsigned & KPoints);

    /*!
     * \brief Read the best score saved for the current level.
     * \param[in|out] Params : all game settings
     * \fn void ReadBestScore (GameSettings & Params)
     */
    void ReadBestScore (GameSettings & Params);

    /*!
     * \brief Load all the chosen level parameters.
     * \param[in|out] Params : all game settings
     * \fn void LoadLevelParams (GameSettings & Params)
     */
    void LoadLevelParams (GameSettings & Params);
}//namespace Level

namespace Player {
    /*!
     * \brief If the current level is the higher the player can plays on,
     * save the next level as the higher level reached.
     * \param[in|out] Player : all player informations
     * \param[in] KCurrentLevel : current running level
     * \fn void SaveLevel (PlayerInfos & Player, const unsigned & KCurrentLevel)
     */
    void SaveLevel (PlayerInfos & Player, const unsigned & KCurrentLevel);

    /*!
     * \brief Create a the file KPlayerFileName.
     * It is a profile for the specified player containing a higher level reached to 1.
     * \param[in] KPlayerFileName : name of the file containing infos about the current player
     * \fn void CreateProfile (const std::string & KPlayerFileName)
     */
    void CreateProfile (const std::string & KPlayerFileName);

    /*!
     * \brief Load the profile of the specified player.
     * \param[in|out] Player : all player informations
     * \param[in] KParams : all game settings
     * \fn void LoadProfile (PlayerInfos & Player, const GameSettings & KParams)
     */
    void LoadProfile (PlayerInfos & Player, const GameSettings & KParams);
}//Player

/*!
 * \brief Checks if all the directories listed in KDirectoriesName exist.
 * \param[in] KDirectoriesName : vector containing all directories name needed to correctly run the game
 * \return true if all the directories listed in KDirectoriesName exist.
 * \fn bool AllDirectoriesExists (const std::vector<const char *> & KDirectoriesName)
 */
bool AllDirectoriesExists (const std::vector<const char *> & KDirectoriesName);

/*!
 * \brief Display the file KFic.
 * \param[in] KFil : file to display
 * \fn void ReadFile(const std::string & KFil)
 */
void ReadFile(const std::string & KFil);

#endif // DATABASE_H
