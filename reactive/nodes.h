#pragma once

#include <tuple>

#include "reactive.h"

template <class ...Ts>
using SourceNode = std::tuple<Get<Ts>...>;


template <class T>
using Morphism = std::function<T>;

template <class ReturnT, class ...Ts>
struct MorphNode {

	std::shared_ptr<SourceNode<Ts...>> sources;
	Morphism<ReturnT(Ts...)> f;

	MorphNode(Morphism<ReturnT(Ts...)> _f, std::shared_ptr<SourceNode<Ts...>> vs)
		: f(_f), sources(vs) {}

	ReturnT apply() { return apply_from_sources(f, *sources); }
};