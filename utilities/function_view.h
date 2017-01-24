#pragma once

#include <type_traits>
#include <functional>

/*
	Based on the function view from
	https://github.com/SuperV1234/vittorioromeo.info/blob/master/extra/passing_functions_to_functions/function_view.hpp
*/

//	Need these definitions of is_callable until c++17 lands
//	http://en.cppreference.com/w/cpp/types/is_callable
// ============================================================================
template <class T, class R = void, class = void>
struct is_callable : std::false_type
{};

template <class T>
struct is_callable<T, void, std::void_t<std::result_of_t<T>>> : std::true_type
{};

template <class T, class R>
struct is_callable<T, R, std::void_t<std::result_of_t<T>>>
	: std::is_convertible<std::result_of_t<T>, R>
{};

// ============================================================================


template <typename TSignature>
class func;

// A light-weight function reference
template <typename TReturn, typename... TArgs>
class func<TReturn(TArgs...)>
{
private:

	void* _ptr;
	TReturn(*_erased_fn)(void*, TArgs...);

public:
	template <typename T>
	func(T&& x) noexcept : _ptr{ (void*)std::addressof(x) }
	{
		// assert error message is preffered over the template specialization error
		static_assert(is_callable<T&(TArgs...)>::value, "Must use callable object with function view!");
		
		//static_assert(!std::is_same<std::decay_t<T>, function_view>::value, "Must not be another function view!");
		
		_erased_fn = [](void* ptr, TArgs... xs) -> TReturn {
			return (*reinterpret_cast<std::add_pointer_t<T>>(ptr))(
				std::forward<TArgs>(xs)...);
		};
	}

	TReturn operator()(TArgs... xs) const
		noexcept(noexcept(_erased_fn(_ptr, std::forward<TArgs>(xs)...)))
	{
		return _erased_fn(_ptr, std::forward<TArgs>(xs)...);
	}
};