

#pragma once

#include "common/constant.h"
#include <optional>
#include <algorithm>
#include <string_view>

#include "entities/body.h"
#include "entities/combat.h"
#include "entities/item.h"
#include "entities/world_time.h"

#include "register.h"

namespace LosG {

	class System {
	public:
		static void Update(
			Register& reg,
			double game_minute,
			WorldTimeComponent* timeCmp);
		static MapIdToT<bool> UpdateHungry(
			Register& r,
			d game_minute
		);

		static void UpdateSantity(
			Register& r,
			d game_minute);

		static bool UpdateInventory(
			Register& r,
			EntityId containId,
			EntityId itemId,
			bool b);

		static void UpdateFood(
			Register& r,
			EntityId who_eat,
			EntityId eat_item_id);

		static void UpdatePerish(
			Register& r,
			EntityId& bag_id,
			d game_minute,
			WorldTimeComponent* time_cmp,
			WorldEnvironmentComponent* envi_cmp
		);

		static void UpdateWeather(
			Register& r,
			EntityId world_id,
			d game_minute
		);

		static void Injured(
			Register& r,
			EntityId item_id,
			EntityId player_id,
			d down_hp
		);
	};

} // namespace LosG