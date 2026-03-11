#include "core/lua_manager.h"
#include "core/engine.h"

namespace LosG
{
	/*
	- instance
	*/
	LuaManager& LuaManager::instance() {
		static LuaManager m;
		return m;
	}



	/*
	init engine
	*/
	void LuaManager::init(Engine* engine)
	{
		if (engine == nullptr)
		{
			P("{}engin is nullptr! please ctrl+c{}", LOSCOLOR_RED, LOSCOLOR_RESET);
			return;
		}
		LOS_engine = engine;
		LOS_lua.open_libraries(
			sol::lib::base,
			sol::lib::string,
			sol::lib::math,
			sol::lib::io,
			sol::lib::table,
			sol::lib::os     
		);
		initComponent();
		initUtil();
		initCommonForestThings();
	}



	/*
	load scripts
	26_3_9
	- 增加 美化样式

	26_3_10
	- 增加 打印中 file_name
	*/
	bool LuaManager::loadScript(lsw file_name)
	{
		auto res = LOS_lua.script_file(ls(file_name));
		if (!res.valid())
		{
			sol::error e = res;
			P("{} Lua script error: {}{}", LOSCOLOR_RED, e.what(), LOSCOLOR_RESET);
			return false;
		}
		P("{} Lua script {} suc{}", LOSCOLOR_GREEN, file_name, LOSCOLOR_RESET);
		return true;
	}



	/*
	26_3_7
	- 初始化 工具函数
	*/
	void LuaManager::initUtil() {
		LOS_lua["P"] = [](sol::variadic_args args) {
			std::string output = "";
			for (auto arg : args) {
				output += arg.get<sol::object>().as<std::string>() + " ";
			}
			std::cout << output << std::endl;
			};
	}



	/*
	26_3_11
	- 初始化 森林事件
	*/
	void LuaManager::initCommonForestThings()
	{
		LOS_lua["Injured"] = [=](EntityId id, d down_hp) {
			auto& r = LOS_engine->getRegister();
			EntityId playerId = LOS_engine->getPlayerId();
			System::Injured(
				r,
				id,
				playerId,
				down_hp
			);
		};

		LOS_lua["FindSthInForest"] = [=](EntityId id, const ls& name, int number) {
			auto& r = LOS_engine->getRegister();
			auto* inventory = r.get<InventoryComponent>(id);
			if (nullptr == inventory)
			{
				P("{}there is wrong in FindSthInForest{}", LOSCOLOR_RED, LOSCOLOR_RESET);
				return;
			}
		};
	}



	/*
	- 初始化 组件
	*/
	void LuaManager::initComponent()
	{
		LOS_lua.new_usertype<Register>("cpp_class_register");
		
		LOS_lua["addPortable"] = [](Register& r, EntityId id, double size) {
		r.emplace<PortableComponent>(id, PortableComponent{
				.size = size });
		};

		LOS_lua["addInfo"] = [](Register& r, EntityId id, ls name, ls desc)
		{
				r.emplace<InfoComponent>(id, InfoComponent{ 
					.name = name , 
					.desc = desc
				});
		};
	}



	/*
	- 获取 state
	*/
	sol::state& LuaManager::getState()
	{
		return LOS_lua;
	}

}
