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

// for std::sort
#include <algorithm>

void HexagonTest::setUp()
{
	h6Coord1 = Hexagon<6>::Coordinate::create(3,  5);
	h6Coord2 = Hexagon<6>::Coordinate::create(1,  5);
	h6Coord3 = Hexagon<6>::Coordinate::create(3, 10);
	h6Coord4 = Hexagon<6>::Coordinate::create(6,  2);
	h6Coord5 = Hexagon<6>::Coordinate::create(1,  6);
	h6Coord6 = Hexagon<6>::Coordinate::create(1,  9);
	h6Coord7 = Hexagon<6>::Coordinate::create(9,  2);
	h6Coord8 = Hexagon<6>::Coordinate::create(2,  5);
	h6Coord9 = Hexagon<6>::Coordinate::create(5,  7);
}

void HexagonTest::tearDown()
{
	h6Coord1.reset();
	h6Coord2.reset();
	h6Coord3.reset();
	h6Coord4.reset();
	h6Coord5.reset();
	h6Coord6.reset();
	h6Coord7.reset();
	h6Coord8.reset();
	h6Coord9.reset();
}

void HexagonTest::testCoordValidity()
{
	CPPUNIT_ASSERT(h6Coord1);
	CPPUNIT_ASSERT(h6Coord2);
	CPPUNIT_ASSERT(h6Coord3);
	CPPUNIT_ASSERT(h6Coord4);
	CPPUNIT_ASSERT(h6Coord5);
	CPPUNIT_ASSERT(h6Coord6);
	CPPUNIT_ASSERT(h6Coord7);
	CPPUNIT_ASSERT(h6Coord8);
	CPPUNIT_ASSERT(h6Coord9);

	CPPUNIT_ASSERT(!Hexagon<6>::Coordinate::create( 0, 11)); // outside – top
	CPPUNIT_ASSERT(!Hexagon<6>::Coordinate::create( 8,  9)); // outside – top right
	CPPUNIT_ASSERT(!Hexagon<6>::Coordinate::create(11,  2)); // outside – bottom right
	CPPUNIT_ASSERT(!Hexagon<6>::Coordinate::create(10, -2)); // outside – bottom
	CPPUNIT_ASSERT(!Hexagon<6>::Coordinate::create( 1,  3)); // outside – bottom left
	CPPUNIT_ASSERT(!Hexagon<6>::Coordinate::create(-2,  7)); // outside – top left
}

void HexagonTest::testCoordEquality()
{
	CPPUNIT_ASSERT(*h6Coord1 == *h6Coord1);
	CPPUNIT_ASSERT(*h6Coord2 == *h6Coord2);
	CPPUNIT_ASSERT(*h6Coord3 == *h6Coord3);

	CPPUNIT_ASSERT(*h6Coord1 != *h6Coord2);
	CPPUNIT_ASSERT(*h6Coord1 != *h6Coord3);
	CPPUNIT_ASSERT(*h6Coord1 != *h6Coord4);
}

void HexagonTest::testCoordCompleteness()
{
	CPPUNIT_ASSERT(Hexagon<2>::getAllCoordinates().size() == 7);
	CPPUNIT_ASSERT(Hexagon<3>::getAllCoordinates().size() == 19);
	CPPUNIT_ASSERT(Hexagon<4>::getAllCoordinates().size() == 37);

	std::set<Hexagon<2>::Coordinate> coords = Hexagon<2>::getAllCoordinates();

	CPPUNIT_ASSERT(coords.find(*Hexagon<2>::Coordinate::create(0, 1)) != coords.end());
	CPPUNIT_ASSERT(coords.find(*Hexagon<2>::Coordinate::create(0, 2)) != coords.end());
	CPPUNIT_ASSERT(coords.find(*Hexagon<2>::Coordinate::create(1, 0)) != coords.end());
	CPPUNIT_ASSERT(coords.find(*Hexagon<2>::Coordinate::create(1, 1)) != coords.end());
	CPPUNIT_ASSERT(coords.find(*Hexagon<2>::Coordinate::create(1, 2)) != coords.end());
	CPPUNIT_ASSERT(coords.find(*Hexagon<2>::Coordinate::create(2, 0)) != coords.end());
	CPPUNIT_ASSERT(coords.find(*Hexagon<2>::Coordinate::create(2, 1)) != coords.end());
}

