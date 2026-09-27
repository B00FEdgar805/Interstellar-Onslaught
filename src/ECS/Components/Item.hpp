#ifndef Item_hpp
#define Item_hpp

//#include "Component.hpp"
#include "Component.hpp"

class Item final : public BaseComponent // Class is just used to easily grab all collectable items. Has no acutal use
{
public:
    Item() = default;
};

#endif /* Item_hpp */
