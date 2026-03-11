#pragma once

#include "sol/sol.hpp"

#include "common/constant.h"

#include "entities/basic.h"
#include "entities/item.h"

#include "core/register.h"

namespace LosG
{
	class Engine;
	class LuaManager
	{
	public:
		static LuaManager& instance();
		void init(Engine* e);
		void initUtil();
		void initCommonForestThings();
		void initComponent();
		sol::state& getState();
		bool loadScript(lsw file_name);
	private:
		LuaManager() = default;
		LuaManager(const LuaManager&) = delete;
		LuaManager& operator=(const LuaManager&) = delete;
		~LuaManager() = default;
		sol::state LOS_lua;
		Engine* LOS_engine;
	};
}

