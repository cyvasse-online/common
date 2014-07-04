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

// same as std::make_unique in C++1y
template<class T, class... Args>
inline std::unique_ptr<T> make_unique(Args&&... args)
{
	return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

// copy a std::unique_ptr's managed object (must be copy-constructible, no deep-copy)
template<class T>
inline std::unique_ptr<T> make_unique(const std::unique_ptr<T>& orig)
{
	return orig ? make_unique<T>(*orig) : nullptr;
}

// copy a std::shared_ptr's managed object (must be copy-constructible, no deep-copy)
template<class T>
inline std::unique_ptr<T> make_unique(const std::shared_ptr<T>& orig)
{
	return orig ? make_unique<T>(*orig) : nullptr;
}

#endif // _MAKE_UNIQUE_HPP_
