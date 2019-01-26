/*!
 * \file bot.h
 * \brief Contains the functions to run the bot.
 * This one play as much games as we want by itself,
 * Bot has been made to see the average score a novice player can make for each level.
 * If you choose to play only 1 game, you can see the bot play.
 *
 * -# Bot operation
 * \n Bot plays the best move possible, and if there is no possible move, plays a random one.
 * Sequences made randomly when the grid is filled are count only when the bot makes a move again.
 * That is why chain reactions are impossible with the bot.
 *
 * -# Bot making
 * \n Bot is has been coded quikly and roughly. It is a total mess.
 * Many things may not be optimize at all, and it can be difficult to understand this part of the program.
 * Please do not judge us on this part.
 *
 * \version 1.2
 * \date 24/01/2018 17:24:41
 * \author Aurélien ROBINEAU
 */

#ifndef BOT_H
#define BOT_H

#include "struct.h"

/*!
 * \brief Do all bot stuff, such as unch Bot menu, play the games...
 * \param[in] KParams : all game parameters
 * \fn void PlayBot(const GameSettings & KParams)
 */
void PlayBot(const GameSettings & KParams);

#endif // BOT_H
