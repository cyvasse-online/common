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

#include <type_traits>
#include <json/value.h>
#include <cyvmath/coordinate.hpp>
#include <cyvmath/piece_type.hpp>
#include <cyvmath/piece.hpp>

namespace cyvws
{
    Json::Value createJsonPiecePosition(cyvmath::PieceType, const cyvmath::Coordinate&);
    Json::Value createJsonPieceMovement(cyvmath::PieceType, const cyvmath::Coordinate& oldPos,
                                        const cyvmath::Coordinate& newPos);

    template<class PieceMap>
    Json::Value createJsonOpeningArray(const PieceMap& pieces)
    {
        static_assert(std::is_convertible<typename PieceMap::key_type, cyvmath::Coordinate>::value,
            "PieceMap has to have a type convertible to cyvmath::Coordinate as key_type");
        static_assert(std::is_convertible<typename PieceMap::mapped_type, std::shared_ptr<cyvmath::Piece>>::value,
            "PieceMap has to have a type convertible to std::shared_ptr<cyvmath::Piece> as mapped_type");

        Json::Value data;

        for(auto&& it : pieces)
        {
            auto pieceTypeStr = PieceTypeToStr(it.second->getType());
            auto coordStr = it.first.toString();

            data[pieceTypeStr].append(coordStr);
        }

        return data;
    }

    Json::Value createJsonPromotion(cyvmath::PieceType origType, cyvmath::PieceType newType);

    // convenience overloads
    Json::Value createJsonPiecePosition(const cyvmath::Piece& piece);
}
