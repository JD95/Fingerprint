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