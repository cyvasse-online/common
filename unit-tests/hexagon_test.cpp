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

#include "hexagon_test.hpp"

#include <stdexcept>

using namespace std;

void HexagonTest::setUp()
{
	#define IGNORE_EXCEPT(expression) \
		try \
		{ expression; } \
		catch(...) \
		{ }

	IGNORE_EXCEPT(h6Coords.emplace_back(3,  5)); // 0
	IGNORE_EXCEPT(h6Coords.emplace_back(1,  5)); // 1
	IGNORE_EXCEPT(h6Coords.emplace_back(3, 10)); // 2
	IGNORE_EXCEPT(h6Coords.emplace_back(6,  2)); // 3
	IGNORE_EXCEPT(h6Coords.emplace_back(1,  6)); // 4
	IGNORE_EXCEPT(h6Coords.emplace_back(1,  9)); // 5
	IGNORE_EXCEPT(h6Coords.emplace_back(9,  2)); // 6
	IGNORE_EXCEPT(h6Coords.emplace_back(5,  7)); // 7

	#undef IGNORE_EXCEPT
}

void HexagonTest::tearDown()
{
	h6Coords.clear();
}

void HexagonTest::testCoordValidity()
{
	static const set<pair<int8_t, int8_t>> invalidCoords {
		{ 0, 11}, // outside – top
		{ 8,  9}, // outside – top right
		{11,  2}, // outside – bottom right
		{10, -2}, // outside – bottom
		{ 1,  3}, // outside – bottom left
		{-2,  7}  // outside – top left
	};

	CPPUNIT_ASSERT(h6Coords.size() == 8);

	for(const auto& invalidCoord : invalidCoords)
	{
		CPPUNIT_ASSERT(!Hexagon<6>::Coordinate::create(invalidCoord.first, invalidCoord.second));
		CPPUNIT_ASSERT_THROW(Hexagon<6>::Coordinate(invalidCoord.first, invalidCoord.second), invalid_argument);
	}
}

void HexagonTest::testCoordEquality()
{
	for(size_t i = 0; i < h6Coords.size(); i++)
		CPPUNIT_ASSERT(h6Coords[i] == h6Coords[i]);

	CPPUNIT_ASSERT(h6Coords[0] != h6Coords[1]);
	CPPUNIT_ASSERT(h6Coords[0] != h6Coords[2]);
	CPPUNIT_ASSERT(h6Coords[0] != h6Coords[3]);
}

void HexagonTest::testCoordCompleteness()
{
	CPPUNIT_ASSERT(Hexagon<2>::allCoordinates.size() == 7);
	CPPUNIT_ASSERT(Hexagon<3>::allCoordinates.size() == 19);
	CPPUNIT_ASSERT(Hexagon<4>::allCoordinates.size() == 37);

	const auto& coords = Hexagon<2>::allCoordinates;

	CPPUNIT_ASSERT_NO_THROW(
		CPPUNIT_ASSERT(coords.find(Hexagon<2>::Coordinate(0, 1)) != coords.end());
		CPPUNIT_ASSERT(coords.find(Hexagon<2>::Coordinate(0, 2)) != coords.end());
		CPPUNIT_ASSERT(coords.find(Hexagon<2>::Coordinate(1, 0)) != coords.end());
		CPPUNIT_ASSERT(coords.find(Hexagon<2>::Coordinate(1, 1)) != coords.end());
		CPPUNIT_ASSERT(coords.find(Hexagon<2>::Coordinate(1, 2)) != coords.end());
		CPPUNIT_ASSERT(coords.find(Hexagon<2>::Coordinate(2, 0)) != coords.end());
		CPPUNIT_ASSERT(coords.find(Hexagon<2>::Coordinate(2, 1)) != coords.end());
	);
}

void HexagonTest::testDistanceOrthogonal()
{
	CPPUNIT_ASSERT(h6Coords[0].getDistanceOrthogonal(h6Coords[1]) == 2);
	CPPUNIT_ASSERT(h6Coords[1].getDistanceOrthogonal(h6Coords[0]) == 2);
	CPPUNIT_ASSERT(h6Coords[0].getDistanceOrthogonal(h6Coords[2]) == 5);
	CPPUNIT_ASSERT(h6Coords[2].getDistanceOrthogonal(h6Coords[0]) == 5);
	CPPUNIT_ASSERT(h6Coords[0].getDistanceOrthogonal(h6Coords[3]) == 3);
	CPPUNIT_ASSERT(h6Coords[3].getDistanceOrthogonal(h6Coords[0]) == 3);

	CPPUNIT_ASSERT(h6Coords[1].getDistanceOrthogonal(h6Coords[2]) == -1);
	CPPUNIT_ASSERT(h6Coords[2].getDistanceOrthogonal(h6Coords[1]) == -1);
	CPPUNIT_ASSERT(h6Coords[0].getDistanceOrthogonal(h6Coords[4]) == -1);
	CPPUNIT_ASSERT(h6Coords[4].getDistanceOrthogonal(h6Coords[0]) == -1);

	CPPUNIT_ASSERT(h6Coords[0].getDistanceOrthogonal(h6Coords[0]) == 0);
}

