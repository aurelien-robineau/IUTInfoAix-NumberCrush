/*!
 * \file boolean_test.h
 * \brief All boolean functions that test if a parameter is <something>
 *
 * \version 1.4
 * \date 24/01/2018 17:00:43
 * \author Aurélien ROBINEAU
 */

#ifndef BOOLEAN_TESTS_H
#define BOOLEAN_TESTS_H

#include "type.h"

#include <string>

/*!
 * \brief Checks if string KData is an unsigned int between Min and Max.
 * \param[in] KData : string to check
 * \param[in] KMin : minimal for for KData
 * \param[in] KMax : maximal value for KData
 * \return true if string KData is an unsigned int between Min and Max.
 * \fn bool IsValidUsignedValue (const std::string & KData, const unsigned & KMin, const unsigned & KMax)
 */
bool IsValidUsignedValue (const std::string & KData, const unsigned & KMin, const unsigned & KMax);

/*!
 * \brief Checks if string KData is char between Min and Max.
 * \param[in] KData : string to check
 * \param[in] KMin : minimal for for KData
 * \param[in] KMax : maximal value for KData
 * \return true if string KData is char between Min and Max.
 * \fn bool IsValidLetter (const std::string & KData, const char & KMin, const char & KMax)
 */
bool IsValidLetter (const std::string & KData, const char & KMin, const char & KMax);

/*!
 * \brief Checks if the box at CPosition is under a wall.
 * \param[in] KGrid : Grid of the game
 * \param[in] KPosition : Position to check
 * \return true if the box at CPosition is under a wall.
 * \fn bool IsUnderAWall (const CMat & KGrid, const CPosition & KPosition)
 */
bool IsUnderAWall (const CMat & KGrid, const CPosition & KPosition);

#endif // BOOLEAN_TESTS_H
