//
//  Components.hpp
//  GameTestSDL3
//
//  Created by Edgar Alamillo on 5/15/26.
//

#ifndef Components_hpp
#define Components_hpp

#include "ECS.hpp"

class PositionComponenet : public Component
{
private:
    int X_POS;
    int Y_POS;
public:
    int x()
    {
        return X_POS;
    }
    int y()
    {
        return Y_POS;
    }
    void setPosition(int x, int y)
    {
        X_POS = x;
        Y_POS = y;
    }
    void init() override
    {
        X_POS = 0;
        Y_POS = 0;
    }
    
    void update() override
    {
        X_POS++;
        Y_POS++;
    }
};

#endif /* Components_hpp */
