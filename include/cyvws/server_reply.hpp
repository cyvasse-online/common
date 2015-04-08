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

#ifndef _CYVWS_SERVER_REPLY_HPP_
#define _CYVWS_SERVER_REPLY_HPP_

#include <string>

namespace cyvws
{
	const std::string
		GAME_STATUS = "gameStatus",
		REPLY_DATA  = "replyData";

	const std::string
		PIECE_POSITIONS = "piecePositions",
		SETUP           = "setup",
		SUCCESS         = "success";

	namespace ServerReplyErrMsg
	{
		const std::string
			CONN_IN_USE         = "connInUse",
			DIFF_MAJOR_PROT_V   = "differingMajorProtVersion",
			GAME_EMPTY          = "gameEmpty",
			GAME_FULL           = "gameFull",
			GAME_NOT_FOUND      = "gameNotFound",
			LIST_DOES_NOT_EXIST = "listDoesNotExist",
			NOT_IN_GAME         = "notInGame";
	}
}

#endif // _CYVWS_SERVER_REPLY_HPP_
