/*!
 * \file type.h
 * \brief All type definitions.
 *
 * \version 1.1
 * \date 24/01/2018 17:42:29
 * \author Aurélien ROBINEAU
 */

#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <vector>
#include <map>

/*!
 * \brief A vector of unsigned int
 */
typedef std::vector <unsigned> CVUInt;

/*!
 * \brief A type representing the grid (a matrix)
 */
typedef std::vector <CVUInt> CMat;

/*!
 * \brief A type representing a position in the grid
 */
typedef std::pair <unsigned, unsigned> CPosition;

/*!
 * \brief A type containing the coordinates and lenght of all sequences found
 */
typedef std::vector <std::pair<CPosition, unsigned>> CSeqInfos;

#endif // TYPEDEF_H