void HexagonTest::testDistanceOrthogonal()
{
	CPPUNIT_ASSERT(h6Coord1->getDistanceOrthogonal(*h6Coord2) == 2);
	CPPUNIT_ASSERT(h6Coord2->getDistanceOrthogonal(*h6Coord1) == 2);
	CPPUNIT_ASSERT(h6Coord1->getDistanceOrthogonal(*h6Coord3) == 5);
	CPPUNIT_ASSERT(h6Coord3->getDistanceOrthogonal(*h6Coord1) == 5);
	CPPUNIT_ASSERT(h6Coord1->getDistanceOrthogonal(*h6Coord4) == 3);
	CPPUNIT_ASSERT(h6Coord4->getDistanceOrthogonal(*h6Coord1) == 3);

	CPPUNIT_ASSERT(h6Coord2->getDistanceOrthogonal(*h6Coord3) == -1);
	CPPUNIT_ASSERT(h6Coord3->getDistanceOrthogonal(*h6Coord2) == -1);
	CPPUNIT_ASSERT(h6Coord1->getDistanceOrthogonal(*h6Coord5) == -1);
	CPPUNIT_ASSERT(h6Coord5->getDistanceOrthogonal(*h6Coord1) == -1);

	CPPUNIT_ASSERT(h6Coord1->getDistanceOrthogonal(*h6Coord1) == 0);
}

void HexagonTest::testDistanceDiagonal()
{
	CPPUNIT_ASSERT(h6Coord1->getDistanceDiagonal(*h6Coord5) == 1);
	CPPUNIT_ASSERT(h6Coord5->getDistanceDiagonal(*h6Coord1) == 1);
	CPPUNIT_ASSERT(h6Coord1->getDistanceDiagonal(*h6Coord6) == 2);
	CPPUNIT_ASSERT(h6Coord6->getDistanceDiagonal(*h6Coord1) == 2);
	CPPUNIT_ASSERT(h6Coord1->getDistanceDiagonal(*h6Coord7) == 3);
	CPPUNIT_ASSERT(h6Coord7->getDistanceDiagonal(*h6Coord1) == 3);

	CPPUNIT_ASSERT(h6Coord1->getDistanceDiagonal(*h6Coord2) == -1);
	CPPUNIT_ASSERT(h6Coord2->getDistanceDiagonal(*h6Coord1) == -1);
	CPPUNIT_ASSERT(h6Coord2->getDistanceDiagonal(*h6Coord3) == -1);
	CPPUNIT_ASSERT(h6Coord3->getDistanceDiagonal(*h6Coord2) == -1);

	CPPUNIT_ASSERT(h6Coord1->getDistanceDiagonal(*h6Coord1) == 0);
}

void HexagonTest::testDistanceHexagonalLine()
{
	CPPUNIT_ASSERT(h6Coord1->getDistanceHexagonalLine(*h6Coord4, *h6Coord6) == -1);
	CPPUNIT_ASSERT(h6Coord1->getDistanceHexagonalLine(*h6Coord6, *h6Coord4) == -1);
	CPPUNIT_ASSERT(h6Coord4->getDistanceHexagonalLine(*h6Coord1, *h6Coord6) == -1);

	CPPUNIT_ASSERT(h6Coord1->getDistanceHexagonalLine(*h6Coord1, *h6Coord1) == 0);
}

