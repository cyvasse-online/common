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

namespace cyvws
{
	namespace json
	{
		Json::Value piecePosition(PieceType pieceType, HexCoordinate<6> pos)
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
				HexCoordinate<6>(val[POS].asString())
			};
		}

		Json::Value piecePosition(const Piece& piece)
		{
			if(!piece.getCoord())
				throw invalid_argument("cyvws::json::piecePosition: piece has no valid coordinate");

			return piecePosition(piece.getType(), *piece.getCoord());
		}

		Json::Value movement(PieceType pieceType, HexCoordinate<6> oldPos, HexCoordinate<6> newPos)
		{
			Json::Value data;
			data[PIECE_TYPE] = PieceTypeToStr(pieceType);
			data[OLD_POS]    = oldPos.toString();
			data[NEW_POS]    = newPos.toString();

			return data;
		}

		PieceMovement movement(const Json::Value& val)
		{
			return {
				StrToPieceType(val[PIECE_TYPE].asString()),
				HexCoordinate<6>(val[OLD_POS].asString()),
				HexCoordinate<6>(val[NEW_POS].asString())
			};
		}

		Json::Value moveCapture(PieceType atkPT, HexCoordinate<6> oldPos, HexCoordinate<6> newPos,
		                        PieceType defPT, HexCoordinate<6> defPiecePos)
		{
			Json::Value data;
			data[ATK_PIECE][PIECE_TYPE] = PieceTypeToStr(atkPT);
			data[ATK_PIECE][OLD_POS]    = oldPos.toString();
			data[ATK_PIECE][NEW_POS]    = newPos.toString();
			data[DEF_PIECE][PIECE_TYPE] = PieceTypeToStr(defPT);
			data[DEF_PIECE][POS]        = defPiecePos.toString();

			return data;
		}

		MoveCapture moveCapture(const Json::Value& val)
		{
			return {
				StrToPieceType(val[ATK_PIECE][PIECE_TYPE].asString()),
				HexCoordinate<6>(val[ATK_PIECE][OLD_POS].asString()),
				HexCoordinate<6>(val[ATK_PIECE][NEW_POS].asString()),
				StrToPieceType(val[DEF_PIECE][PIECE_TYPE].asString()),
				HexCoordinate<6>(val[DEF_PIECE][POS].asString())
			};
		}

		Json::Value pieceMap(const PieceMap& map)
		{
			Json::Value val;

			for (const auto& it : map)
			{
				auto& pieceTypeArr = val[PieceTypeToStr(it.first)];

				for (const auto& coord : it.second)
					pieceTypeArr.append(coord.toString());
			}

			return val;
		}

		PieceMap pieceMap(const Json::Value& val)
		{
			PieceMap map;

			for (const auto& str : val.getMemberNames())
			{
				auto it = map.emplace(StrToPieceType(str), set<HexCoordinate<6>>()).first;

				for (const auto& coord : val[str])
					it->second.emplace(coord.asString());
			}

			return map;
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

			if(!param.isNull())
				msg[MSG_DATA][PARAM]  = param;

			return msg;
		}

		Json::Value gameMsgSetOpeningArray(const PieceMap& map)
		{ return gameMsg(GameMsgAction::SET_OPENING_ARRAY, pieceMap(map)); }

		Json::Value gameMsgSetIsReady()
		{ return gameMsg(GameMsgAction::SET_IS_READY, Json::Value()); }

		Json::Value gameMsgMove(PieceType pieceType, HexCoordinate<6> oldPos, HexCoordinate<6> newPos)
		{ return gameMsg(GameMsgAction::MOVE, movement(pieceType, oldPos, newPos)); }

		Json::Value gameMsgMoveCapture(PieceType atkPT, HexCoordinate<6> oldPos, HexCoordinate<6> newPos,
		                               PieceType defPT, HexCoordinate<6> defPiecePos)
		{ return gameMsg(GameMsgAction::MOVE_CAPTURE, moveCapture(atkPT, oldPos, newPos, defPT, defPiecePos)); }

		Json::Value gameMsgPromote(PieceType origType, PieceType newType)
		{ return gameMsg(GameMsgAction::PROMOTE, promotion(origType, newType)); }
	}
}
