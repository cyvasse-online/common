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

#ifndef _CYVWS_JSON_GAME_MSG_HPP_
#define _CYVWS_JSON_GAME_MSG_HPP_

#include <map>
#include <set>
#include <json/value.h>
#include <cyvasse/coordinate.hpp>
#include <cyvasse/piece_type.hpp>
#include <cyvasse/piece.hpp>

namespace cyvws
{
	struct PiecePosition
	{
		cyvasse::PieceType pieceType;
		cyvasse::Coordinate pos;
	};

	struct PieceMovement
	{
		cyvasse::PieceType pieceType;
		cyvasse::Coordinate oldPos;
		cyvasse::Coordinate newPos;
	};

	struct MoveCapture
	{
		cyvasse::PieceType atkPT;
		cyvasse::Coordinate oldPos;
		cyvasse::Coordinate newPos;

		cyvasse::PieceType defPT;
		cyvasse::Coordinate defPiecePos;
	};

	struct Promotion
	{
		cyvasse::PieceType origType;
		cyvasse::PieceType newType;
	};

	typedef std::map<cyvasse::PieceType, std::set<cyvasse::Coordinate>> PieceMap;

	namespace json
	{
		using namespace std;
		using namespace cyvasse;

		Json::Value piecePosition(PieceType pieceType, Coordinate pos);
		PiecePosition piecePosition(const Json::Value&);
		Json::Value piecePosition(const Piece&); // convenience overload

		Json::Value movement(PieceType pieceType, Coordinate oldPos, Coordinate newPos);
		PieceMovement movement(const Json::Value&);

		Json::Value moveCapture(PieceType atkPT, Coordinate oldPos, Coordinate newPos, PieceType defPT, Coordinate defPiecePos);
		MoveCapture moveCapture(const Json::Value&);

		Json::Value pieceMap(const PieceMap&);
		template <class piece_t> // convenience overload
		Json::Value pieceMap(const map<Coordinate, piece_t>&);
		PieceMap pieceMap(const Json::Value&);

		Json::Value promotion(PieceType origType, PieceType newType);
		Promotion promotion(const Json::Value&);

		// complete message assembling

		Json::Value gameMsg(const string& gameMsgAction, const Json::Value& param);

		Json::Value gameMsgSetOpeningArray(const PieceMap&);
		template <class piece_t> // convenience overload
		Json::Value gameMsgSetOpeningArray(const map<Coordinate, piece_t>& pieces);

		Json::Value gameMsgSetIsReady();
		Json::Value gameMsgMove(PieceType pieceType, Coordinate oldPos, Coordinate newPos);
		Json::Value gameMsgMoveCapture(PieceType atkPT, Coordinate oldPos, Coordinate newPos, PieceType defPT, Coordinate defPiecePos);
		Json::Value gameMsgPromote(PieceType origType, PieceType newType);
	}
}

#include "json_game_msg.ipp"

#endif // _CYVWS_JSON_GAME_MSG_HPP_