void HexagonTest::testDirectionOrthogonal()
{
	CPPUNIT_ASSERT(h6Coord1->getDirectionOrthogonal(*h6Coord3) == DirectionOrthogonal::TOP_RIGHT);
	CPPUNIT_ASSERT(h6Coord3->getDirectionOrthogonal(*h6Coord1) == DirectionOrthogonal::BOTTOM_LEFT);
	CPPUNIT_ASSERT(h6Coord1->getDirectionOrthogonal(*h6Coord2) == DirectionOrthogonal::LEFT);
	CPPUNIT_ASSERT(h6Coord2->getDirectionOrthogonal(*h6Coord1) == DirectionOrthogonal::RIGHT);
	CPPUNIT_ASSERT(h6Coord1->getDirectionOrthogonal(*h6Coord4) == DirectionOrthogonal::BOTTOM_RIGHT);
	CPPUNIT_ASSERT(h6Coord4->getDirectionOrthogonal(*h6Coord1) == DirectionOrthogonal::TOP_LEFT);

	CPPUNIT_ASSERT(h6Coord1->getDirectionOrthogonal(*h6Coord1) == DirectionOrthogonal::UNDEFINED);

	CPPUNIT_ASSERT(h6Coord1->getDirectionOrthogonal(*h6Coord5) == DirectionOrthogonal::UNDEFINED);
	CPPUNIT_ASSERT(h6Coord5->getDirectionOrthogonal(*h6Coord1) == DirectionOrthogonal::UNDEFINED);
	CPPUNIT_ASSERT(h6Coord2->getDirectionOrthogonal(*h6Coord3) == DirectionOrthogonal::UNDEFINED);
	CPPUNIT_ASSERT(h6Coord3->getDirectionOrthogonal(*h6Coord2) == DirectionOrthogonal::UNDEFINED);
	CPPUNIT_ASSERT(h6Coord6->getDirectionOrthogonal(*h6Coord7) == DirectionOrthogonal::UNDEFINED);
	CPPUNIT_ASSERT(h6Coord7->getDirectionOrthogonal(*h6Coord6) == DirectionOrthogonal::UNDEFINED);
}

void HexagonTest::testDirectionDiagonal()
{
	CPPUNIT_ASSERT(h6Coord1->getDirectionDiagonal(*h6Coord6) == DirectionDiagonal::TOP);
	CPPUNIT_ASSERT(h6Coord6->getDirectionDiagonal(*h6Coord1) == DirectionDiagonal::BOTTOM);
	CPPUNIT_ASSERT(h6Coord1->getDirectionDiagonal(*h6Coord7) == DirectionDiagonal::BOTTOM_RIGHT);
	CPPUNIT_ASSERT(h6Coord7->getDirectionDiagonal(*h6Coord1) == DirectionDiagonal::TOP_LEFT);
	CPPUNIT_ASSERT(h6Coord1->getDirectionDiagonal(*h6Coord9) == DirectionDiagonal::TOP_RIGHT);
	CPPUNIT_ASSERT(h6Coord9->getDirectionDiagonal(*h6Coord1) == DirectionDiagonal::BOTTOM_LEFT);

	CPPUNIT_ASSERT(h6Coord1->getDirectionDiagonal(*h6Coord1) == DirectionDiagonal::UNDEFINED);

	CPPUNIT_ASSERT(h6Coord1->getDirectionDiagonal(*h6Coord2) == DirectionDiagonal::UNDEFINED);
	CPPUNIT_ASSERT(h6Coord1->getDirectionDiagonal(*h6Coord2) == DirectionDiagonal::UNDEFINED);
	CPPUNIT_ASSERT(h6Coord2->getDirectionDiagonal(*h6Coord3) == DirectionDiagonal::UNDEFINED);
	CPPUNIT_ASSERT(h6Coord2->getDirectionDiagonal(*h6Coord3) == DirectionDiagonal::UNDEFINED);
	CPPUNIT_ASSERT(h6Coord5->getDirectionDiagonal(*h6Coord6) == DirectionDiagonal::UNDEFINED);
	CPPUNIT_ASSERT(h6Coord6->getDirectionDiagonal(*h6Coord5) == DirectionDiagonal::UNDEFINED);
}
