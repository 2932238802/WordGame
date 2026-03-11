#pragma once

#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <cstdint>

namespace LosG {
	using ls = std::string;
	using lsw = std::string_view;
	using b = bool;
	using c = char;
	using i = int32_t;
	using d = double;
	using EntityId = uint32_t;
	using EnVec = std::vector<EntityId>;
	template <typename T>
	using MapIdToT = std::unordered_map<EntityId, T>;
}
