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

#include <cyvmath/mikelepage/bearing_table.hpp>

#include <vector>

namespace cyvmath
{
    namespace mikelepage
    {
        void BearingTable::addCanReach(const Piece* piece)
        {
            assert(piece);
            assert(piece->getType() != PieceType::DRAGON);
            assert(piece->getType() != PieceType::MOUNTAIN);

            auto reachableOpPieces = piece->getReachableOpponentPieces();

            if(!reachableOpPieces.empty())
            {
                auto res = _canReach.emplace(piece, std::set<const Piece*>());
                assert(res.second);

                std::set<const Piece*>& selfCanReach = res.first->second;

                for(const Piece* opPiece : reachableOpPieces)
                {
                    auto res = selfCanReach.insert(opPiece);
                    assert(res.second);

                    auto opPieceIt = _canBeReachedBy.find(opPiece);
                    if(opPieceIt == _canBeReachedBy.end())
                    {
                        auto res = _canBeReachedBy.emplace(opPiece, std::set<const Piece*>{piece});
                        assert(res.second);
                    }
                    else
                    {
                        auto res = opPieceIt->second.insert(piece);
                        assert(res.second);
                    }
                }
            }
        }

        void BearingTable::addCanBeReachedBy(const Piece* piece)
        {
            assert(piece);
            assert(piece->getType() != PieceType::MOUNTAIN);

            for(auto it : _pieceMap)
            {
                Piece* opPiece = it.second.get();

                if(opPiece->getColor() == piece->getColor() ||
                   opPiece->getType() == PieceType::DRAGON ||
                   opPiece->getType() == PieceType::MOUNTAIN)
                    continue;

                if(opPiece->canReach(*piece->getCoord()))
                {
                    auto otherPieceIt = _canReach.find(opPiece);
                    if(otherPieceIt == _canReach.end())
                    {
                        auto res = _canReach.emplace(opPiece, std::set<const Piece*>{piece});
                        assert(res.second);
                    }
                    else
                    {
                        auto res = otherPieceIt->second.insert(piece);
                        assert(res.second);
                    }

                    auto pieceIt = _canBeReachedBy.find(piece);
                    if(pieceIt == _canBeReachedBy.end())
                    {
                        auto res = _canBeReachedBy.emplace(piece, std::set<const Piece*>{opPiece});
                        assert(res.second);
                    }
                    else
                    {
                        auto res = pieceIt->second.insert(opPiece);
                        assert(res.second);
                    }
                }
            }
        }

        bool BearingTable::canTake(const Piece* atkPiece, const Piece* defPiece) const
        {
            assert(atkPiece);
            assert(defPiece);
            assert(defPiece->getType() != PieceType::MOUNTAIN);

            uint_least8_t defenseTier = defPiece->getEffectiveDefenseTier();

            if(atkPiece->getBaseTier() >= defenseTier)
                return true;

            bool haveKing = (atkPiece->getType() == PieceType::KING);

            uint_least8_t maxAllowedTier = haveKing ? 3 : atkPiece->getBaseTier();
            uint_least8_t maxTier = 1;

            auto defPieceIt = _canBeReachedBy.find(defPiece);

            if(defPieceIt == _canBeReachedBy.end())
                return false;

            std::map<uint_least8_t, uint_least8_t> flankingTiers {
                {1, 0},
                {2, 0},
                {3, 0}
            };

            for(const Piece* piece : defPieceIt->second)
            {
                auto baseTier = piece->getBaseTier();

                if(baseTier > maxAllowedTier)
                    continue;

                if(piece->getType() == PieceType::KING)
                {
                    // king will count as maxTier after the loop
                    haveKing = true;
                    continue;
                }

                if(baseTier > maxTier)
                    maxTier = baseTier;

                ++flankingTiers[baseTier];
            }

            if(haveKing)
                ++flankingTiers[maxTier];

            for(uint_least8_t i = 1; i < maxTier; ++i)
                flankingTiers[i+1] += flankingTiers[i] / 2;

            uint_least8_t attackTier = maxTier + flankingTiers[maxTier] - 1;

            return attackTier >= defenseTier;
        }

        void BearingTable::init()
        {
            for(auto it : _pieceMap)
            {
                if(it.second->getType() != PieceType::MOUNTAIN &&
                   it.second->getType() != PieceType::DRAGON)
                    addCanReach(it.second.get());
            }
        }

        void BearingTable::add(const Piece* piece)
        {
            if(piece->getType() != PieceType::DRAGON)
                addCanReach(piece);

            addCanBeReachedBy(piece);
        }

        void BearingTable::remove(const Piece* piece)
        {
            assert(piece);

            auto it1 = _canReach.find(piece);
            auto it2 = _canBeReachedBy.find(piece);

            if(it1 != _canReach.end())
            {
                for(auto it1Piece : it1->second)
                {
                    auto it = _canBeReachedBy.find(it1Piece);
                    assert(it != _canBeReachedBy.end());

                    auto pieceRefIt = it->second.find(piece);
                    assert(pieceRefIt != it->second.end());
                    it->second.erase(pieceRefIt);
                }

                _canReach.erase(it1);
            }

            if(it2 != _canBeReachedBy.end())
            {
                for(auto it2Piece : it2->second)
                {
                    auto it = _canReach.find(it2Piece);
                    assert(it != _canReach.end());

                    auto pieceRefIt = it->second.find(piece);
                    assert(pieceRefIt != it->second.end());
                    it->second.erase(pieceRefIt);
                }

                _canBeReachedBy.erase(it2);
            }
        }

        void BearingTable::update(const Piece* piece)
        {
            remove(piece);
            add(piece);
        }

        void BearingTable::clear()
        {
            _canReach.clear();
            _canBeReachedBy.clear();
        }
    }
}
