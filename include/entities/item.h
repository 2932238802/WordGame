#pragma once
#include "common/constant.h"
#include "core/register.h"

/*
26_3_1
- 创建

26_3_2
- 食物的腐烂属性 增加
*/
namespace LosG {

	// 信息组件
	struct InfoComponent {
		ls name = "unkown";
		ls desc = "";
	};

	// 可使用组件
	struct EatAbleComponent {
		d hp;
		d energy;
		d mental;
	};

	// 能造成伤害属性
	struct HarmAbleComponent {
		d damage;
	};

	// 可携带的物品的 属性
	struct PortableComponent {
		d size;                   // 可携带物品的空间
	};

	// 食物的腐烂属性
	struct PerishComponent {
		d max_freshness;
		d cur_freshness;
		d rot_rate;
		b is_rotten;
	};

} // namespace LosG



/*
26_3_5
- 合成物
*/
namespace LosG {
	// 合成物品 和 数量
	struct IngredientRequirement
	{
		ls item_id;
		i amount;
	};

	// 物品
	using func_for_component_t = std::function<void(Register&, EntityId)>;
	struct Item {
		ls name;
		ls desc;
		bool is_craftable;
		std::vector<struct IngredientRequirement> ingredients;
		func_for_component_t handle;
	};
};