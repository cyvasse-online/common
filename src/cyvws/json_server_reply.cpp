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

#include <cyvws/json_server_reply.hpp>

#include <cyvws/common.hpp>
#include <cyvws/msg.hpp>
#include <cyvws/server_reply.hpp>

namespace cyvws
{
	namespace json
	{
		Json::Value serverReply(unsigned msgID, const Json::Value& replyData)
		{
			Json::Value msg;
			msg[MSG_TYPE]   = MsgType::SERVER_REPLY;
			msg[MSG_ID]     = msgID;
			msg[REPLY_DATA] = replyData;

			return msg;
		}

		Json::Value requestSuccess(unsigned msgID)
		{
			Json::Value replyData;
			replyData[SUCCESS] = true;

			return json::serverReply(msgID, replyData);
		}

		Json::Value requestErr(unsigned msgID, const std::string& error, const std::string& errDetails)
		{
			Json::Value replyData;
			replyData[SUCCESS] = false;
			replyData[ERR_MSG] = error;

			if (!errDetails.empty())
				replyData[ERR_DETAILS] = errDetails;

			return json::serverReply(msgID, replyData);
		}

		Json::Value createGameSuccess(unsigned msgID, const std::string& matchID, const std::string& playerID)
		{
			Json::Value replyData;
			replyData[SUCCESS]   = true;
			replyData[MATCH_ID]  = matchID;
			replyData[PLAYER_ID] = playerID;

			return json::serverReply(msgID, replyData);
		}
	}
}
