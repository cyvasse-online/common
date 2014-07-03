/* Copyright 2014 Jonas Platte
 *
 * This file is NO part of Cyvasse Online or cyvmath.
 *
 * Use it however you want to.
 */

#ifndef _DEEPCOPY_UNIQUE_PTR_HPP_
#define _DEEPCOPY_UNIQUE_PTR_HPP_

#include <memory>

namespace dc
{
	template <class T, class D = std::default_delete<T>>
	class unique_ptr
	{
		public:
			typedef T element_type;
			typedef D deleter_type;
			typedef T* pointer;
			/*typedef typename std::conditional
					<std::is_pointer<typename std::remove_reference<D>::type::pointer>::value,
					typename std::remove_reference<D>::type::pointer, T*>
				pointer;*/

		private:
			std::unique_ptr<T, D> _ptr;

		public:
			constexpr unique_ptr() noexcept = default;

			constexpr unique_ptr(std::nullptr_t) noexcept : unique_ptr() {}
			explicit unique_ptr(pointer p) noexcept : _ptr(p) {}

			unique_ptr(pointer p, typename std::conditional<std::is_reference<D>::value, D, const D&> del) noexcept : _ptr(p, del) {}
			unique_ptr(pointer p, typename std::remove_reference<D>::type&& del) noexcept : _ptr(p, std::move(del)) {}

			unique_ptr(unique_ptr&& x) noexcept : _ptr(std::move(x._ptr)) {}
			unique_ptr(std::unique_ptr<T, D>&& x) noexcept : _ptr(std::move(x)) {}

			template <class U, class E>
			unique_ptr(unique_ptr<U, E>&& x) noexcept : _ptr(std::move(x.get_std_ptr())) {}

			template <class U, class E>
			unique_ptr(std::unique_ptr<U, E>&& x) noexcept : _ptr(std::move(x)) {}

			template <class U>
			unique_ptr(std::auto_ptr<U>&& x) noexcept : _ptr(std::move(x)) {}

			unique_ptr(const unique_ptr&) = delete;

			~unique_ptr() = default;

			unique_ptr& operator=(unique_ptr&& x) noexcept { _ptr = std::move(x._ptr); return *this; }
			unique_ptr& operator=(std::unique_ptr<T, D>&& x) noexcept { _ptr = std::move(x); return *this; }

			unique_ptr& operator=(std::nullptr_t null) noexcept { _ptr = null; return *this; }

			template <class U, class E>
			unique_ptr& operator=(unique_ptr<U, E>&& x) noexcept { _ptr = std::move(x._ptr); return *this; }

			template <class U, class E>
			unique_ptr& operator=(std::unique_ptr<U, E>&& x) noexcept { _ptr = std::move(x); return *this; }

			unique_ptr& operator=(const unique_ptr&) = delete;

			pointer get() const noexcept { return _ptr.get(); }

			deleter_type& get_deleter() noexcept { return _ptr.get_deleter(); }
			const deleter_type& get_deleter() const noexcept { return _ptr.get_deleter(); }

			explicit operator bool() const noexcept { return static_cast<bool>(_ptr); }

			pointer release() noexcept { return _ptr.release(); }

			void reset(pointer p = pointer()) noexcept { _ptr.reset(p); }

			void swap(unique_ptr& x) noexcept { _ptr.swap(x._ptr); }
			void swap(std::unique_ptr<T, D>& x) noexcept { _ptr.swap(x); }

			// not STL compatible, should only be available for non-array element_type
			typename std::add_lvalue_reference<element_type>::type operator*() const { return *_ptr; }
			pointer operator->() const noexcept { return _ptr.operator->(); }

			// extra functionality
			unique_ptr clone() const { return _ptr ? unique_ptr(_ptr->clone()) : unique_ptr(); }

			std::unique_ptr<T, D>& get_std_ptr() { return _ptr; }
	};

	template<class T, class... Args>
	inline unique_ptr<T> make_unique(Args&&... args)
	{
		return unique_ptr<T>(new T(std::forward<Args>(args)...));
	}

	#define DEEPCOPYABLE_BASE(type) \
		friend dc::unique_ptr< type >; \
		virtual type * clone() \
		{ return new type(*this); }

	#define DEEPCOPYABLE_ABSTRACT_BASE(type) \
		friend dc::unique_ptr< type >; \
		virtual type * clone() = 0;

	#define DEEPCOPYABLE_DERIVED(type) \
		friend dc::unique_ptr< type >; \
		virtual type * clone() override \
		{ return new type(*this); }
}

template<class T>
struct managed_less
{
	bool operator()(const T& obj1, const T& obj2) const
	{
		return (*obj1) < (*obj2);
	}
};

// outside any namespace to avoid conflicts when std::make_unique() is added to STL
template<class T, class... Args>
inline std::unique_ptr<T> make_unique(Args&&... args)
{
	return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

#endif // _DEEPCOPY_UNIQUE_PTR_HPP_
