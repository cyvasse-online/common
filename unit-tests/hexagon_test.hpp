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

#ifndef _HEXAGON_TEST_HPP_
#define _HEXAGON_TEST_HPP_

#include <cppunit/TestFixture.h>

#include <cppunit/extensions/HelperMacros.h>
#include "hexagon.hpp"

using namespace cyvmath;

class HexagonTest : public CppUnit::TestFixture
{
	private:
		std::unique_ptr<hexagon<6>::Coordinate> h6Coord1;
		std::unique_ptr<hexagon<6>::Coordinate> h6Coord2;
		std::unique_ptr<hexagon<6>::Coordinate> h6Coord3;
		std::unique_ptr<hexagon<6>::Coordinate> h6Coord4;
		std::unique_ptr<hexagon<6>::Coordinate> h6Coord5;
		std::unique_ptr<hexagon<6>::Coordinate> h6Coord6;
		std::unique_ptr<hexagon<6>::Coordinate> h6Coord7;
		std::unique_ptr<hexagon<6>::Coordinate> h6Coord8;

	public:
		void setUp() override;
		void tearDown() override;

		void testCoordValidity();
		void testCoordEquality();
		void testCoordCompleteness();

		void testDistanceOrthogonal();
		void testDistanceDiagonal();
		void testDistanceHexagonalLine();

	CPPUNIT_TEST_SUITE(HexagonTest);
		CPPUNIT_TEST(testCoordValidity);
		CPPUNIT_TEST(testCoordEquality);
		CPPUNIT_TEST(testCoordCompleteness);

		CPPUNIT_TEST(testDistanceOrthogonal);
		CPPUNIT_TEST(testDistanceDiagonal);
		CPPUNIT_TEST(testDistanceHexagonalLine);
	CPPUNIT_TEST_SUITE_END();
};

#endif // _HEXAGON_TEST_HPP_
