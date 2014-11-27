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

#ifndef _SERVER_MESSAGE_HPP_
#define _SERVER_MESSAGE_HPP_

#include "enum_str.hpp"

enum class Action
{
	UNDEFINED,
	CREATE_GAME,
	JOIN_GAME,
	RESUME_GAME,
	CHAT_MSG
};

enum class Update
{
	UNDEFINED,
	LEAVE_SETUP,
	MOVE_PIECE,
	PROMOTE_PIECE,
	RESIGN
};

enum class GameMode
{
	UNDEFINED,
	DIRECT,
	RANDOM,
	BOT,
	LOCAL
};

ENUM_STR(Action, ({
		{Action::UNDEFINED, "undefined"},
		{Action::CREATE_GAME, "create game"},
		{Action::JOIN_GAME, "join game"},
		{Action::RESUME_GAME, "resume game"},
		{Action::CHAT_MSG, "chat message"}
	}),
	Action::UNDEFINED
)

ENUM_STR(Update, ({
		{Update::UNDEFINED, "undefined"},
		{Update::LEAVE_SETUP, "leave setup"},
		{Update::MOVE_PIECE, "move piece"},
		{Update::PROMOTE_PIECE, "promote piece"},
		{Update::RESIGN, "resign"}
	}),
	Update::UNDEFINED
)

ENUM_STR(GameMode, ({
		{GameMode::UNDEFINED, "undefined"},
		{GameMode::DIRECT, "direct"},
		{GameMode::RANDOM, "random"},
		{GameMode::BOT, "bot"},
		{GameMode::LOCAL, "local"}
	}),
	GameMode::UNDEFINED
)

#endif // _SERVER_MESSAGE_HPP_
