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

#include <cyvws/json_game_msg.hpp>

#include <stdexcept>
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
			if(pieceType == PieceType::UNDEFINED)
				throw invalid_argument("cyvws::json::piecePosition: piece type is undefined");

			Json::Value data;
			data["pieceType"] = PieceTypeToStr(pieceType);
			data["pos"]	      = pos.toString();

			return data;
		}

		PiecePosition piecePosition(const Json::Value& val)
		{
			return {
				StrToPieceType(val["pieceType"].asString()),
				Coordinate(val["pos"].asString())
			};
		}

		Json::Value pieceMovement(PieceType pieceType, Coordinate oldPos, Coordinate newPos)
		{
			if(pieceType == PieceType::UNDEFINED)
				throw invalid_argument("cyvws::json::pieceMovement: pieceType is undefined");

			Json::Value data;
			data["pieceType"] = PieceTypeToStr(pieceType);
			data["oldPos"]    = oldPos.toString();
			data["newPos"]    = newPos.toString();

			return data;
		}

		PieceMovement pieceMovement(const Json::Value& val)
		{
			return {
				StrToPieceType(val["pieceType"].asString()),
				Coordinate(val["oldPos"].asString()),
				Coordinate(val["newPos"].asString())
			};
		}

		Json::Value promotion(PieceType origType, PieceType newType)
		{
			Json::Value data;
			data["origType"] = PieceTypeToStr(origType);
			data["newType"]  = PieceTypeToStr(newType);

			return data;
		}

		Promotion promotion(const Json::Value& val)
		{
			return {
				StrToPieceType(val["origType"].asString()),
				StrToPieceType(val["newType"].asString())
			};
		}

		Json::Value piecePosition(const Piece& piece)
		{
			if(!piece.getCoord())
				throw invalid_argument("cyvws::json::piecePosition: piece has coordinate");

			return piecePosition(piece.getType(), *piece.getCoord());
		}

		Json::Value gameMsg(const string& gameMsgAction, Json::Value param)
		{
			Json::Value msg;
			msg["msgType"] = MsgType::GAME_MSG;
			msg["msgData"]["action"] = gameMsgAction;
			msg["msgData"]["param"]  = param;

			return msg;
		}

		Json::Value gameMsgSetIsReady(bool val)
		{ return gameMsg(GameMsgAction::SET_IS_READY, val); }

		Json::Value gameMsgMove(PieceType pieceType, Coordinate oldPos, Coordinate newPos)
		{ return gameMsg(GameMsgAction::MOVE, pieceMovement(pieceType, oldPos, newPos)); }

		Json::Value gameMsgPromote(PieceType origType, PieceType newType)
		{ return gameMsg(GameMsgAction::PROMOTE, promotion(origType, newType)); }
	}
}
