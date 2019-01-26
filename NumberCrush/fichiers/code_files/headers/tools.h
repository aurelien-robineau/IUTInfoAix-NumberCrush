/*!
 * \file tools.h
 * \brief All litle tools used in the program.
 *
 * \version 1.3
 * \date 24/01/2018 17:40:02
 * \author Aurélien ROBINEAU
 */

#ifndef TOOLS_H
#define TOOLS_H

#include <string>

namespace Console {
    /*!
     * \brief Clears the console.
     *
     * Method depends on the OS on wich one program is running
     * \fn void ClearScreen ()
     */
    void ClearScreen ();

    /*!
     * \brief Changes console text and text background color.
     *
     * Method depends on the OS on wich one program is running
     *
     * \param[in] KLinuxTextColor
     * \param[in] KLinuxBackgroundColor
     * \param[in] KWindowsTextColor
     * \param[in] KWindowsBackgroundColor
     * \fn void Color (const unsigned & KLinuxTextColor,
                       const unsigned & KLinuxBackgroundColor,
                       const int      & KWindowsTextColor,
                       const int      & KWindowsBackgroundColor)
     */
    void Color (const unsigned & KLinuxTextColor,
                const unsigned & KLinuxBackgroundColor,
                const int      & KWindowsTextColor,
                const int      & KWindowsBackgroundColor);
}//Console

/*!
 * \brief Returns Letter number value for the grid (A=1, B=2, ...)
 * \param[in] KLetter : char to convert
 * \return unsigned int
 * \fn unsigned CharCoordToUnsigned (const char & KLetter)
 */
unsigned CharCoordToUnsigned (const char & KLetter);

/*!
 * \brief Returns the char that match with Number for the grid (1=A, B=2, ...)
 * \param[in] KNumber : unsigned to convert
 * \return  char
 * \fn char UnsignedCoordToChar (const unsigned & KNumber)
 */
char UnsignedCoordToChar (const unsigned & KNumber);

/*!
 * \brief Replace all '_' by ' ' in String
 * \param String : string to change
 * \fn void ReplaceSpaceByUnderscore (std::string & String)
 */
void ReplaceSpaceByUnderscore (std::string & String);

#endif // TOOLS_H
