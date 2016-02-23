#pragma once

#include "stdVars.h"
#include "BirdAvatar.h"
#include "PipeBlock.h"
#include "stdString.h"
#include "PipeTrigger.h"
//
//
//
//

class MainScene : public cocos2d::Layer,public ITriggerObserver
{
public:
    static cocos2d::Scene* createScene();
    bool init();
    void setPhysicsWorld(cocos2d::PhysicsWorld* world);
    
    void makeRestart();
    
    bool onContactBegin(cocos2d::PhysicsContact &contact);
    bool onTouch(cocos2d::Touch *touch, cocos2d::Event *event);
    
    void createMenuLayers();
    void showMenu(MenuState menuCase);
    
    cocos2d::Layer* createMainMenuLayer();
    cocos2d::Layer* createMenuScoreLayer();
    cocos2d::Layer* createAboutMenuLayer();
    cocos2d::Layer* createGameDefeatMenuLayer();
    cocos2d::Layer* createOptionMenuLayer();
    
    void menuScore(cocos2d::Ref* pSender);
    void menuScoreOnStart(cocos2d::Ref* pSender);
    void menuBackToMenu(cocos2d::Ref* pSender);
    void menuRestart(cocos2d::Ref* pSender);
    void menuOption(cocos2d::Ref* pSender);
    void menuAbout(cocos2d::Ref* pSender);
    void menuStart(cocos2d::Ref *pSender);
    
    void createHUDLayer();
    void createGameLayer();
    
    void generateBlock(float positionX);
    void gerarateNewBlock(EventCustom* event);
    float randomFloat();
    
    virtual void handleTriggerActivate(PipeTrigger* ref);
    
    virtual void update(float deltaTime);
    
    static MainScene* createWithPhysicsBody(cocos2d::PhysicsWorld* world);
private:
    cocos2d::Vec2 screenCenterPosition;
    cocos2d::Size screenSize;
    
    cocos2d::PhysicsWorld* physicsWorld;
    
    cocos2d::Sprite* sceneGround;
    cocos2d::Sprite* sceneBackground;
    
    BirdAvatar* birdObject;
    
    GameState gameState ;
    
    cocos2d::ui::Text* scoreText;
    cocos2d::ui::Text* scoreMenu;
    
    cocos2d::Layer* HUDLayer;
    
    cocos2d::Layer* currentMenuLayer;
    cocos2d::Layer* mainMenuLayer;
    cocos2d::Layer* menuScoreLayer;
    cocos2d::Layer* menuGameDefeatLayer;
    cocos2d::Layer* menuAboutLayer;
    cocos2d::Layer* menuOptionLayer;
    
    cocos2d::Layer* gameLayer;
    
    cocos2d::Layer* backgroundLayer;
    
    cocos2d::EventListenerPhysicsContact* contactListener;
    
    Vec2 menuScoreCenter;
    Vec2 menuScoreLeftTop;
    
    float groundHeight;
    
    int score;
    int maxScore;
    float maxPositionBlock;
    
    float moveTextureGround;
    float moveTextureBackground;

    
};
