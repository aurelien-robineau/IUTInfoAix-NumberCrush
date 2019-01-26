/*!
 * \file grid_managing.h
 * \brief All functions used to manage the game grid.
 *
 * \version 1.3
 * \date 24/01/2018 17:31:12
 * \author Aurélien ROBINEAU
 */

#ifndef GRID_MANAGING_H
#define GRID_MANAGING_H

#include "type.h"
#include "struct.h"

/*!
 * \brief Makes a new grid without sequences.
 * \param[in/out] Grid : grid of the game
 * \param[in] KParams : all game settings
 * \fn void InitGrid (CMat & Grid, const GameSettings & KParams)
 */
void InitGrid (CMat & Grid, const GameSettings & KParams);

/*!
 * \brief Set each wall of the selected level
 * \param[in/out] Grid : grid of the game
 * \param[in] KParams : all game settings
 * \fn void SetWalls (CMat & Grid, const GameSettings & KParams)
 */
void SetWalls (CMat & Grid, const GameSettings & KParams);

/*!
 * \brief Returns the value of the box on wich one the selected box will arrive.
 * \param[in] KGrid : grid of the game
 * \param[in] KMove : all infos about the move to check
 * \return box value
 * \fn unsigned ArrivalBoxValue (const CMat & KGrid, const MoveInfos & KMove)
 */
unsigned ArrivalBoxValue (const CMat & KGrid, const MoveInfos & KMove);

/*!
 * \brief Returns the position of the box on wich one the selected box will arrive.
 * \param[in] KMove : all infos about the move to check
 * \return box position
 * \fn CPosition ArrivalBoxPosition (const MoveInfos & KMove)
 */
CPosition ArrivalBoxPosition (const MoveInfos & KMove);

/*!
 * \brief Makes go up the empty boxes while they can.
 * \param[in|out] Grid : grid of the game
 * \param[in] KParams : all game settings
 * \fn void GoUpEmptyBoxes (CMat & Grid, const GameSettings & KParams)
 */
void GoUpEmptyBoxes (CMat & Grid, const GameSettings & KParams);

/*!
 * \brief Replace all empty boxes wich are not under a wall with ramdom values.
 * \param[in|out] Grid : grid of the game
 * \param[in] KParams : all game settings
 * \fn void FillGrid (CMat & Grid, const GameSettings & KParams)
 */
void FillGrid (CMat & Grid, const GameSettings & KParams);

/*!
 * \brief Displays the grid with colors and character for special boxes
 * \param[in] KGrid : grid of the game
 * \param[in] KParams : all game settings
 * \fn void DisplayGrid (const CMat & KGrid, const GameSettings & KParams)
 */
void DisplayGrid (const CMat & KGrid, const GameSettings & KParams);

/*!
 * \brief Delete all box worth Value, and add points for each deleted box.
 * \param[in|out] Grid : grid of the game
 * \param[in] KParams : all game settings
 * \param[in|out] PointCounter : points of the player
 * \param[in] KValue : value to delete
 * \fn void DeleteCandies (CMat & Grid, const GameSettings & KParams, unsigned & PointCounter, const unsigned & KValue)
 */
void DeleteCandies (CMat & Grid, const GameSettings & KParams, unsigned & PointCounter, const unsigned & KValue);

#endif // GRID_MANAGING_H
