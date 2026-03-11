
#pragma once

#include "common/constant.h"
#include "core/pool.h"
#include <memory>
#include <typeindex>
#include <vcruntime_typeinfo.h>

namespace LosG {

class Register {

private:
  EntityId LOS_id = 0;
  std::vector<EntityId> LOS_freeList;
  std::unordered_map<std::type_index, std::unique_ptr<Ipool>> LOS_typeToPools;
  template <typename Component> Pool<Component> *getOrCreatePool();

public:
  Register() = default;
  EntityId create();
  void destroy(EntityId id);
  template <typename Component>
  Component *get(EntityId id);          // 获取 Component sheet 表里面的数据
  template <typename Component>
  bool has(EntityId id);                // 判断 对应 Component sheet 表里面 数据在不在
  template <typename Component>
  void emplace(EntityId id, Component&& com); // 找到 Component 表 然后 找到对应的
                                            // id 然后 把  com 存进去
  template <typename Component> std::unordered_map<EntityId, Component> &view();
  template <typename ComponentOne, typename... ComponentTwo> EnVec group();
  template<typename  Component> void remove(EntityId id);
};

} // namespace LosG

namespace LosG {
/*
27_2_26
- 创建或者获取 对应的 sheet表
*/
template <typename Component> Pool<Component> *Register::getOrCreatePool() {
  auto type = std::type_index(typeid(Component));
  if (LOS_typeToPools.find(type) == LOS_typeToPools.end()) {
    LOS_typeToPools[type] = std::make_unique<Pool<Component>>();
  }
  return static_cast<Pool<Component> *>(LOS_typeToPools[type].get());
}

/*
27_2_26
- 获取 对应 sheet的 数据 也就是组件!
*/
template <typename Component> Component *Register::get(EntityId id) {
  auto type = std::type_index(typeid(Component));
  if (LOS_typeToPools.find(type) == LOS_typeToPools.end()) {
    return nullptr;
  }
  auto *pool = static_cast<Pool<Component> *>(LOS_typeToPools[type].get());
  return pool->get(id);
}

/*
27_2_26
- 判断是不是 存在对应的 entity id
*/
template <typename Component> bool Register::has(EntityId id) {
  return get<Component>(id) != nullptr;
}

/*
27_2_26
找到 Component 表 然后 找到对应的 id 然后 把  com 存进去
*/
template <typename Component>
void Register::emplace(EntityId id, Component&& com) {
  Pool<Component> *pool = getOrCreatePool<Component>();
  pool->Add(id, std::forward<Component>(com));
}

/*
26_2_27
- 找到对应一个表里面的所有数据
*/
template <typename Component>
std::unordered_map<EntityId, Component> &Register::view() {
  return getOrCreatePool<Component>()->LOS_idToComponentMap;
}

/*
26_2_27
找到 多个 表里面的 相同id
*/
template <typename ComponentOne, typename... ComponentTwo>
EnVec Register::group() {
  EnVec vec;
  auto &componentOne = view<ComponentOne>();
  for (auto &[id, component] : componentOne) {
    if ((has<ComponentTwo>(id) && ...)) {
      vec.push_back(id);
    }
  }
  return vec;
}

template <typename Component>
void Register::remove(EntityId id)
{
    if(has<Component>(id))
    {
        getOrCreatePool<Component>()->remove(id);
    }
}

}; // namespace LosG