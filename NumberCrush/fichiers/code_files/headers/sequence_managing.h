/*!
 * \file sequence_managing.h
 * \brief All functions linked with sequences.
 *
 * \version 2.1
 * \date 24/01/2018 17:38:36
 * \author Aurélien ROBINEAU
 */

#ifndef SEQUENCE_MANAGING_H
#define SEQUENCE_MANAGING_H

#include "type.h"
#include "struct.h"

/*!
 * \brief Find all sequences in columns and save their lenght and coordinates.
 * \param[in] KGrid : grid of the game
 * \param[in/out] AlignPosX : coordinates and lenght of each sequence found
 * \param[in] KParams : all game settings
 * \fn void FindSequenceInColumn (const CMat & KGrid, CSeqInfos & AlignPosX, const GameSettings & KParams)
 */
void FindSequenceInColumn (const CMat & KGrid, CSeqInfos & AlignPosX, const GameSettings & KParams);

/*!
 * \brief Find all sequences in lines and save their lenght and coordinates.
 * \param[in] KGrid : grid of the game
 * \param[in/out] AlignPosY : coordinates and lenght of each sequence found
 * \param[in] KParams : all game settings
 * \fn void FindSequenceInLine (const CMat & KGrid, CSeqInfos & AlignPosY, const GameSettings & KParams)
 */
void FindSequenceInLine (const CMat & KGrid, CSeqInfos & AlignPosY, const GameSettings & KParams);

/*!
 * \brief Delete (if KRemove is true) all found sequences in columns or fill them with new random values (if KRemove is false)
 * \param[in/out] Grid : grid of the game
 * \param[in] KAlignPosX : coordinates and lenght of each sequence found
 * \param[in] KRemove : true to delete sequences, false to replace sequences with new numbers
 * \param[in] KNbCandies : number of differents types of candy
 * \fn void ChangeInColumn (CMat & Grid, const CSeqInfos & KAlignPosX, const bool & KRemove, const unsigned & KNbCandies)
 */
void ChangeInColumn (CMat & Grid, const CSeqInfos & KAlignPosX, const bool & KRemove, const unsigned & KNbCandies);

/*!
 * \brief Delete (if KRemove is true) all found sequences in lines or fill them with new random values (if KRemove is false)
 * \param[in/out] Grid : grid of the game
 * \param[in] KAlignPosY : coordinates and lenght of each sequence found
 * \param[in] KRemove : true to delete sequences, false to replace sequences with new numbers
 * \param[in] KNbCandies : number of differents types of candy
 * \fn void ChangeInLine (CMat & Grid, const CSeqInfos & KAlignPosY, const bool & KRemove, const unsigned & KNbCandies)
 */
void ChangeInLine (CMat & Grid, const CSeqInfos & KAlignPosY, const bool & KRemove, const unsigned & KNbCandies);

/*!
 * \brief Finds all golden candies included in a sequence of their type, and triggers them.
 * \param[in] KSeq : all infos about sequences
 * \param[in] KParams : all game settings
 * \param[in|out] PointCounter : points of the player
 * \param[in|out] Grid : grid of the game
 * \return true if golden candies are found in sequences
 * \fn bool FindActiveGoldenCandies (const SequencesInfos & KSeq, const GameSettings & KParams, unsigned & PointCounter, CMat & Grid)
 */
bool FindActiveGoldenCandies (const SequencesInfos & KSeq, const GameSettings & KParams, unsigned & PointCounter, CMat & Grid);

#endif // SEQUENCE_MANAGING_H
