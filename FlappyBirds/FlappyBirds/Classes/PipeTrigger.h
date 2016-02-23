#pragma once

#include "ITriggerObserver.h"
#include "stdVars.h"
#include "ICollisionObject .h"


class PipeTrigger : public cocos2d::Node , public ICollisionObject
{
public:
    bool initTrigger();
    
    ITriggerObserver* observer;
    
    void onCollide();
    
private:
    
};