void HexagonTest::testDistanceDiagonal()
{
	CPPUNIT_ASSERT(h6Coords[0].getDistanceDiagonal(h6Coords[4]) == 1);
	CPPUNIT_ASSERT(h6Coords[4].getDistanceDiagonal(h6Coords[0]) == 1);
	CPPUNIT_ASSERT(h6Coords[0].getDistanceDiagonal(h6Coords[5]) == 2);
	CPPUNIT_ASSERT(h6Coords[5].getDistanceDiagonal(h6Coords[0]) == 2);
	CPPUNIT_ASSERT(h6Coords[0].getDistanceDiagonal(h6Coords[6]) == 3);
	CPPUNIT_ASSERT(h6Coords[6].getDistanceDiagonal(h6Coords[0]) == 3);

	CPPUNIT_ASSERT(h6Coords[0].getDistanceDiagonal(h6Coords[1]) == -1);
	CPPUNIT_ASSERT(h6Coords[1].getDistanceDiagonal(h6Coords[0]) == -1);
	CPPUNIT_ASSERT(h6Coords[1].getDistanceDiagonal(h6Coords[2]) == -1);
	CPPUNIT_ASSERT(h6Coords[2].getDistanceDiagonal(h6Coords[1]) == -1);

	CPPUNIT_ASSERT(h6Coords[0].getDistanceDiagonal(h6Coords[0]) == 0);
}

void HexagonTest::testDistanceHexagonalLine()
{
	CPPUNIT_ASSERT(h6Coords[0].getDistanceHexagonalLine(h6Coords[3], h6Coords[5]) == -1);
	CPPUNIT_ASSERT(h6Coords[0].getDistanceHexagonalLine(h6Coords[5], h6Coords[3]) == -1);
	CPPUNIT_ASSERT(h6Coords[3].getDistanceHexagonalLine(h6Coords[0], h6Coords[5]) == -1);

	CPPUNIT_ASSERT(h6Coords[0].getDistanceHexagonalLine(h6Coords[0], h6Coords[0]) == 0);
}

void HexagonTest::testDirectionOrthogonal()
{
	CPPUNIT_ASSERT(h6Coords[0].getDirectionOrthogonal(h6Coords[2]) == DirectionOrthogonal::TOP_RIGHT);
	CPPUNIT_ASSERT(h6Coords[2].getDirectionOrthogonal(h6Coords[0]) == DirectionOrthogonal::BOTTOM_LEFT);
	CPPUNIT_ASSERT(h6Coords[0].getDirectionOrthogonal(h6Coords[1]) == DirectionOrthogonal::LEFT);
	CPPUNIT_ASSERT(h6Coords[1].getDirectionOrthogonal(h6Coords[0]) == DirectionOrthogonal::RIGHT);
	CPPUNIT_ASSERT(h6Coords[0].getDirectionOrthogonal(h6Coords[3]) == DirectionOrthogonal::BOTTOM_RIGHT);
	CPPUNIT_ASSERT(h6Coords[3].getDirectionOrthogonal(h6Coords[0]) == DirectionOrthogonal::TOP_LEFT);

	CPPUNIT_ASSERT(h6Coords[0].getDirectionOrthogonal(h6Coords[0]) == DirectionOrthogonal::UNDEFINED);

	CPPUNIT_ASSERT(h6Coords[0].getDirectionOrthogonal(h6Coords[4]) == DirectionOrthogonal::UNDEFINED);
	CPPUNIT_ASSERT(h6Coords[4].getDirectionOrthogonal(h6Coords[0]) == DirectionOrthogonal::UNDEFINED);
	CPPUNIT_ASSERT(h6Coords[1].getDirectionOrthogonal(h6Coords[2]) == DirectionOrthogonal::UNDEFINED);
	CPPUNIT_ASSERT(h6Coords[2].getDirectionOrthogonal(h6Coords[1]) == DirectionOrthogonal::UNDEFINED);
	CPPUNIT_ASSERT(h6Coords[5].getDirectionOrthogonal(h6Coords[6]) == DirectionOrthogonal::UNDEFINED);
	CPPUNIT_ASSERT(h6Coords[6].getDirectionOrthogonal(h6Coords[5]) == DirectionOrthogonal::UNDEFINED);
}

void HexagonTest::testDirectionDiagonal()
{
	CPPUNIT_ASSERT(h6Coords[0].getDirectionDiagonal(h6Coords[5]) == DirectionDiagonal::TOP);
	CPPUNIT_ASSERT(h6Coords[5].getDirectionDiagonal(h6Coords[0]) == DirectionDiagonal::BOTTOM);
	CPPUNIT_ASSERT(h6Coords[0].getDirectionDiagonal(h6Coords[6]) == DirectionDiagonal::BOTTOM_RIGHT);
	CPPUNIT_ASSERT(h6Coords[6].getDirectionDiagonal(h6Coords[0]) == DirectionDiagonal::TOP_LEFT);
	CPPUNIT_ASSERT(h6Coords[0].getDirectionDiagonal(h6Coords[7]) == DirectionDiagonal::TOP_RIGHT);
	CPPUNIT_ASSERT(h6Coords[7].getDirectionDiagonal(h6Coords[0]) == DirectionDiagonal::BOTTOM_LEFT);

	CPPUNIT_ASSERT(h6Coords[0].getDirectionDiagonal(h6Coords[0]) == DirectionDiagonal::UNDEFINED);

	CPPUNIT_ASSERT(h6Coords[0].getDirectionDiagonal(h6Coords[1]) == DirectionDiagonal::UNDEFINED);
	CPPUNIT_ASSERT(h6Coords[0].getDirectionDiagonal(h6Coords[1]) == DirectionDiagonal::UNDEFINED);
	CPPUNIT_ASSERT(h6Coords[1].getDirectionDiagonal(h6Coords[2]) == DirectionDiagonal::UNDEFINED);
	CPPUNIT_ASSERT(h6Coords[1].getDirectionDiagonal(h6Coords[2]) == DirectionDiagonal::UNDEFINED);
	CPPUNIT_ASSERT(h6Coords[4].getDirectionDiagonal(h6Coords[5]) == DirectionDiagonal::UNDEFINED);
	CPPUNIT_ASSERT(h6Coords[5].getDirectionDiagonal(h6Coords[4]) == DirectionDiagonal::UNDEFINED);
}
