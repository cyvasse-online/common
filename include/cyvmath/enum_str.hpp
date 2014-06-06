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

#ifndef _CYVMATH_ENUM_STR_HPP_
#define _CYVMATH_ENUM_STR_HPP_

#include <initializer_list>
#include <map>
#include <utility>

namespace cyvmath
{
	template <typename T1, typename T2>
	using initMap = std::initializer_list<std::pair<const T1, T2>>;

	template <typename EnumT>
	std::map<const char*, const EnumT> swapFirstSecond(initMap<EnumT, const char*> orig)
	{
		std::map<const char*, const EnumT> ret;
		for(const std::pair<const EnumT, const char*>& it : orig)
			ret.emplace(it.second, it.first);

		return ret;
	}

	#define ENUM_STR_PROT(type) \
		const char* type ## ToStr(type e); \
		type StrTo ## type(const char* s); \
		type StrTo ## type(const std::string& s);

	#define ENUM_STR(type, init) \
		inline const char* type ## ToStr(type e) \
		{ \
			static std::map< type , const char*> data(init); \
			\
			auto it = data.find(e); \
			if(it == data.end()) \
				return ""; \
			\
			return it->second; \
		}\
		\
		inline type StrTo ## type(const char* s) \
		{ \
			static std::map<const char*, const type > data(swapFirstSecond(init)); \
			\
			auto it = data.find(s); \
			if(it == data.end()) \
				return static_cast<type>(0); \
			\
			return it->second; \
		} \
		\
		inline type StrTo ## type(const std::string& s) \
		{ \
			return StrTo ## type(s.c_str()); \
		}
}

#endif // _CYVMATH_ENUM_STR_HPP_
