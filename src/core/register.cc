#include "core/register.h"


namespace LosG {

/*
26_2_27
- 定义 注册类的 create 函数
- 返回 对应的 entity 的id
*/
EntityId Register::create() {
  if (!LOS_freeList.empty()) {
    EntityId id = LOS_freeList.back();
    LOS_freeList.pop_back();
    return id;
  }
  return ++LOS_id;
}

/*
26_2_27
- 定义 注册类的销毁函数
- type -> id -> T(component)
*/
void Register::destroy(EntityId id) {
  LOS_freeList.push_back(id);     // 销毁 一个 放入u1一个
  for (auto &[type, pool] : LOS_typeToPools) {
    pool->remove(id);
  }
}


}; // namespace LosG