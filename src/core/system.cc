#include "core/system.h"



namespace LosG {
	/*
	26_3_4
	- game_minute
	*/
	void System::Update(Register& reg, double game_minute, WorldTimeComponent* timeCmp) {
		UpdateHungry(reg, game_minute);
		UpdateSantity(reg, game_minute);
	}

	/*
	26_3_4
	- Update hunger level
	*/
	MapIdToT<bool> System::UpdateHungry(Register& r, d game_minute) {
		auto entitiesIds = r.group<HungerComponent, HpComponent>();
		if (entitiesIds.empty())
			return {};
		MapIdToT<bool> res;
		for (auto id : entitiesIds) {
			auto hungryCom = r.get<HungerComponent>(id);
			auto hpCom = r.get<HpComponent>(id);
			hungryCom->value = hungryCom->value - hungryCom->decay_rate * game_minute;
			if (hungryCom->value < 0) {
				d downHpTime = (-1.0) * hungryCom->value / hungryCom->decay_rate;
				hpCom->hp -= hungryCom->decay_hp_rate * downHpTime;
				hungryCom->value = 0;
				if (hpCom->hp <= 0) {
					hpCom->hp = 0;
					res[id] = false;
					continue;
				}
			}
			res[id] = true;
		}
		return res;
	}



	/*
	26_2_28
	- Update mental state
	*/
	void System::UpdateSantity(Register& r, d game_minute) {
		auto entitiesIds = r.group<SanityComponent>();
		for (auto id : entitiesIds) {
			auto sanity = r.get<SanityComponent>(id);

			sanity->value -= sanity->decay_rate * game_minute;
			if (sanity->value < 0.0)
				sanity->value = 0.0;
			if (sanity->value > 100.0)
				sanity->value = 100.0;

			d curValue = sanity->value;

			bool isDead = (curValue <= 0.001);
			bool isDizzy = (curValue > 0.001 && curValue <= 20.0);
			bool isDrowsy = (curValue > 20.0 && curValue <= 50.0);
			bool isNormal = (curValue > 50.0);
			if (isDead) {
				r.emplace<DiedTag>(id, DiedTag{});
			}

			if (isDizzy) {
				if (!r.has<DizzyTag>(id))
					r.emplace<DizzyTag>(id, {});
				if (r.has<DrowsyTag>(id))
					r.remove<DrowsyTag>(id);
			}

			else if (isDrowsy) {
				if (!r.has<DrowsyTag>(id))
					r.emplace<DrowsyTag>(id, {});
				if (r.has<DizzyTag>(id))
					r.remove<DizzyTag>(id);
			}

			else if (isNormal) {
				if (r.has<DizzyTag>(id))
					r.remove<DizzyTag>(id);
				if (r.has<DrowsyTag>(id))
					r.remove<DrowsyTag>(id);
			}
		}
	}


	/*
	26_3_2
	- Update backpack system
	*/
	bool System::UpdateInventory(
		Register& r, 
		EntityId containId, 
		EntityId itemId,
		bool b) {
		auto* inventory = r.get<InventoryComponent>(containId);
		auto* item = r.get<PortableComponent>(itemId);
		if (!inventory || !item)
			return false;
		if (b) {
			if (inventory->curSize + item->size > inventory->maxSize)
				return false;
			inventory->items.push_back(itemId);
			inventory->curSize += item->size;
		}
		else {
			auto pos =
				std::find(inventory->items.begin(), inventory->items.end(), itemId);
			if (pos != inventory->items.end()) {
				inventory->curSize = inventory->curSize - item->size;
				std::swap(inventory->items.back(), *pos);
				inventory->items.pop_back();
			}
		}
		return true;
	}


