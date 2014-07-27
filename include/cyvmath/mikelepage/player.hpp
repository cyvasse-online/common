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

#ifndef _CYVMATH_MIKELEPAGE_PLAYER_HPP_
#define _CYVMATH_MIKELEPAGE_PLAYER_HPP_

#include <array>
#include <map>
#include <memory>
#include <cyvmath/player.hpp>

#include "piece.hpp"

namespace cyvmath
{
	namespace mikelepage
	{
		class Fortress;

		class Player : public cyvmath::Player
		{
			protected:
				bool _dragonAliveInactive;

				PieceMap& _activePieces;
				PieceVec _inactivePieces;

				std::shared_ptr<Fortress> _fortress;

			public:
				Player(PlayersColor color, PieceMap& activePieces)
					: cyvmath::Player(color)
					, _dragonAliveInactive(true)
					, _activePieces(activePieces)
				{ }

				virtual ~Player() = default;

				bool dragonAliveInactive() const
				{ return _dragonAliveInactive; }

				PieceVec& getInactivePieces()
				{ return _inactivePieces; }

				std::shared_ptr<Fortress> getFortress()
				{ return _fortress; }

				void setFortress(std::shared_ptr<Fortress>);

				virtual bool setupComplete() override;

				void dragonBroughtOut()
				{ _dragonAliveInactive = false; }

				void fortressRuined()
				{ _fortress.reset(); }
		};

		typedef std::array<std::shared_ptr<Player>, 2> PlayerArray;
	}
}

#endif // _CYVMATH_MIKELEPAGE_PLAYER_HPP_
