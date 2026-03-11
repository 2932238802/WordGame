#pragma once
#include "common/constant.h"
#include "common/utities.h"
#include <concepts>
#include <format>
#include <iostream>
#include <string>
#include <string_view>
#include <chrono>
#include <regex>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <future>
#include <regex>
#include <chrono>
#include <cstdlib>
#include <format>
#include <regex>
#include <stack>
#include <string>
#include <thread>
#include <utility>

/////////////////////////////////
// 常量
namespace LosG {

	inline constexpr std::string_view EXE_EXIT_PARAM = R"(^\s*(exit)\s*$)";
	inline constexpr std::string_view EXE_HELLO_PARAM = R"(^\s*(hello|hi)\s*$)";
	inline constexpr std::string_view EXE_STATE_PARAM = R"(^\s*(state|st|s|stat|status)\s*(\d*)\s*$)";
	inline constexpr std::string_view EXE_HELP_PARAM = R"(^\s*(help|h)\s*$)";
	inline constexpr std::string_view EXE_WHERE_PARAM = R"(^\s*(where)\s*$)";
	inline constexpr std::string_view EXE_MAP_PARAM = R"(^\s*(map)\s*$)";
	inline constexpr std::string_view EXE_BAG_PARAM = R"(^\s*(bag)\s*$)";
	inline constexpr std::string_view EXE_EAT_PARAM = R"(^\s*(eat|e)\s+(\d+)\s*$)";
	inline constexpr std::string_view EXE_CLS_PARAM = R"(^\s*(cls|clear)\s*$)";
	inline constexpr std::string_view EXE_GO_PARAM = R"(^\s*(go|went|g)\s+([DFHLPSC])\s*$)";
	inline constexpr std::string_view EXE_MAKE_PARAM = R"(^\s*(make)\s*(\d*)\s*$)";
	inline constexpr std::string_view YES_PARAM = R"(^\s*(yes|y|ye)\s*$)";
	inline constexpr std::string_view LOSCOLOR_RESET = "\033[0m";
	inline constexpr std::string_view LOSCOLOR_RED = "\033[31m";
	inline constexpr std::string_view LOSCOLOR_GREEN = "\033[32m";

	inline d PLAYER_MAX_HUNGRY = 100.0;
	inline d PLAYER_MIN_HUNGRY = 0.0;
	inline d PLAYER_MAX_MANTEL = 100.0;
	inline d PLAYER_MIN_MANTEL = 0.0;
	inline d PLAYER_LITTLE_HUNGRY = 60.0;
	inline d PLAYER_LITTLE_MENTAL = 70.0;
	inline d PLAYER_MID_HUNGRY = 30.0;
	inline d PLAYER_MID_MENTAL = 50.0;
	inline d PLAYER_EXTREME_HUNGRY = 10.0;
	inline d PLAYER_EXTREME_MENTAL = 30.0;


} // namespace LosG




///////////////////////////////////
// 枚举
namespace LosG {

	enum class Place {
		HOME,             // H
		FOREST,             // F
		DEEP_FOREST,        // DF
		LAKE,             // L
		PLAINS,             // 平原-
		SWAMP,              // 沼泽
		CAVE,               // 矿洞
	};

	/*
	3_5
	- season
	*/
	enum class Season {
		Spring,
		Summer,
		Autumn,
		Winter,
		Unkonw
	};
	
	/*
	3_5
	- weather
	*/
	enum class Weather {
		Clear,				// 晴朗
		Rainy,				// 小雨
		Storm,				// 雷雨 潮湿
		Heatwave,			// 高温
		Foggy,				// 大雾
		Snowy
	};

	/*
	3_5 
	- compare
	*/
	enum class CompareFlag {
		LEFT_NONE,
		RIGHE_NONE,
		ALL_NONE,
		ALL
	};
}





///////////////////////////////////
/*
3_10
- tool 
*/
namespace LosG {
	/*
	26_2_25
	打印
	 */
	template <typename... Arg> inline void P(std::string_view fmt, Arg... args) {
		std::cout << std::vformat(fmt, std::make_format_args(args...)) << std::endl;
	}

