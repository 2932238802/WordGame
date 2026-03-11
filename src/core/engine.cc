#include "core/engine.h"

#include <future>
#include <stack>

namespace LosG {
	/*
	26_2_25
	- create Engine()
	26_2_26
	- init player
	*/
	Engine::Engine() : LOS_run(true) {
		LOS_startTime = std::chrono::steady_clock::now();
	}



	/*
	26_2_25
	1. create run()
	2. 根据 LOS_run loop
	26_2_28
	- exit Issue

	26_3_5
	- add perish
	- add weather change
	*/
	void Engine::run() {
		auto input = std::async(std::launch::async, &Engine::getInput);
		formatPrint();
		lastBeginTime = std::chrono::steady_clock::now();
		while (LOS_run) {
			lastEndTime = std::chrono::steady_clock::now();
			std::chrono::duration<double> diff = lastEndTime - lastBeginTime;
			lastBeginTime = lastEndTime;
			double dt = diff.count();	// real time

			auto* timeCom = LOS_register.get<WorldTimeComponent>(LOS_worldId);
			auto* envCom = LOS_register.get<WorldEnvironmentComponent>(LOS_worldId);
			d totalGameTime = dt * 0.4;
			timeCom->total_game_minite += totalGameTime;	// update game time

			System::Update(
				LOS_register,
				totalGameTime,
				timeCom
			);
			System::UpdatePerish(LOS_register,
				LOS_playerId,
				totalGameTime,
				timeCom,
				envCom
			);
			System::UpdateWeather(
				LOS_register,
				LOS_worldId,
				totalGameTime
			);
			auto status = input.wait_for(std::chrono::milliseconds(10));
			if (std::future_status::ready == status) {
				std::string line = input.get().data();
				if ("<EOF>" == line) {
					LOS_run = false;
				}
				else {
					exe(line);
				}
				if (LOS_run) {
					formatPrint();
					input = std::async(std::launch::async, &Engine::getInput);
				}
			}
		}
	}



	EntityId Engine::getPlayerId() const
	{
		return LOS_playerId;
	}


	/*
	26_3_10
	- 获取注册表
	*/
	Register& Engine::getRegister()
	{
		return LOS_register;
	}



	/*
	3_9
	- 初始化 改为手动
	*/
	void Engine::init()
	{
		initWorld();
		World::init();
		initPlayer();
		initCommands();            // 初始化指令
	}



	/*
	26_2_26
	- formatPrint
	*/
	void Engine::formatPrint() {
		auto* timeCom = LOS_register.get<WorldTimeComponent>(LOS_worldId);
		auto* weCom = LOS_register.get<WorldEnvironmentComponent>(LOS_worldId);
		auto* loc = LOS_register.get<PositionComponent>(LOS_playerId);
		if (!timeCom || !weCom) return;
		int day = timeCom->get_day();
		int hour = timeCom->get_hour();
		int minute = timeCom->get_min();
		Season s = timeCom->getSeason();
		std::string placeName = loc ? GetPlaceName(loc->place) : "Unknown";
		std::cout << "\r"
			<< std::format("[{} | {}:{} Day {} {:02}:{:02}] > ",
				placeName,
				GetSeasonName(s),
				GetWeatherStr(weCom->current_weather),
				day,
				hour,
				minute)
			<< std::flush;
	}



	/*
	26_2_25
	- exe
	*/
	void Engine::exe(lsw line) {
		std::smatch match;
		std::string s(line);
		bool find = false;
		for (const auto& cm : LOS_commands) {
			if (std::regex_match(s, match, cm.reg)) {
				cm.handle(match);
				find = true;
				break;
			}
		}
		if (!find)
			P("no...");
	}



	/*
	26_2_26
	- async
	*/
	std::string Engine::getInput() {
		std::string line;
		if (std::getline(std::cin, line)) {
			return line;
		}
		return "<EOF>";
	}

