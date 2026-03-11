#include"common/constant.h"
#include "register.h"
#include <string_view>
#include <vector>

namespace LosG{

    struct WorldNode{
        ls name;
        c icon;
        i x;
        i y;
    };

    class World{
        public:
        static void init();
        static d go(Place from, Place to, double v);  
        static void goTo(Register& r, EntityId id,Place p);    
        static void render(Place where);
        static Place strToPlace(std::string_view s,bool& b);

    private:
        static std::unordered_map<Place, WorldNode> LOS_worldNodes;
    };

};