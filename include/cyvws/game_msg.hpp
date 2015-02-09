/* Copyright 2015 Jonas Platte
 *
 * This file is part of Cyvasse Online.
 *
 * Cyvasse Online is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Affero General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * Cyvasse Online is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _CYVWS_GAME_MSG_HPP_
#define _CYVWS_GAME_MSG_HPP_

#include <string>

namespace cyvws
{
	const std::string
		NEW_POS    = "newPos",
		NEW_TYPE   = "newType",
		OLD_POS    = "oldPos",
		ORIG_TYPE  = "origType",
		PIECE_TYPE = "pieceType";

	namespace GameMsgAction
	{
		const std::string
			END_TURN          = "endTurn",
			MOVE              = "move",
			MOVE_CAPTURE      = "moveCapture",
			PROMOTE           = "promote",
			RESIGN            = "resign",
			SET_IS_READY      = "setIsReady",
			SET_OPENING_ARRAY = "setOpeningArray";
	}
}

#endif // _CYVWS_GAME_MSG_HPP_
