#pragma once

#include "stdVars.h"

//
//
//
//
enum BirdGameState
{
    BGS_WAIT,
    BGS_TAP,
    BGS_FALL,
    BGS_DEATH
};

//
//
//
//
class BirdAvatar : public cocos2d::Node
{
public:    
    bool initBird();
    void update(float deltaTime);

    bool makeFlyUp();
    
    void prepareToFly();
    void killBird();
    void riseBird();
    
    void makeTap();
    void toFall();
    
    void onGround();
    void onRestartAction();
    
    void makeUpDownAnimation();
    
    float getBirdSpeed();
    
private:
    cocos2d::PhysicsBody* physicsBodyBird;
    cocos2d::Sprite* birdSprite;
    
    BirdGameState birdGameState;
};
