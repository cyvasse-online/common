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

#ifndef _CYVWS_JSON_SERVER_REPLY_HPP_
#define _CYVWS_JSON_SERVER_REPLY_HPP_

#include <json/value.h>

namespace cyvws
{
	namespace json
	{
		Json::Value serverReply(unsigned msgID, const Json::Value& replyData);

		Json::Value requestErr(unsigned msgID, const std::string& error, const std::string& errDetails = {});
		Json::Value initCommSuccess(unsigned msgID);
		Json::Value createGameSuccess(unsigned msgID, const std::string& matchID, const std::string& playerID);
	}
}

#endif // _CYVWS_JSON_SERVER_REPLY_HPP_
