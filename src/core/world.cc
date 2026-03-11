#include "core/world.h"
#include "common/constant.h"
#include "entities/map.h"
#include <cmath>
#include <format>
#include <unordered_map>

namespace LosG {
std::unordered_map<Place, WorldNode> World::LOS_worldNodes;

void World::init() {

  LOS_worldNodes[Place::HOME] = {.name = "home", .icon = 'H', .x = 7, .y = 7};

  LOS_worldNodes[Place::CAVE] = {.name = "cave", .icon = 'C', .x = 3, .y = 12};

  LOS_worldNodes[Place::DEEP_FOREST] = {
      .name = "deep forest", .icon = 'D', .x = 5, .y = 3};
  LOS_worldNodes[Place::LAKE] = {.name = "lake", .icon = 'L', .x = 4, .y = 8};
  LOS_worldNodes[Place::PLAINS] = {
      .name = "plains", .icon = 'P', .x = 7, .y = 9};
  LOS_worldNodes[Place::SWAMP] = {
      .name = "swamp", .icon = 'S', .x = 5, .y = 12};
  LOS_worldNodes[Place::FOREST] = {
      .name = "forest", .icon = 'F', .x = 6, .y = 4};
}

/*
26_3_3
- go
*/
d World::go(Place from, Place to, d v) {
  auto fromNode = LOS_worldNodes[from];
  auto toNode = LOS_worldNodes[to];
  d x = fromNode.x - toNode.x;
  d y = fromNode.y - toNode.y;
  d distance = sqrt((x * x) + (y * y));
  d t = (distance / v) ;
  P("need {} min" , t);
  return t * 60;
}

/*
26_3_3
- render
*/
void World::render(Place where) {
  std::string grid[15][15];

  for (int i = 0; i < 15; i++) {
    for (int j = 0; j < 15; j++) {
      grid[i][j] = "  ";
    }
  }

  for (auto const &[place, node] : LOS_worldNodes) {
    if (node.x >= 0 && node.x < 15 && node.y >= 0 && node.y < 15) {
      if (place == where) {
        grid[node.x][node.y] =
            std::format("{}{} {}", LOSCOLOR_GREEN, node.icon, LOSCOLOR_RESET);
        continue;
      }
      grid[node.x][node.y] = std::format("{} ", node.icon);
    }
  }

  for (int i = 0; i < 15; i++) {
    for (int j = 0; j < 15; j++) {
      std::cout << std::format("{}", grid[i][j]);
    }
    P("");
  }
  P("Current Location: {}", LOS_worldNodes[where].name);
}

/*
26_3_4
go to
*/
void World::goTo(Register &r, EntityId id, Place p) {
  auto player = r.get<PositionComponent>(id);
  player->place = p;
}

/*
26_3_4
str to place
*/
Place World::strToPlace(std::string_view s , bool& b) {
  b = true;
  if (s == "C" || s == "c") {
    return Place::CAVE;
  } else if (s == "D" || s == "d") {
    return Place::DEEP_FOREST;
  } else if (s == "F" || s == "f") {
    return Place::FOREST;
  } else if (s == "H" || s == "h") {
    return Place::HOME;
  } else if (s == "L" || s == "l") {
    return Place::LAKE;
  } else if (s == "P" || s == "p") {
    return Place::PLAINS;
  } else if (s == "S" || s == "s") {
    return Place::SWAMP;
  }
  else{
    b = false;
    return Place::HOME;
  }
}
}; // namespace LosG