/* Copyright 2014 - 2015 Jonas Platte
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

#include <cyvws/json_game_msg.hpp>

#include <stdexcept>
#include <cyvws/common.hpp>
#include <cyvws/msg.hpp>
#include <cyvws/game_msg.hpp>

using namespace std;
using namespace cyvmath;

namespace cyvws
{
	namespace json
	{
		Json::Value piecePosition(PieceType pieceType, Coordinate pos)
		{
			Json::Value data;
			data[PIECE_TYPE] = PieceTypeToStr(pieceType);
			data[POS]        = pos.toString();

			return data;
		}

		PiecePosition piecePosition(const Json::Value& val)
		{
			return {
				StrToPieceType(val[PIECE_TYPE].asString()),
				Coordinate(val[POS].asString())
			};
		}

		Json::Value piecePosition(const Piece& piece)
		{
			if(!piece.getCoord())
				throw invalid_argument("cyvws::json::piecePosition: piece has no valid coordinate");

			return piecePosition(piece.getType(), *piece.getCoord());
		}

		Json::Value pieceMovement(PieceType pieceType, Coordinate oldPos, Coordinate newPos)
		{
			Json::Value data;
			data[PIECE_TYPE] = PieceTypeToStr(pieceType);
			data[OLD_POS]    = oldPos.toString();
			data[NEW_POS]    = newPos.toString();

			return data;
		}

		PieceMovement pieceMovement(const Json::Value& val)
		{
			return {
				StrToPieceType(val[PIECE_TYPE].asString()),
				Coordinate(val[OLD_POS].asString()),
				Coordinate(val[NEW_POS].asString())
			};
		}

		Json::Value openingArray(const OpeningArray& arr)
		{
			Json::Value val;

			for (const auto& it : arr)
			{
				auto& pieceTypeArr = val[PieceTypeToStr(it.first)];

				for (const auto& coord : it.second)
					pieceTypeArr.append(coord.toString());
			}

			return val;
		}

		OpeningArray openingArray(const Json::Value& val)
		{
			OpeningArray arr;

			for (const auto& str : val.getMemberNames())
			{
				auto it = arr.emplace(StrToPieceType(str), set<Coordinate>()).first;

				for (const auto& coord : val[str])
					it->second.emplace(coord.asString());
			}

			return arr;
		}

		Json::Value promotion(PieceType origType, PieceType newType)
		{
			Json::Value data;
			data[ORIG_TYPE] = PieceTypeToStr(origType);
			data[NEW_TYPE]  = PieceTypeToStr(newType);

			return data;
		}

		Promotion promotion(const Json::Value& val)
		{
			return {
				StrToPieceType(val[ORIG_TYPE].asString()),
				StrToPieceType(val[NEW_TYPE].asString())
			};
		}

		Json::Value gameMsg(const string& gameMsgAction, const Json::Value& param)
		{
			Json::Value msg;
			msg[MSG_TYPE] = MsgType::GAME_MSG;
			msg[MSG_DATA][ACTION] = gameMsgAction;
			msg[MSG_DATA][PARAM]  = param;

			return msg;
		}

		Json::Value gameMsgSetOpeningArray(const OpeningArray& arr)
		{ return gameMsg(GameMsgAction::SET_OPENING_ARRAY, openingArray(arr)); }

		Json::Value gameMsgSetIsReady(bool val)
		{ return gameMsg(GameMsgAction::SET_IS_READY, val); }

		Json::Value gameMsgMove(PieceType pieceType, Coordinate oldPos, Coordinate newPos)
		{ return gameMsg(GameMsgAction::MOVE, pieceMovement(pieceType, oldPos, newPos)); }

		Json::Value gameMsgPromote(PieceType origType, PieceType newType)
		{ return gameMsg(GameMsgAction::PROMOTE, promotion(origType, newType)); }
	}
}
