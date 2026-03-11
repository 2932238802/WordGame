#pragma once
#include <random>
#include <windows.h>
#include <filesystem>
#include <algorithm> 

#include "common/constant.h"
#include "common/types.h"

namespace LosG {
	inline double LosRand(double baseNumber, double min, double max) {
		static std::mt19937 gen(std::random_device{}());
		double sigma = (max - min) / 6.0;
		if (sigma <= 0.0) return baseNumber;
		std::normal_distribution<double> dist(static_cast<double>(baseNumber), sigma);
		double result;
		int retry_limit = 10;
		do {
			result = dist(gen);
		} while ((result < min || result > max) && --retry_limit > 0);
		return std::clamp(result, static_cast<double>(min), static_cast<double>(max));
	}

	inline double LosRandUniform(double min, double max)
	{
		static std::mt19937 gen(std::random_device{}());
		std::uniform_real_distribution<double> dist(min, max);
		return dist(gen);
	}

	inline ls getExePath()
	{
		char buffer[MAX_PATH];
		GetModuleFileNameA(nullptr, buffer, MAX_PATH);
		ls path(buffer);
		size_t pos = path.find_last_of("\\/");
		if (pos != ls::npos)
		{
			return path.substr(0, pos);
		}
		return path;
	}

	inline ls findItemConfigLua(ls target_filename)
	{
		std::filesystem::path currentPath = getExePath();
		for (int i = 0; i < 4; ++i)
		{
			if (!currentPath.has_parent_path()) break;
			currentPath = currentPath.parent_path();
			std::filesystem::path targetPath = currentPath / "scripts" / target_filename;
			if (std::filesystem::exists(targetPath))
			{
				return targetPath.string();
			}
		}

		return "";
	}


}; // namespace LosG