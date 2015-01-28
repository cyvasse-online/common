/* Copyright 2014 Jonas Platte
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

#include "enum_str.hpp"

namespace cyvws
{
	enum class GameMsgAction
	{
		UNDEFINED,
		END_TURN,
		MOVE,
		MOVE_CAPTURE,
		PROMOTE,
		RESIGN,
		SET_IS_READY,
		SET_OPENING_ARRAY,
	};

	ENUM_STR(GameMsgAction, ({
			{GameMsgAction::UNDEFINED, "undefined"},
			{GameMsgAction::END_TURN, "endTurn"},
			{GameMsgAction::MOVE, "move"},
			{GameMsgAction::MOVE_CAPTURE, "moveCapture"},
			{GameMsgAction::PROMOTE, "promote"},
			{GameMsgAction::RESIGN, "resign"},
			{GameMsgAction::SET_IS_READY, "setIsReady"},
			{GameMsgAction::SET_OPENING_ARRAY, "setOpeningArray"},
		}),
		GameMsgAction::UNDEFINED
	)
}
