#include "MainScene.h"


#pragma mark INIT
Scene* MainScene::createScene()
{
    auto scene = Scene::createWithPhysics();
    
    scene->getPhysicsWorld()->setGravity(Vec2(0, GRAVITY));
    scene->getPhysicsWorld()->setSpeed(PHYSICS_WORLD_SPEED);
    
    MainScene* layer = MainScene::createWithPhysicsBody(scene->getPhysicsWorld());
    
    scene->addChild(layer);
    
    return scene;
}

MainScene* MainScene::createWithPhysicsBody(cocos2d::PhysicsWorld *world)
{
    MainScene *pRet = new MainScene();
    
    if (pRet)
    {
        pRet->setPhysicsWorld(world);
        if (pRet->init())
        {
            pRet->autorelease();
            return pRet;
        }
        else
        {
            delete pRet;
            pRet = NULL;
        }
    }
    return NULL;
}

void MainScene::setPhysicsWorld(cocos2d::PhysicsWorld* world)
{
    physicsWorld = world;
}

bool MainScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    moveTextureBackground = 0;
    moveTextureGround = 0;
        
    gameState = GS_MENU;
    
    score = 0;
    maxScore = UserDefault::getInstance()->getIntegerForKey(KEY_MAX_SCORE, 0);
    maxPositionBlock = 0;    
    
    backgroundLayer = Layer::create();
    screenSize = Director::getInstance()->getVisibleSize();
    screenCenterPosition = Vec2(screenSize.width / 2.0f,screenSize.height / 2.0f);
    
    
    auto sceneBackColor = Sprite::create("scene_back_color.png");
    sceneBackColor->setAnchorPoint(Vec2::ZERO);
    sceneBackColor->setPosition(Vec2::ZERO);
    sceneBackColor->setScale(screenSize.width, screenSize.height);
    backgroundLayer->addChild(sceneBackColor);
    
    auto sceneBottom = Sprite::create("scene_bottom.png");
    sceneBottom->setPosition(Vec2::ZERO);
    sceneBottom->setTextureRect(Rect(0, 0, screenSize.width, sceneBottom->getContentSize().height));
    sceneBottom->setAnchorPoint(Vec2::ZERO);
    backgroundLayer->addChild(sceneBottom);
    
    sceneGround = Sprite::create("scene_ground.png");
    sceneGround->setAnchorPoint(Vec2(Vec2::ZERO));
    sceneGround->setPosition(0, sceneBottom->getContentSize().height);
    sceneGround->getTexture()->setTexParameters(TEXTURE_PARAMETR_REPEAT_ALL);
    sceneGround->setTextureRect(Rect(0, 0, screenSize.width, sceneGround->getContentSize().height));
    backgroundLayer->addChild(sceneGround);
    
    sceneBackground = Sprite::create("scene_background.png");
    sceneBackground->setAnchorPoint(Vec2::ZERO);
    sceneBackground->setPosition(0, sceneGround->getContentSize().height + sceneGround->getPositionY());
    sceneBackground->getTexture()->setTexParameters(TEXTURE_PARAMETR_REPEAT_ONE);
    sceneBackground->setTextureRect(Rect(0, 0, screenSize.width, sceneBackground->getContentSize().height));
    backgroundLayer->addChild(sceneBackground);
    
    groundHeight = sceneBottom->getContentSize().height + sceneGround->getContentSize().height;
    
    auto physicsBodyGround = PhysicsBody::createBox(Size(screenSize.width, groundHeight + OFFSET_PHYSICS_BODY_GROUND), PhysicsMaterial(PHYSICS_MATERIAL, 0, PHYSICS_MATERIAL));
    physicsBodyGround->setDynamic(false);
    physicsBodyGround->setCollisionBitmask(TAG_GROUND);
    physicsBodyGround->setGravityEnable(false);
    physicsBodyGround->setContactTestBitmask(true);
    
    sceneBottom->setPhysicsBody(physicsBodyGround);
    sceneBottom->setPosition(Vec2(sceneBottom->getContentSize().width / 2.0f, sceneBottom->getContentSize().height / 2.0f));
    
    this->addChild(backgroundLayer);
    
    contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(MainScene::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    createGameLayer();
    
    createHUDLayer();
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(MainScene::onTouch, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    this->scheduleUpdate();
    
    return true;
}

#pragma mark  - HUD

void MainScene::createHUDLayer()
{
    HUDLayer = Layer::create();
    
    scoreText = Text::create();
    
    scoreText->setString(TEXT_TITLE_GAME);
    scoreText->setFontSize(FONT_SIZE_TITLE);
    scoreText->setColor(Color3B::ORANGE);
    scoreText->setFontName(GAME_FONT);
    scoreText->setTextHorizontalAlignment(TextHAlignment::LEFT);
    
    menuScoreLeftTop = Vec2(OFFSET_MENU_SCORE_LEFT_TOP_WIDTH, screenSize.height - OFFSET_MENU_SCORE_LEFT_TOP_HEIGHT);
    menuScoreCenter = Vec2(screenCenterPosition.x, screenSize.height * 4 / 5.0f);
    
    scoreText->setPosition(menuScoreCenter);
    
    HUDLayer->addChild(scoreText);
    this->addChild(HUDLayer);
    
    createMenuLayers();
    showMenu(MENU_START);
}

#pragma mark - MENU

void MainScene::createMenuLayers()
{
    mainMenuLayer = createMainMenuLayer();
    menuAboutLayer = createAboutMenuLayer();
    menuGameDefeatLayer = createGameDefeatMenuLayer();
    menuOptionLayer = createOptionMenuLayer();
    menuScoreLayer = createMenuScoreLayer();
    
    currentMenuLayer = NULL;
    
    HUDLayer->addChild(mainMenuLayer);
    HUDLayer->addChild(menuAboutLayer);
    HUDLayer->addChild(menuGameDefeatLayer);
    HUDLayer->addChild(menuOptionLayer);
    HUDLayer->addChild(menuScoreLayer);
}

void MainScene::showMenu(MenuState menuState)
{
    if (currentMenuLayer != NULL)
    {
        currentMenuLayer->setVisible(false);
    }
    
    switch (menuState)
    {
        case MENU_START :
        {
            currentMenuLayer = mainMenuLayer;
            break;
        }
        case MENU_SCORE :
        {
            scoreMenu->setString(String::createWithFormat("%s%i", TEXT_SCORE_BEST_SCORE, maxScore)->getCString());
            currentMenuLayer = menuScoreLayer;
            break;
        }
        case MENU_ABOUT :
        {
            currentMenuLayer =  menuAboutLayer;
            break;
        }
        case MENU_OPTION :
        {
            currentMenuLayer = menuOptionLayer;
            break;
        }
        case MENU_GAME_DEFEAT :
        {
            currentMenuLayer = menuGameDefeatLayer;
            break;
        }
        default:
        {
            currentMenuLayer = NULL;
            return;
        }
    }
    
    currentMenuLayer->setVisible(true);
}

cocos2d::Layer* MainScene::createMainMenuLayer()
{
    auto mainMenuLayer = Layer::create();
    mainMenuLayer->setVisible(false);
    
    auto menuStart = MenuItemLabel::create(Label::createWithTTF(TEXT_GAME_MENU_PLAY, GAME_FONT, FONT_SIZE_MAIN_MENU), CC_CALLBACK_1(MainScene::menuStart, this));
    menuStart->setPosition(Vec2( screenCenterPosition.x , screenCenterPosition.y + OFFSET_MENU_START_HEIGHT));
    
    auto menuScore = MenuItemLabel::create(Label::createWithTTF(TEXT_GAME_MENU_SCORE, GAME_FONT, FONT_SIZE_MAIN_MENU), CC_CALLBACK_1(MainScene::menuScoreOnStart, this));
    menuScore->setPosition(Vec2(screenCenterPosition.x , screenCenterPosition.y - OFFSET_MENU_SCORE_HEIGHT));
    
    auto menuAbout = MenuItemLabel::create(Label::createWithTTF(TEXT_GAME_MENU_ABOUT, GAME_FONT, FONT_SIZE_MAIN_MENU), CC_CALLBACK_1(MainScene::menuAbout, this));
    menuAbout->setPosition(Vec2(screenCenterPosition.x , screenCenterPosition.y - OFFSET_MENU_ABOUT_HEIGHT));
    
    auto menu = Menu::create(menuStart, menuScore, menuAbout,  NULL);
    menu->setPosition(Vec2::ZERO);
    
    mainMenuLayer->addChild(menu);
    return mainMenuLayer;
}

cocos2d::Layer* MainScene::createMenuScoreLayer()
{
    auto menuScoreLayer = Layer::create();
    menuScoreLayer->setVisible(false);
    
    scoreMenu = Text::create();
    
    scoreMenu->ignoreContentAdaptWithSize(false);
    scoreMenu->setTextHorizontalAlignment(TextHAlignment::CENTER);
    
    scoreMenu->setContentSize(Size(screenSize.width, MENU_ABOUT_TEXT_CONTENT_HIGHT));
    scoreMenu->setPosition(Vec2(screenCenterPosition.x , screenCenterPosition.y ));
    
    scoreMenu->setFontSize(FONT_SIZE_MENU_ABOUT);
    scoreMenu->setFontName(GAME_FONT);
    
    scoreMenu->setString(String::createWithFormat("%s%i", TEXT_SCORE_BEST_SCORE, maxScore)->getCString());
    menuScoreLayer->addChild(scoreMenu);

    auto menuBack = MenuItemLabel::create(Label::createWithTTF(TEXT_GAME_MENU_BACK, GAME_FONT, FONT_SIZE_MENU_ABOUT), CC_CALLBACK_1(MainScene::menuBackToMenu, this));
    menuBack->setPosition(Vec2(screenCenterPosition.x , screenSize.height / 3.0f));
    
    auto menu = Menu::create(menuBack,  NULL);
    menu->setPosition(Vec2::ZERO);
    
    menuScoreLayer->addChild(menu);
    return menuScoreLayer;
}

cocos2d::Layer* MainScene::createAboutMenuLayer()
{
    auto menuAboutLayer = Layer::create();
    menuAboutLayer->setVisible(false);
    
    auto menuBack = MenuItemLabel::create(Label::createWithTTF(TEXT_GAME_MENU_BACK, GAME_FONT, FONT_SIZE_MENU_ABOUT), CC_CALLBACK_1(MainScene::menuBackToMenu, this));
    menuBack->setPosition(Vec2(screenCenterPosition.x , screenSize.height / 3.0f));
    
    auto about = Text::create();
    
    about->ignoreContentAdaptWithSize(false);
    about->setTextHorizontalAlignment(TextHAlignment::CENTER);
    
    about->setContentSize(Size(screenSize.width, MENU_ABOUT_TEXT_CONTENT_HIGHT));
    about->setPosition(Vec2(screenCenterPosition.x , screenCenterPosition.y));
    
    about->setFontSize(FONT_SIZE_MENU_ABOUT);
    about->setFontName(GAME_FONT);
    
    about->setString(TEXT_WHO_CREATED_DEMO);
    menuAboutLayer->addChild(about);
    
    auto menu = Menu::create(menuBack, NULL);
    menu->setPosition(Vec2::ZERO);
    
    menuAboutLayer->addChild(menu);
    return menuAboutLayer;
}

cocos2d::Layer* MainScene::createGameDefeatMenuLayer()
{
    auto menuGameDefeatLayer = Layer::create();
    menuGameDefeatLayer->setVisible(false);
    
    auto menuRestart = MenuItemLabel::create(Label::createWithTTF(TEXT_GAME_MENU_RESTART, GAME_FONT, FONT_SIZE_MENU_GAME_DEFEAT), CC_CALLBACK_1(MainScene::menuRestart, this));
    menuRestart->setPosition(Vec2( screenSize.width * MENU_RESTART_POSITION_OFFSET , screenCenterPosition.y));
    
    auto menuBacks = MenuItemLabel::create(Label::createWithTTF(TEXT_GAME_MENU_BACK, GAME_FONT, FONT_SIZE_MENU_GAME_DEFEAT), CC_CALLBACK_1(MainScene::menuBackToMenu, this));
    menuBacks->setPosition(Vec2(screenCenterPosition.x , screenSize.height / 3.0f));
    
    auto menuScore = MenuItemLabel::create(Label::createWithTTF(TEXT_GAME_MENU_SCORE, GAME_FONT, FONT_SIZE_MENU_GAME_DEFEAT), CC_CALLBACK_1(MainScene::menuScore, this));
    menuScore->setPosition(Vec2(screenSize.width * MENU_SCORE_POSITION_OFFSET , screenCenterPosition.y));
    
    auto menu = Menu::create(menuRestart, menuBacks, menuScore,  NULL);
    menu->setPosition(Vec2::ZERO);
    
    menuGameDefeatLayer->addChild(menu);
    return menuGameDefeatLayer;
}

cocos2d::Layer* MainScene::createOptionMenuLayer()
{
    //TODO create option menu
    return Layer::create();
}

void MainScene::menuStart(cocos2d::Ref *pSender)
{
    scoreText->setString(TEXT_ZERO);
    scoreText->setFontSize(FONT_SIZE_TITLE);
    
    scoreText->setPosition(menuScoreLeftTop);
    score = 0 ;
    gameState = GS_PLAY;
    
    birdObject->riseBird();
    birdObject->stopAllActions();
    birdObject->makeFlyUp();
    
    showMenu(MENU_EMPTY);
    
    birdObject->makeFlyUp();
}

void MainScene::menuOption(cocos2d::Ref *pSender)
{
    //TODO option for game
}

void MainScene::menuAbout(cocos2d::Ref *pSender)
{
    showMenu(MENU_ABOUT);
}

void MainScene::menuScoreOnStart(cocos2d::Ref *pSender)
{
    showMenu(MENU_SCORE);
}

void MainScene::menuRestart(cocos2d::Ref *pSender)
{
    gameState = GS_READY;
    
    scoreText->setString(TEXT_ZERO);
    scoreText->setPosition(menuScoreLeftTop);
    score = 0 ;
    
    gameLayer->setPositionX(0);
    makeRestart();
    showMenu(MENU_EMPTY);
    
    birdObject->setPosition(screenCenterPosition.x, screenCenterPosition.y + OFFSET_BIRD_ON_START);
    birdObject->riseBird();
}

void MainScene::menuBackToMenu(cocos2d::Ref *pSender)
{
    if (gameState == GS_SCORE)
    {
        gameState = GS_MENU;
        gameLayer->setPositionX(0);
        
        makeRestart();
        
        scoreText->setString(TEXT_TITLE_GAME);
        scoreText->setFontSize(FONT_SIZE_TITLE);
        scoreText->setPosition(menuScoreCenter);
        
        birdObject->setPosition(screenCenterPosition.x, screenCenterPosition.y + BIRD_HEIGTH_UNDER_CENTER);
        birdObject->riseBird();
    }
    showMenu(MENU_START);
}

void MainScene::menuScore(cocos2d::Ref *pSender)
{
    scoreText->setString(String::createWithFormat("%s%i\n%s%i", TEXT_SCORE_YOUR_SCORE, score, TEXT_SCORE_BEST_SCORE, maxScore)->getCString());
    scoreText->setPosition(Vec2(screenCenterPosition.x, screenSize.height * OFFSET_SCORE_TEXT_HEIGTH));
}

#pragma mark - GAME

void MainScene::createGameLayer()
{
    gameLayer = Layer::create();
    
    birdObject = new BirdAvatar();
    birdObject->initBird();

    birdObject->setPosition(screenCenterPosition.x, screenCenterPosition.y + BIRD_HEIGTH_UNDER_CENTER);
    gameLayer->addChild(birdObject);
    
    makeRestart();
    
    this->addChild(gameLayer);
}

void MainScene::makeRestart()
{    
    while (gameLayer->getChildByTag(TAG_PIPE) != NULL)
    {
        gameLayer->removeChildByTag(TAG_PIPE);
        gameLayer->removeChildByTag(TAG_PIPE);
    }
    
    while (gameLayer->getChildByTag(TAG_TRIGGER) != NULL)
    {
        gameLayer->removeChildByTag(TAG_TRIGGER);
    }
    
    physicsWorld->setGravity(Vec2(0, GRAVITY));
    
    generateBlock(START_BLOCK_POSITION);
    
    maxPositionBlock = START_BLOCK_POSITION;
}

bool MainScene::onContactBegin(cocos2d::PhysicsContact &contact)
{
    PhysicsBody* a = contact.getShapeA()->getBody();
    PhysicsBody* b = contact.getShapeB()->getBody();
    

    if (gameState != GS_SCORE)
    {
        ICollisionObject* objectOne = dynamic_cast<ICollisionObject* >(a->getNode());
        ICollisionObject* objectTwo = dynamic_cast<ICollisionObject* >(b->getNode());
        
        if (objectOne )
        {
            objectOne->onCollide();
        }
        
        if (objectTwo )
        {
            objectTwo->onCollide();
        }
        
        if ((a->getCollisionBitmask() != TAG_TRIGGER && (b->getCollisionBitmask() == TAG_BIRD))||(b->getCollisionBitmask() != TAG_TRIGGER && (a->getCollisionBitmask() == TAG_BIRD)))
        {
            gameState = GS_SCORE;
            if (score > maxScore)
            {
                maxScore = score;
                UserDefault::getInstance()->setIntegerForKey(KEY_MAX_SCORE, maxScore);
            }
            physicsWorld->setGravity(Vec2::ZERO);
            birdObject->killBird();
            showMenu(MENU_GAME_DEFEAT);
        }
    }
    return true;
}

bool MainScene::onTouch(cocos2d::Touch *touch, cocos2d::Event *event)
{
    switch (gameState)
    {
        case GS_MENU:
        {
            return false;
        }
        case GS_READY:
        {
            gameState = GS_PLAY;
            birdObject->makeFlyUp();
        }
        default:
        {
            break;
        }
    }
    
    if (birdObject->getPositionY() + birdObject->getContentSize().height >= screenSize.height )
    {
        return false;
    }
    birdObject->stopAllActions();
    
    return birdObject->makeFlyUp();
}

#pragma mark - PIPE

void MainScene::handleTriggerActivate(PipeTrigger* ref )
{
    score++;
    scoreText->setString(String::createWithFormat("%i", score)->getCString());
    
    maxPositionBlock += BLOCK_DISTANCE ;
    generateBlock(maxPositionBlock);
}

void MainScene::generateBlock(float positionX)
{
    PipeBlock* bottomPipe = new PipeBlock();
    
    float randomValue = randomFloat();
   
    bottomPipe->initPipe(randomValue - 0.1f);
    bottomPipe->setPosition(positionX , groundHeight + bottomPipe->getContentSize().height / 2.0f );
    
    bottomPipe->setTag(TAG_PIPE);
    gameLayer->addChild(bottomPipe);
    
    PipeBlock* ceilingPipe = new PipeBlock();
    
    ceilingPipe->initPipe(1 - randomValue - SPACE_BETWEEN_PIPE);

    ceilingPipe->setRotation(PIPE_ROTATION);
    ceilingPipe->setPosition(positionX, screenSize.height - ceilingPipe->getContentSize().height / 2.0f);
    
    ceilingPipe->setTag(TAG_PIPE);
    gameLayer->addChild(ceilingPipe);
    
    auto pipeTrigger = new PipeTrigger();
    pipeTrigger->initTrigger();
    pipeTrigger->setPosition((bottomPipe->getPositionX() + bottomPipe->getContentSize().width / 2.0f + birdObject->getContentSize().width),
                             bottomPipe->getContentSize().height / 2.0f + bottomPipe->getPositionY());
    pipeTrigger->observer = this;
    gameLayer->addChild(pipeTrigger);
}

float MainScene::randomFloat()
{
    return ((RANDOM_MIN_VALUE + (random_func % RANDOM_COUNT)) / ONE_HUNDRED);
}

#pragma mark UPDATE

void MainScene::update(float deltaTime)
{
    float birdSpeed =  birdObject->getBirdSpeed() * deltaTime;
    
    if (gameState == GS_PLAY)
    {
        gameLayer->setPositionX( gameLayer->getPositionX() - ( birdSpeed ) );
    }
    
    if (gameState != GS_SCORE)
    {
        moveTextureGround += birdSpeed * TEXTURE_MOVE_GROUND_SPEED;
        moveTextureBackground += birdSpeed * TEXTURE_MOVE_BACKGROUND_SPEED;
        
        if ( moveTextureBackground  > sceneBackground->getTexture()->getContentSize().width)
        {
            moveTextureBackground -= sceneBackground->getTexture()->getContentSize().width ;
        }
        
        if ( moveTextureGround  > sceneGround->getTexture()->getContentSize().width)
        {
            moveTextureGround -= sceneGround->getTexture()->getContentSize().width ;
        }
        
        sceneGround->setTextureRect(Rect(moveTextureGround, 0, screenSize.width, sceneGround->getContentSize().height));
        sceneBackground->setTextureRect(Rect(moveTextureBackground, 0, screenSize.width, sceneBackground->getContentSize().height));
    }
}



