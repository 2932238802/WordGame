#pragma once
#include "core/register.h"
#include "core/lua_manager.h"

#include "nlohmann/detail/macro_scope.hpp"
#include "nlohmann/json.hpp"

#include "common/constant.h"
#include "common/utities.h"
#include "entities/item.h"

#include "sol/sol.hpp"

#include <fstream>
#include <vcruntime_typeinfo.h>


namespace LosG {

	struct FoodInfo {
		ls name;
		ls desc;
		d hp_base;
		d hp_min;
		d hp_max;
		d energy_base;
		d energy_min;
		d energy_max;
		d mental_min;
		d mental_base;
		d mental_max;
		b composed;
		d size_base;
		d size_min;
		d size_max;
		d rot_rate;
	};

	struct Apple {};
	struct WholeWheatBread {};
	struct Orange {};

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(FoodInfo, name, desc, hp_base, hp_min,
		hp_max, energy_base, energy_min, energy_max,
		mental_min, mental_base, mental_max,
		composed, size_base, size_min, size_max,
		rot_rate)

		class ItemFactory {
		public:
			template <typename Food> EntityId createBaseFood(Register& r) {
				std::string foodName = getFoodName<Food>();
				if (!LOS_foodInfos.contains(foodName)) {
					P("can't find: {}", foodName);
					return -1;
				}
				auto foodConfig = LOS_foodInfos[foodName];
				auto foodId = r.create();
				r.emplace<InfoComponent>(foodId, InfoComponent{ .name = foodConfig.name,
																.desc = foodConfig.desc });
				r.emplace<EatAbleComponent>(
					foodId,
					EatAbleComponent{
						.hp = LosRand(foodConfig.energy_base, foodConfig.hp_min,
										foodConfig.hp_max),
						.energy = LosRand(foodConfig.energy_base, foodConfig.energy_min,
											foodConfig.energy_max),
						.mental = LosRand(foodConfig.mental_base, foodConfig.mental_min,
											foodConfig.mental_max)
					}
				);

				r.emplace<PortableComponent>(
					foodId, PortableComponent{ .size = LosRand(foodConfig.size_base,
																foodConfig.size_min,
																foodConfig.size_max)
					});

				r.emplace<PerishComponent>(foodId, PerishComponent{
														.max_freshness = 100.0,
														.cur_freshness = 100.0,
														.rot_rate = foodConfig.rot_rate,
														.is_rotten = false, });
				return foodId;
			}
			static ItemFactory& instance();
			Item getItem(const ls& name) const;
		private:
			ItemFactory();
			ItemFactory(const ItemFactory&) = delete;
			ItemFactory(ItemFactory&&) = delete;
			ItemFactory& operator=(const ItemFactory&) = delete;
			EntityId createItem(Register& r, lsw item_str);
			void initFood();
			void initItem();
			void initLuaFunc();
			void initHandle(lsw name, Item& item);

			std::unordered_map<ls, FoodInfo> LOS_foodInfos;
			std::unordered_map<ls, Item> LOS_itemInfos;
			sol::protected_function LOS_luaInitItemFunc;
			template <typename T> std::string getFoodName() { return "未知食物"; }
		};

	template <> inline std::string ItemFactory::getFoodName<Apple>() {
		return "apple";
	}
	template <> inline std::string ItemFactory::getFoodName<WholeWheatBread>() {
		return "whole-wheat bread";
	}
	template <> inline std::string ItemFactory::getFoodName<Orange>() {
		return "orange";
	}
} // namespace LosG