	template <std::totally_ordered T>
	inline bool isBettew(T _val, T _little, T _max, CompareFlag _flag = CompareFlag::ALL)
	{
		if (_flag == CompareFlag::ALL)
			return _val <= _max && _val >= _little;
		else if (_flag == CompareFlag::ALL_NONE)
			return _val < _max && _val > _little;
		else if (_flag == CompareFlag::LEFT_NONE)
			return _val <= _max && _val > _little;
		else
			return _val < _max && _val >= _little;
	}

	/*
3_5
- get string name
*/
	inline std::string GetSeasonName(Season s)
	{
		switch (s)
		{
		case Season::Autumn: {
			return "Autumn";
		}
		case Season::Spring: {
			return "Spring";

		}
		case Season::Winter: {
			return "Winter";
		}
		case Season::Summer: {
			return "Summer";
		}
		default:
			return "Unkown";
		}
	}
	inline std::string GetPlaceName(Place place)
	{
		switch (place)
		{
		case Place::FOREST: {
			return "forest";
		}
		case Place::HOME: {
			return "home";
		}
		case Place::PLAINS: {
			return "plains";
		}
		case Place::CAVE: {
			return "cave";
		}
		case Place::DEEP_FOREST: {
			return "deep forest";
		}
		case Place::SWAMP: {
			return "swamp";
		}
		case Place::LAKE:
		{
			return "lake";
		}
		default: {
			return "unkown place";
		}
		}
	}

	inline Season DayToSeason(int day)
	{
		i a = day % 80;		// 80 day a year
		i b = a / 20;
		i c = a % 20;
		int number = b + c == 0 ? 0 : 1;
		switch (number)
		{
		case 4:
		{
			return Season::Winter;
		}
		case 3:
		{
			return Season::Autumn;
		}
		case 2:
		{
			return Season::Summer;
		}
		case 1:
		{
			return Season::Spring;
		}
		default:
			return Season::Unkonw;
		}
	}


	/*
	3_5
	- weather to str
	*/
	inline ls GetWeatherStr(Weather w)
	{
		switch (w)
		{
		case Weather::Clear:
		{
			return "Clear";
		}
		case Weather::Foggy:
		{
			return "Foggy";
		}
		case Weather::Heatwave:
		{
			return "Heatwave";
		}
		case Weather::Rainy:
		{
			return "Rainy";
		}
		default:
			return "Unkown";
		}
	}



	static inline const std::unordered_map<Season, std::unordered_map<Weather, int>> SEASON_WEATHER_WEIGHT
	{
		{
			Season::Spring,
			{
				{Weather::Clear, 50},
				{Weather::Rainy, 30},
				{Weather::Foggy, 10},
				{Weather::Storm, 5},
				{Weather::Snowy, 5}
			}
		},
		{
			Season::Summer,
			{
				{Weather::Clear, 40},
				{Weather::Heatwave, 30},
				{Weather::Storm, 20},
				{Weather::Rainy, 10}
			}
		},
		{
			Season::Autumn,
			{
				{Weather::Clear, 60},
				{Weather::Foggy, 25},
				{Weather::Rainy, 10},
				{Weather::Storm, 5}
			}
		},
		{
			Season::Winter,
			{
				{Weather::Snowy, 60},
				{Weather::Clear, 20},
				{Weather::Storm, 10},
				{Weather::Foggy, 10}
			}
		}
	};


	/*
3_5
- 获取 随机天气
*/
	inline Weather getRandomWeather(Season currentSeason)
	{
		auto it = SEASON_WEATHER_WEIGHT.find(currentSeason);
		if (it == SEASON_WEATHER_WEIGHT.end())
		{
			P("天气权重哈希表未找到该季节: {}", (int)currentSeason);
			return Weather::Clear;
		}
		const auto& weatherMap = it->second;
		double randomRoll = LosRandUniform(0.0, 100.0);
		double cursor = 0.0;
		for (const auto& [weather, weight] : weatherMap)
		{
			cursor += weight;
			if (randomRoll <= cursor)
			{
				return weather;
			}
		}
		return Weather::Clear;
	}

}