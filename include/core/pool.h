

#pragma once

#include "common/constant.h"
#include <cerrno>
#include <iterator>
#include <unordered_map>
namespace LosG {
struct Ipool {
  virtual ~Ipool() = default;
  virtual void remove(EntityId id) = 0;
};

/*
26_3_5
- Component is data
- one id to a data
*/
template <typename Component> struct Pool : public Ipool {
  std::unordered_map<EntityId, Component> LOS_idToComponentMap;
  void Add(EntityId id_, Component &&component) {
    LOS_idToComponentMap[id_] = std::move(component);
  }

  Component *get(EntityId id_) {
    if (LOS_idToComponentMap.find(id_) != LOS_idToComponentMap.end()) {
      return &LOS_idToComponentMap[id_];
    }
    return nullptr;
  }

  void remove(EntityId id_) override {
    if (LOS_idToComponentMap.contains(id_))
      LOS_idToComponentMap.erase(id_);
  }
};

} // namespace LosG