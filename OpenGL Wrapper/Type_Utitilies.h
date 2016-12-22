#pragma once

template <int N, class ...Types> struct type_index {};

template <int N, class T, class ...Types> struct type_index<N, T, Types...>
{
	using type = typename type_index<N - 1, T...>::type;
};

template <class T, class ...Types> struct type_index<0, T, Types...>
{
	using type = T;
};

//template <int N, class ...Types>
//using type_index_t = type_index<N, Types...>::type;

//template <class A, class B>
//using enable_if_same_t = std::enable_if<std::is_same<A, B>>::type;