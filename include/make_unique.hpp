/* Copyright 2014 Jonas Platte
*
* This file is NO part of Cyvasse Online or cyvmath.
* It was just put here for convenience reasons.
*
* Use it however you want to.
*/

#ifndef _MAKE_UNIQUE_HPP_
#define _MAKE_UNIQUE_HPP_

#include <memory>

template<class T, class... Args>
inline std::unique_ptr<T> make_unique(Args&&... args)
{
	return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

#endif // _MAKE_UNIQUE_HPP_
