#pragma once

#include "common/constant.h"

#include "factories/item_factory.h"

#include "core/world.h"
#include "core/register.h"
#include "core/system.h"
#include "core/register.h"

#include "entities/body.h"
#include "entities/combat.h"
#include "entities/item.h"
#include "entities/map.h"
#include "entities/world_time.h"
#include "entities/basic.h"
#include "entities/cmd.h"

#include <vector>
#include <chrono>

namespace LosG {
	/*
	26_2_25
	- create
	*/
	class Engine {
	public:
		explicit Engine();
		void run();                   
		EntityId getPlayerId() const;
		Register& getRegister();
		void init();

	private:
		void formatPrint();
		void exe(lsw line);         
		static std::string getInput(); 
		void showState(EntityId id_);
		void initCommands();           
		void initPlayer();
		void initWorld();
		void shiftGameTime(d game_time);
		void exitCommand(const std::smatch& sm);
		void helloCommand(const std::smatch& sm);
		void stateCommand(const std::smatch& sm);
		void whereCommand(const std::smatch& sm);
		void mapCommand(const std::smatch& sm);
		void bagCommand(const std::smatch& sm);
		void helpCommand(const std::smatch& sm);
		void eatCommand(const std::smatch& sm);
		void clearCommand(const std::smatch& sm);
		void goCommand(const std::smatch& sm);
		void makeCommand(const std::smatch& sm);
		void doCommand(const std::smatch& sm);

	private:
		b LOS_run;
		EntityId LOS_playerId;
		EntityId LOS_worldId;
		Register LOS_register;
		std::vector<Command> LOS_commands;


		std::chrono::steady_clock::time_point LOS_startTime;
		std::chrono::steady_clock::time_point lastBeginTime;
		std::chrono::steady_clock::time_point lastEndTime;
	};
};