	/*
	26_2_26
	- show id
	*/
	void Engine::showState(EntityId id_) {
		auto* i = LOS_register.get<InfoComponent>(id_);
		auto* body = LOS_register.get<HpComponent>(id_);
		auto* santity = LOS_register.get<SanityComponent>(id_);
		auto* hungry = LOS_register.get<HungerComponent>(id_);
		auto* velocity = LOS_register.get<VelocityComponent>(id_);
		if (nullptr == i) {
			P("showstate 找不到对应的 id_: {}", id_);
			return;
		}

		P("============ state begin ============");
		P("[{}]\n", i->name);
		if (nullptr != body) {
			P("[hp]:{:.2f}\n ", body->hp);
		}
		if (nullptr != santity) {
			P("[santity]:{:.2f}\n", santity->value);
		}
		if (nullptr != hungry) {
			P("[hungry]:{:.2f}\n", hungry->value);
		}
		if (nullptr != velocity) {
			P("[velocity]:{:.2f}", velocity->velocity);
		}
		P("============ state end ============");
	}



	/*
	26_2_28
	- init command
	*/
	void Engine::initCommands() {

		// exit
		LOS_commands.push_back({
			std::regex(EXE_EXIT_PARAM.data(), std::regex_constants::icase),
			[this](const std::smatch& sm) { this->exitCommand(sm); },
			R"doc([exit] exit)doc"
			});

		// hello
		LOS_commands.push_back({
			std::regex(EXE_HELLO_PARAM.data(), std::regex_constants::icase),
			[this](const std::smatch& sm) { this->helloCommand(sm); },
			R"doc([hello] call
[hi])doc"
			});

		// state
		LOS_commands.push_back({
			std::regex(EXE_STATE_PARAM.data(), std::regex_constants::icase),
			[this](const std::smatch& sm) { this->stateCommand(sm); },
			R"doc([state] [id] show player's state)doc"
			});

		// where
		LOS_commands.push_back({
			std::regex(EXE_WHERE_PARAM.data(), std::regex_constants::icase),
			[this](const std::smatch& sm) { this->whereCommand(sm); },
			R"doc([where] show where are you)doc"
			});

		// map
		LOS_commands.push_back({
			std::regex(EXE_MAP_PARAM.data(), std::regex_constants::icase),
			[this](const std::smatch& sm) { this->mapCommand(sm); },
			R"doc([map] show map)doc"
			});

		// bag (26_3_5 add fresh)
		LOS_commands.push_back({
			std::regex(EXE_BAG_PARAM.data(), std::regex_constants::icase),
			[this](const std::smatch& sm) { this->bagCommand(sm); },
			R"doc([bag] show sth your bags have)doc"
			});

