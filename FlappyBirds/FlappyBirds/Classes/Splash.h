#pragma once

#include "MainScene.h"
#include "stdVars.h"

class Splash : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    bool init();
        
    void onShowMainScene(float d);
    
    CREATE_FUNC(Splash);
};