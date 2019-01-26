/*!
 * \file player_actions.h
 * \brief All functions corresponding to actions that are directly linked to the player.
 *
 * \version 1.3
 * \date 24/01/2018 17:35:45
 * \author Aurélien ROBINEAU
 */

#ifndef PLAYER_ACTIONS_H
#define PLAYER_ACTIONS_H

#include "type.h"
#include "struct.h"

/*!
 * \brief Makes bomb explosed.
 * It deletes each box wich are on the line where the bomb is moved, between the walls surrounding the bomb.
 * Walls are not deleted.
 * \param[in|out] Grid : grid of the game
 * \param[in] KBombPos : position of the bomb
 * \param[in] KDir : direction of the move
 * \param[in|out] PointCounter : points of the player
 * \param[in] KParams : all game settings
 * \fn void TriggerBomb (CMat & Grid, const CPosition & KBombPos, const char & KDir, unsigned & PointCounter, const GameSettings & KParams)
 */
void TriggerBomb (CMat & Grid, const CPosition & KBombPos, const char & KDir, unsigned & PointCounter, const GameSettings & KParams);

/*!
 * \brief Swap the selected box with the one next to it in the specified direction.
 * \param[in|out] Grid : grid of the game
 * \param[in] KMove : all move informations
 * \fn void MakeAMove (CMat & Grid, const MoveInfos & KMove
 */
void MakeAMove (CMat & Grid, const MoveInfos & KMove);

/*!
 * \brief Wait for the player to press 'Enter'
 * \fn void WaitForEnter()
 */
void WaitForEnter();

#endif // PLAYER_ACTIONS_H
