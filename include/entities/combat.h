
#pragma once

#include "common/constant.h"

#include "core/register.h"

namespace LosG{
    struct InventoryComponent{
        EnVec items;                // 
        d maxSize;                  // 最大的空间
        d curSize;                  // 目前的占比
        static int getSize(EntityId inventory_id,Register& r);
    };

    struct KnowledgeComponent {
        std::vector<ls> konwledges;
    };

}