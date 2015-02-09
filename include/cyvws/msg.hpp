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

#ifndef _CYVWS_MSG_HPP_
#define _CYVWS_MSG_HPP_

#include <string>

namespace cyvws
{
	const std::string
		MSG_TYPE = "msgType",
		MSG_ID   = "msgID";

	namespace MsgType
	{
		const std::string
			CHAT_MSG       = "chatMsg",
			CHAT_MSG_ACK   = "chatMsgAck",
			GAME_MSG       = "gameMsg",
			GAME_MSG_ACK   = "gameMsgAck",
			GAME_MSG_ERR   = "gameMsgErr",
			NOTIFICATION   = "notification",
			SERVER_REPLY   = "serverReply",
			SERVER_REQUEST = "serverRequest";
	}
}

#endif // _CYVWS_MSG_HPP_
