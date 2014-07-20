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

enum MessageType
{
	MESSAGE_UNDEFINED,
	MESSAGE_REQUEST,
	MESSAGE_REPLY,
	MESSAGE_GAME_UPDATE
};

enum ActionType
{
	ACTION_UNDEFINED,
	ACTION_CREATE_GAME,
	ACTION_JOIN_GAME,
	ACTION_RESUME_GAME,
	ACTION_CHAT_MSG
};

enum UpdateType
{
	UPDATE_LEAVE_SETUP,
	UPDATE_MOVE_PIECE,
	UPDATE_RESIGN
};

ENUM_STR(MessageType, ({
	{MESSAGE_UNDEFINED, "undefined"},
	{MESSAGE_REQUEST, "request"},
	{MESSAGE_REPLY, "reply"},
	{MESSAGE_GAME_UPDATE, "game update"}
}))

ENUM_STR(ActionType, ({
	{ACTION_UNDEFINED, "undefined"},
	{ACTION_CREATE_GAME, "create game"},
	{ACTION_JOIN_GAME, "join game"},
	{ACTION_RESUME_GAME, "resume game"},
	{ACTION_CHAT_MSG, "chat message"}
}))

ENUM_STR(UpdateType, ({
	{UPDATE_LEAVE_SETUP, "leave setup"},
	{UPDATE_MOVE_PIECE, "move piece"},
	{UPDATE_RESIGN, "resign"}
}))

#endif // _SERVER_MESSAGE_HPP_