		// help
		LOS_commands.push_back({
			std::regex(EXE_HELP_PARAM.data(), std::regex_constants::icase),
			[this](const std::smatch& sm) { this->helpCommand(sm); },
			R"doc([help] help list 
[h])doc"
			});

		// eat
		LOS_commands.push_back({
			std::regex(EXE_EAT_PARAM.data(), std::regex_constants::icase),
			[this](const std::smatch& sm) { this->eatCommand(sm); },
			R"doc([eat] <foot-index>    
eat the food of the id you choose)doc"
			});

		// clear
		LOS_commands.push_back({
			std::regex(EXE_CLS_PARAM.data(), std::regex_constants::icase),
			[this](const std::smatch& sm) { this->clearCommand(sm); },
			R"doc([clear] clear the screen
[cls])doc"
			});

		// go
		LOS_commands.push_back({
			std::regex(EXE_GO_PARAM.data(), std::regex_constants::icase),
			[this](const std::smatch& sm) { this->goCommand(sm); },
			R"doc([go|went|g] PlaceStr
go to place)doc"
			});

		// make
		LOS_commands.push_back({
			std::regex(EXE_MAKE_PARAM.data(), std::regex_constants::icase),
			[this](const std::smatch& sm) { this->makeCommand(sm); },
			// 注意这里原来没有help文档，如果你需要可以加上
			""
		});

	}



	/*
	26_2_28
	- 初始化玩家

	26_3_2
	- 增加容量的逻辑
	*/
	void Engine::initPlayer() {
		LOS_playerId = LOS_register.create();
		LOS_register.emplace<HpComponent>(LOS_playerId, HpComponent{ 100, 100 });
		LOS_register.emplace<HungerComponent>(LOS_playerId,
			HungerComponent{ 100, 100, 0.05, 0.5 });
		LOS_register.emplace<SanityComponent>(LOS_playerId,
			SanityComponent{ 100, 0.002, 100.0 });
		LOS_register.emplace<PositionComponent>(LOS_playerId,
			PositionComponent{ Place::HOME });
		LOS_register.emplace<InfoComponent>(LOS_playerId,
			InfoComponent{ "LosAngelous", "I'm linshengjie" });
		LOS_register.emplace<VelocityComponent>(LOS_playerId,
			VelocityComponent{ .velocity = 2.1 });
		LOS_register.emplace<InventoryComponent>(LOS_playerId,
			InventoryComponent{ {}, 20 });
		LOS_register.emplace<KnowledgeComponent>(LOS_playerId,
			KnowledgeComponent{
				{"torch","handle","plank","rope","bag_a"}
			});

		EnVec vec;
		for (int i = 0; i < 3; i++) {
			auto appId = ItemFactory::instance().createBaseFood<Apple>(LOS_register);
			vec.push_back(appId);
			System::UpdateInventory(LOS_register, LOS_playerId, appId, true);
		}
		auto orangeId = ItemFactory::instance().createBaseFood<Orange>(LOS_register);
		vec.push_back(orangeId);
		System::UpdateInventory(LOS_register, LOS_playerId, orangeId, true);
	}



	/*
	26_3_5
	- init world
	*/
	void Engine::initWorld()
	{
		LOS_worldId = LOS_register.create();
		LOS_register.emplace<WorldTimeComponent>(LOS_worldId,
			WorldTimeComponent{
			.total_game_minite = 0.0,
			}
			);
		LOS_register.emplace<WorldEnvironmentComponent>(LOS_worldId,
			WorldEnvironmentComponent{
			.current_weather = Weather::Clear,
			.durationTime = 60 * 2
			}
		);
	}



	void Engine::shiftGameTime(d game_time_to_add) {
		auto* timeComp = LOS_register.get<WorldTimeComponent>(LOS_worldId);
		auto* envComp = LOS_register.get<WorldEnvironmentComponent>(LOS_worldId);
		if (timeComp) {
			timeComp->total_game_minite += game_time_to_add;
			System::Update(LOS_register, game_time_to_add, timeComp);
			System::UpdatePerish(
				LOS_register,
				LOS_playerId,
				game_time_to_add,
				timeComp,
				envComp);
			System::UpdateWeather(
				LOS_register,
				LOS_worldId,
				game_time_to_add
			);
		}
	}



	void Engine::exitCommand(const std::smatch& sm) {
		this->LOS_run = false;
		P("bye~ : {}", LOS_register.get<InfoComponent>(LOS_playerId)->name);
	}



	void Engine::helloCommand(const std::smatch& sm) {
		P("hi ~");
		P(LOS_register.get<InfoComponent>(LOS_playerId)->name);
	}



	void Engine::stateCommand(const std::smatch& sm) {
		std::string idStr = sm[2].str();
		EntityId id = this->LOS_playerId;
		if (!idStr.empty()) {
			try {
				id = std::stoi(idStr);
			}
			catch (...) {
				P("Invalid ID: {}", idStr);
				return;
			}
		}
		this->showState(id);
	}



	void Engine::whereCommand(const std::smatch& sm) {
		P("[where] : {}", GetPlaceName(LOS_register.get<PositionComponent>(LOS_playerId)->place));
	}



	void Engine::mapCommand(const std::smatch& sm) {
		World::render(LOS_register.get<PositionComponent>(LOS_playerId)->place);
	}



	void Engine::bagCommand(const std::smatch& sm) {
		std::stack<std::pair<int, int>> bagStack;
		bagStack.push({ LOS_playerId, 1 });
		int index = 1;
		while (!bagStack.empty()) {
			auto top = bagStack.top();
			auto bag = LOS_register.get<InventoryComponent>(top.first);
			bagStack.pop();
			for (auto id : bag->items) {
				if (LOS_register.has<InventoryComponent>(id))
					bagStack.push({ id, top.second + 1 });
				auto itemInfo = LOS_register.get<InfoComponent>(id);
				ls indent = "";
				if (nullptr != itemInfo) {
					for (int s = 0; s < top.second; ++s) {
						indent += "  ";
					}
					auto isFood = LOS_register.get<EatAbleComponent>(id);
					if (nullptr == isFood) {
						P("{}{}- [item-id -> {}].{}:{}", index, indent, id,
							itemInfo->name, itemInfo->desc);
					}
					else {
						auto isPerished = LOS_register.get<PerishComponent>(id);
						if (isPerished) {
							P("{}{}- [eat-id -> {}].{}[fresh:{:.2f}]:{}",
								index, indent, id, itemInfo->name,
								isPerished->cur_freshness, itemInfo->desc);
						}
						else {
							P("{}{}- [item-id -> {}].{}:{}", index, indent, id,
								itemInfo->name, itemInfo->desc);
						}
					}
				}
			}
		}
	}



	void Engine::helpCommand(const std::smatch& sm) {
		P("====== help BEGIN======");
		for (const auto& cmd : LOS_commands) {
			P("{}", cmd.help);
		}
		P("====== help END======");
	}



	void Engine::eatCommand(const std::smatch& sm) {
		auto foodid = sm[2];
		i foodId = stoi(foodid);
		System::UpdateInventory(LOS_register, LOS_playerId, foodId, false);
		System::UpdateFood(LOS_register, LOS_playerId, foodId);
	}



	void Engine::clearCommand(const std::smatch& sm) {
		system("cls");
		formatPrint();
	}



	void Engine::goCommand(const std::smatch& sm) {
		auto placeStrValue = sm[2];
		auto placeStr = placeStrValue.str();
		bool b;
		Place to = World::strToPlace(placeStr, b);
		if (b == false) {
			P("unkown Place");
			return;
		}
		auto position = LOS_register.get<PositionComponent>(LOS_playerId);
		auto velocity = LOS_register.get<VelocityComponent>(LOS_playerId);
		if (position && velocity && to != position->place) {
			double t = World::go(position->place, to, velocity->velocity);
			ls input;
			std::smatch input_sm;
			P("go to there maybe need {} time, are you sure(yes/y/ye/...)", t);
			std::getline(std::cin, input);
			if (std::regex_match(input, input_sm, std::regex(YES_PARAM.data()))) {
				P("go there...");
				std::this_thread::sleep_for(std::chrono::seconds(1));
				ls fromPlaceStr = GetPlaceName(position->place);
				ls toPlaceStr = GetPlaceName(to);
				auto func = LuaManager::instance().getState()["Lua_HandleTravelEvent"];
				if (func.valid())
				{
					auto res = func(LOS_playerId, fromPlaceStr, toPlaceStr, t);
					if (!res.valid())
					{
						sol::error err = res;
						P("{}路途事件 Lua 脚本执行报错: {}{}", LOSCOLOR_RED, err.what(), LOSCOLOR_RESET);
					}
				}
				else {
					P("{}Lua_HandleTravelEvent查找错误{}",LOSCOLOR_RED,LOSCOLOR_RESET);
				}
				shiftGameTime(t);
				World::goTo(LOS_register, LOS_playerId, to);
			}
			else {
				P("player cancel this action");
			}
		}
		else {
			P("position is nullptr or velocity is nullptr or to == position->place");
		}
	}



	void Engine::makeCommand(const std::smatch& sm) {
		if (sm[2].str().empty()) {
			auto* knowledges = LOS_register.get<KnowledgeComponent>(LOS_playerId);
			if (nullptr != knowledges)
			{
				for (auto a : knowledges->konwledges)
				{
					auto item = ItemFactory::instance().getItem(a);
					P("[{}]:{}", item.name, item.desc);
					for (auto itemson : item.ingredients)
					{
						P("  - {}({})", itemson.item_id,itemson.amount);
					}
				}
				P("");
			}
		}
		else {
			try {
				int itemId = stoi(sm[2].str());
				auto* knowledges = LOS_register.get<KnowledgeComponent>(LOS_playerId);
				auto itemStr = knowledges->konwledges.at(itemId);
				ls input;
				P("are you sure to make {} (yes/ye/y/...)", itemStr);
				std::getline(std::cin, input);
				std::smatch input_sm;
				if (!std::regex_match(input, input_sm, std::regex(YES_PARAM.data()))) {
					P("Production has been cancelled... ");
					return;
				}
			}
			catch (const std::invalid_argument& e) {
				P("Enter the item index to be obtained (the index displayed by make) {}", e.what());
				return;
			}
			catch (const std::out_of_range& e) {
				P("exceeded index position, please re-enter");
				return;
			}
		}
	}

	

	/*
	26_3_11
	- 做一些事情
	*/
	void Engine::doCommand(const std::smatch& sm)
	{


	}
};
