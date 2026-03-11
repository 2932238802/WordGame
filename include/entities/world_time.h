#pragma once
#include "common/constant.h"

namespace LosG
{
	/*
	时间
	*/
	struct WorldTimeComponent {
		d total_game_minite;										// game time
		i get_day() const
		{
			return static_cast<int>(total_game_minite / 1440.0) + 1;
		}
		i get_hour() const {
			return (static_cast<int>(total_game_minite) % 1440) / 60;
		}
		i get_min() const {
			return static_cast<int>(total_game_minite) % 60;
		}
		Season getSeason()
		{
			return DayToSeason(get_day());
		}
	};


	/*
	3_5
	- weather
	*/
	struct WorldEnvironmentComponent {
		Weather current_weather = Weather::Clear;
		d durationTime = 120.0 ;
		static double getSeasonPerishMult(Season s) {
			switch (s) {
			case Season::Spring: return 1.0;  // 常温
			case Season::Summer: return 1.8;  // 高温，腐烂显著加快
			case Season::Autumn: return 1.0;  // 常温
			case Season::Winter: return 0.2;  // 低温/冷冻，腐烂极慢
			default: return 1.0;
			}
		}
		static double getWeatherPerishMult(Weather w) {
			switch (w) {
			case Weather::Clear:    return 1.0;  // 正常
			case Weather::Rainy:    return 1.3;  // 潮湿加速
			case Weather::Storm:    return 1.5;  // 极度潮湿
			case Weather::Heatwave: return 2.5;  // 极端高温
			case Weather::Foggy:    return 1.1;  // 轻微潮湿
			case Weather::Snowy:	return 0.2;
			default: return 1.0;
			}
		}
	};

	/*
	26_3_6
	- 获取 腐烂速度
	*/
	inline d getPerishMult(Season s, Weather w)
	{
		return WorldEnvironmentComponent::getSeasonPerishMult(s) * WorldEnvironmentComponent::getWeatherPerishMult(w);
	}

}
