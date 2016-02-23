#pragma once 

#include "stdVars.h"

class PipeBlock : public cocos2d::Node
{
public:
    
    bool initPipe(float pipeHeight);
    
    void update(float deltaTime);
    
    void changeHeight(float pipeHeight);
    
private:
    cocos2d::Sprite* pipeMainBlock;
    cocos2d::Sprite* pipeHeadBlock;
    
    cocos2d::PhysicsBody* physicsPipe;
    
    float pipeHeight;
    
};