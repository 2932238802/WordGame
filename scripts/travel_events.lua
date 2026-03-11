math.randomseed(os.time())

sth_in_the_forest_road = {
	{ name = "branch", min = 2, max = 8 },
	{ name = "leaves", min = 3, max = 10 },
	{ name = "vines",  min = 1, max = 2 },
	{ name = "log",    min = 1, max = 3 }
}


TravelEvents = {
	{
		id = "find_sth_in_forest",
		weight = 30,
		condition = function(play_id, from_place, to_place, time_cost)
			return to_place == "forest" or to_place == "deep forest"
		end,
		execute = function(play_id)
			local random_index = math.random(1, #sth_in_the_forest_road)
			local reward = sth_in_the_forest_road[random_index]
			local amount = math.random(reward.min, reward.max)
			P("Gained something on the road: " .. reward.name .. " x" .. amount)
			FindSthInForest(play_id, reward.name, amount)
        end
	},

	{
		id = "sprain_ankle",
		weight = 10,
		condition = function( play_id,from_place,to_place,time_cost )
			return time_cost > 90
		end,
		execute = function(play_id)
			P("injured...on the road.")
			Injured(play_id,math.random(1,4));
        end
	},

	{
		id = "nothing",
		weight = 100,
		condition = function() return true end,
		execute = function(play_id)
			P("Nothing happened on the road.")
        end
	}
}

function Lua_HandleTravelEvent( play_id,from_place,to_place,time_cost )
	local possble_events = {}
	local total_weight = 0

	for i,event in ipairs(TravelEvents) do
		if event.condition(play_id,from_place,to_place,time_cost) then
			table.insert(possble_events,event);
			total_weight = total_weight + event.weight;
		end
	end

	if #possble_events == 0 then
		return
	end

	local roll = math.random(1,total_weight)
	local cursor = 0
	local selected_event = nil

	for i,event in ipairs(possble_events) do
		cursor = cursor + event.weight
		if roll <= cursor then
			selected_event = event
			break
		end
	end

	if selected_event then
		selected_event.execute(play_id)
	end

end

