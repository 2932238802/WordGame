
#include "core/engine.h"
#include "core/lua_manager.h"
#include <windows.h> 

using namespace LosG;
int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::ios_base::sync_with_stdio(false);

    Engine e;
    LuaManager::instance().init(&e);

	if (!LuaManager::instance().loadScript(findItemConfigLua("item_config.lua")))
	{
		P("item_config.lua º”‘ÿ ß∞‹!");
        return 0;
	}

    if (!LuaManager::instance().loadScript(findItemConfigLua("travel_events.lua")))
    {
        P("travel_events.lua º”‘ÿ ß∞‹!");
        return 0;
    }
    e.init();
    e.run();
    return 0;
}