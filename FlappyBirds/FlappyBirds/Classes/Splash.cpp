#include "Splash.h"
#include "stdString.h"
#include "stdVars.h"

Scene* Splash::createScene()
{
    auto scene = Scene::create();
    
    auto layer = Splash::create();
    scene->addChild(layer);
    
    return scene;
}

bool Splash::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size screenSize = Director::getInstance()->getVisibleSize();
    
    auto background = Sprite::create("BackgroundSplash.png");
    background->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    background->setScale(screenSize.width, screenSize.height);
    this->addChild(background);
    
    auto whoCreated = Label::createWithSystemFont(TEXT_WHO_CREATED_DEMO_N, FONT_ARIAL, 40);
    whoCreated->setPosition(screenSize.width / 2.0f, screenSize.height / 2.0f);
    whoCreated->setAlignment(TextHAlignment::CENTER);
    this->addChild(whoCreated);
    
    auto NonCommercial = Label::createWithSystemFont(TEXT_NOT_COMMERCIAL, FONT_ARIAL, 25);
    NonCommercial->setPosition(screenSize.width / 2.0f, screenSize.height / 8.0f);
    this->addChild(NonCommercial);

    this->schedule(CC_CALLBACK_1(Splash::onShowMainScene, this), 0.0f, 0.0f, 4.0f, KEY_SPLASH_TIMER);

    this->scheduleUpdate();
    return true;
} 

void Splash::onShowMainScene(float d)
{
    auto mainScene = MainScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(TIME_SPLASH , mainScene));

}
