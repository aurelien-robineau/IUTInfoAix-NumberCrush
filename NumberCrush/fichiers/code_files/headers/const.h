/*!
 * \file const.h
 * \brief All constants used everywhere in the program
 *
 * \version 1.1
 * \date 26/01/2018 10:08:02
 * \author Aurélien ROBINEAU
 */

#ifndef CONST_H
#define CONST_H

#include <vector>

/*Value of special candies*/

/*!
 * \brief empty boxes
 */
const unsigned KEmpty = 0 ;

/*!
 * \brief bombs (delete all the arrival line when moved)
 */
const unsigned KBomb = 10;

/*!
 * \brief jokers (can form sequences with any candy)
 */
const unsigned KJoker = 11;

/*!
 * \brief blocks (can not for sequences and can not be moved by the player)
 */
const unsigned KBlock = 12;

/*!
 * \brief walls (blocks that do not move when empty boxes go up).
 * It prevents the candies above to fall (empty boxes under walls are not filled)
 */
const unsigned KWall = 13;

/*!
 * \brief golden candies have for value their type value (between 1 and 9) + 20
 * A golden candie is triggered when it is include in a sequence of it's type.
 * That deletes each candy of this type in the grid.
 */
const unsigned KGold = 20;

/*All directories that have to exist*/
const std::vector <const char *> KDirectoriesName = {"../NumberCrush/fichiers/data_files/players",
                                                     "../NumberCrush/fichiers/data_files/levels",
                                                     "../NumberCrush/fichiers/data_files/levels/config",
                                                     "../NumberCrush/fichiers/data_files/levels/top_score",
                                                     "../NumberCrush/fichiers/data_files/sounds",
                                                     "../NumberCrush/fichiers/data_files/docs"};

#endif // CONST_H
