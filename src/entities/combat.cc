
#include "entities/combat.h"
#include <stack>

namespace LosG {
int InventoryComponent::getSize(EntityId inventory_id, Register &r) {
  std::stack<EntityId> inventory_ids;
  inventory_ids.push(inventory_id);
  int totalSize = 0;
  while (!inventory_ids.empty()) {
    auto topId = inventory_ids.top();
    inventory_ids.pop();
    auto bagSelf = r.get<InventoryComponent>(inventory_id);
    totalSize += bagSelf->maxSize;
    for (auto i : bagSelf->items) {
      if (r.has<InventoryComponent>(i)) {
        inventory_ids.push(i);
      }
    }
  }
  return totalSize;
}
}; // namespace LosG