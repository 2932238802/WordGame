
#include "factories/item_factory.h"


namespace LosG {
	/*
	26_3_3
	- 单例模式
	*/
	ItemFactory& ItemFactory::instance() {
		static ItemFactory fa;
		return fa;
	}

	ItemFactory::ItemFactory() {
		initLuaFunc();
		initFood();
		initItem();
	}

	/*
	26_3_3
	- 获取本地的 json文件

	26_3_4
	- 发现读取问题 进行修复
	*/
	void ItemFactory::initFood() {
		std::string path = "./assets/foodData.json";
		std::fstream f(path);

		if (!f.is_open()) {
			path = "../assets/foodData.json";
			f.open(path);
		}

		if (!f.is_open()) {
			path = "../../assets/foodData.json";
			f.open(path);
		}

		if (!f.is_open()) {
			P("Unable to find data.json");
			return;
		}

		nlohmann::json j;
		try {
			f >> j;

			if (!j.is_array()) {
				P("data.json format error, the root node should be an array");
				return;
			}
			LOS_foodInfos.clear();
			for (const auto& food_json : j) {
				try {
					FoodInfo food = food_json.get<FoodInfo>();
					LOS_foodInfos[food.name] = food;
					P("Loading food: {}", food.name);
				}
				catch (const nlohmann::json::exception& e) {
					P("Failed to parse food data: {}", e.what());
					continue;
				}
			}
			P("Loaded a total of {} kinds of food", LOS_foodInfos.size());
		}
		catch (nlohmann::json::exception& e) {
			P("JSON parsing failed: {}", e.what());
		}
		P("{}load foodData.json suc{}", LOSCOLOR_GREEN, LOSCOLOR_RESET);
	}



	/*
	26_3_5
	- 初始化item
	*/
	void ItemFactory::initItem()
	{
		std::string path = "./assets/itemData.json";
		std::fstream f(path);

		if (!f.is_open()) {
			path = "../assets/itemData.json";
			f.open(path);
		}

		if (!f.is_open()) {
			path = "../../assets/itemData.json";
			f.open(path);
		}

		if (!f.is_open()) {
			P("Unable to find data.json");
			return;
		}

		try
		{
			nlohmann::json data = nlohmann::json::parse(f);
			for (auto& item : data)
			{
				Item i;
				i.name = item.value("name", "unknown");
				initHandle(i.name, i);
				i.desc = item.value("desc", "");
				i.is_craftable = item.value("is_craftable", false);
				if (item.contains("ingredients"))
				{
					for (auto& ing : item["ingredients"])
					{
						i.ingredients.push_back(
							IngredientRequirement
							{
								.item_id = ing.value("id", ""),
								.amount = ing.value("amount", 0)
							}
						);
					}
				}
				LOS_itemInfos[i.name] = i;
			}
		}
		catch (const std::exception& e)
		{
			P("data JSON Parsing Error: {}", e.what());
		}

		P("{}load itemData.json suc{}", LOSCOLOR_GREEN, LOSCOLOR_RESET);
	}


	/*
	3_7
	- 初始化 lua 脚本的函数
	*/
	void ItemFactory::initLuaFunc()
	{
		LOS_luaInitItemFunc = LuaManager::instance().getState()["InitItemComponents"];
	}



	/*
	36_3_6
	- 初始化 物件的 handle
	*/
	void ItemFactory::initHandle(lsw name, Item& item) {
		auto& state = LuaManager::instance().getState();
		sol::table configs = state["ItemConfigs"];
		
		if (configs[name].valid())
		{
			item.handle = [this, itemName = std::string(name)](Register& r, EntityId id) {

				if (LOS_luaInitItemFunc.valid())
				{
					auto res = LOS_luaInitItemFunc(std::ref(r), id, itemName);

					if (!res.valid())
					{
						sol::error e = res;
						P("{}lua handle error for{}:{}{}", LOSCOLOR_RED, itemName,e.what(), LOSCOLOR_RESET);
					}
				}
			};
		}
	}


	/*
	26_3_10
	- 获取物品的信息
	*/
	Item ItemFactory::getItem(const ls& name) const
	{
		return LOS_itemInfos.at(name);
	}



	/*
	26_3_5
	- 创建物品
	*/
	EntityId ItemFactory::createItem(Register& r, lsw item_str)
	{
		if (LOS_itemInfos.find(ls(item_str)) == LOS_itemInfos.end())
		{
			P("LOS_itemInfos 没有意义");
			return -1;
		}

		const auto& def = LOS_itemInfos[ls(item_str)];
		auto id = r.create();

		r.emplace<InfoComponent>(id, InfoComponent{
				.name = def.name,
				.desc = def.desc
			});

		if(def.handle != nullptr) def.handle(r, id);
		return id;
	}
} // namespace LosG
