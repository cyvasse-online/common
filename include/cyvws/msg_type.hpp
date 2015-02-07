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

#ifndef _CYVWS_MSG_TYPE_HPP_
#define _CYVWS_MSG_TYPE_HPP_

#include "enum_str.hpp"

namespace cyvws
{
	enum class MsgType
	{
		UNDEFINED,
		CHAT_MSG,
		CHAT_MSG_ACK,
		GAME_MSG,
		GAME_MSG_ACK,
		GAME_MSG_ERR,
		NOTIFICATION,
		SERVER_REPLY,
		SERVER_REQUEST,
	};

	ENUM_STR(MsgType, ({
			{MsgType::UNDEFINED, "undefined"},
			{MsgType::CHAT_MSG, "chatMsg"},
			{MsgType::CHAT_MSG_ACK, "chatMsgAck"},
			{MsgType::GAME_MSG, "gameMsg"},
			{MsgType::GAME_MSG_ACK, "gameMsgAck"},
			{MsgType::GAME_MSG_ERR, "gameMsgErr"},
			{MsgType::NOTIFICATION, "notification"},
			{MsgType::SERVER_REPLY, "serverReply"},
			{MsgType::SERVER_REQUEST, "serverRequest"},
		}),
		MsgType::UNDEFINED
	)
}

#endif // _CYVWS_MSG_TYPE_HPP_