	/*
	26_3_4
	update food
	*/
	void System::UpdateFood(
		Register& r, 
		EntityId who_eat, 
		EntityId eat_item_id
	) {
		auto eatAble = r.get<EatAbleComponent>(eat_item_id);
		auto perishAble = r.get<PerishComponent>(eat_item_id);
		if (!eatAble || !perishAble) {
			return;
		}
		auto hpCom = r.get<HpComponent>(who_eat);
		auto huCom = r.get<HungerComponent>(who_eat);
		auto saCom = r.get<SanityComponent>(who_eat);
		auto pState = [&](d val, lsw s) {
			if (val >= 0) {
				P("{} +- get {} {}{}", LOSCOLOR_GREEN, val, s, LOSCOLOR_RESET);
			}
			else {
				P("{} +- get {} {}{}", LOSCOLOR_RED, val, s, LOSCOLOR_RESET);
			}
			};

		double fresh = perishAble->cur_freshness / perishAble->max_freshness;
		if (hpCom) {
			auto nowHp = std::clamp(hpCom->hp + (fresh * eatAble->hp), 0.0, hpCom->max);
			pState(nowHp - hpCom->hp, "hp");
			hpCom->hp = nowHp;
		}
		if (huCom) {
			auto nowHu =
				std::clamp(huCom->value + (fresh * eatAble->energy), 0.0, huCom->max);
			pState(nowHu - huCom->value, "hungry");
			huCom->value = nowHu;
		}
		if (saCom) {
			auto nowSa =
				std::clamp(saCom->value + (fresh * eatAble->mental), 0.0, saCom->max);
			pState(nowSa - saCom->value, "santity");
			saCom->value = nowSa;
		}
	}


	/*
	26_3_5
	update perish
	*/
	void System::UpdatePerish(
		Register& r,
		EntityId& bag_id,
		d game_minute,
		WorldTimeComponent* time_cmp,
		WorldEnvironmentComponent * envi_cmp
	) {
		auto* bag = r.get<InventoryComponent>(bag_id);
		if (nullptr == bag)
		{
			P("{}背包元素为空{}", LOSCOLOR_RED, LOSCOLOR_RESET);
			return;
		}
		d perishMuld = getPerishMult(time_cmp->getSeason(), envi_cmp->current_weather);
		for (auto id : bag->items)
		{
			auto perishCom = r.get<PerishComponent>(id);
			if (nullptr != perishCom)
			{
				auto d = perishCom->rot_rate * perishMuld * game_minute;
				perishCom->cur_freshness = std::clamp(perishCom->cur_freshness - d,0.0,100.0);
			}
		}
	}


	/*
	26_3_5
	weather update
	*/
	void System::UpdateWeather(
		Register& r,
		EntityId world_id,
		d game_minute
	) {
		auto* envCom = r.get<WorldEnvironmentComponent>(world_id);
		auto* timeCom = r.get<WorldTimeComponent>(world_id);
		if (envCom == nullptr || timeCom == nullptr) return;

		int dt = (envCom->durationTime -= game_minute);
		if (dt < 0)
		{
			P("�л�����");
			envCom->durationTime = 0;
			double duration = LosRandUniform(60 * 2, 60 * 24);
			envCom->durationTime = duration + dt;
			envCom->current_weather = getRandomWeather(timeCom->getSeason());
		}
	}


	void System::Injured(
		Register& r,
		EntityId item_id,
		EntityId player_id,
		d down_hp
	) {
		auto hpComponent = r.get<HpComponent>(item_id);
		if (nullptr == hpComponent)
		{
			P("{}id of {} has no HpComponent{}",LOSCOLOR_RED, item_id,LOSCOLOR_RESET);
			return;
		}

		// 受伤 还是 死亡
		if ((hpComponent->hp - down_hp) < 0)
		{
			// 死了 那就 加一个死亡的标签
			r.emplace<DiedTag>(item_id, DiedTag{});
		}
		else {
			hpComponent->hp -= down_hp;
			if (player_id == item_id)
			{
				P("{}id of {} injured: {} {}", LOSCOLOR_RED , item_id ,  down_hp, LOSCOLOR_RESET);
			}
			else {
				P("id of {} injured: {} ", item_id, down_hp);
			}
		}
	}
}; // namespace LosG