#include "BirdAvatar.h"


bool BirdAvatar::initBird()
{
    birdSprite = Sprite::create("img_bird.png");

    physicsBodyBird = PhysicsBody::createBox(Size(birdSprite->getContentSize().width - OFFSET_PHYSiCS_BODY_BIRD, birdSprite->getContentSize().height),
                                                  PhysicsMaterial(PHYSICS_MATERIAL, 0, PHYSICS_MATERIAL));
    physicsBodyBird->setDynamic(true);
    physicsBodyBird->setGravityEnable(false);
    physicsBodyBird->setRotationEnable(false);
    physicsBodyBird->setAngularVelocityLimit(0.1);

    this->setPhysicsBody(physicsBodyBird);
    
    birdSprite->setPosition(birdSprite->getContentSize().width / 2.0f, birdSprite->getContentSize().height / 2.0f);
    
    this->addChild(birdSprite);
    makeUpDownAnimation();
    
    birdGameState = BGS_WAIT;
    
    this->scheduleUpdate();

    this->setContentSize(birdSprite->getContentSize());
    
    return true;
}

void BirdAvatar::update(float deltaTime)
{
    if (birdGameState != BGS_WAIT && birdGameState != BGS_DEATH)
    {
        this->setPositionX(this->getPositionX() + (this->getBirdSpeed() * deltaTime));
    }
    if (birdGameState == BGS_DEATH && physicsBodyBird->getVelocity().y > 0)
    {
        birdSprite->stopAllActions();
    }
}

bool BirdAvatar::makeFlyUp()
{
    switch (birdGameState)
    {
        case BGS_WAIT:
            prepareToFly();
            break;
            
        case BGS_TAP:
            makeTap();
            break;
            
        case BGS_FALL:
            toFall();
            break;
            
        default:
            return false;
    }
    return true;
}

float BirdAvatar::getBirdSpeed()
{
    return BIRD_SPEED_DEFAULT;
}

void BirdAvatar::prepareToFly()
{
    physicsBodyBird->setGravityEnable(true);
    physicsBodyBird->setDynamic(true);
    
    physicsBodyBird->setCollisionBitmask(TAG_BIRD);
    physicsBodyBird->setContactTestBitmask(true);
    physicsBodyBird->addMass(BIRD_MASS);

    physicsBodyBird->setLinearDamping(1);
    
    birdGameState = BGS_FALL;
}

void BirdAvatar::makeTap()
{
    birdGameState =  BGS_TAP;
    
    toFall();
}

void BirdAvatar::toFall()
{
    birdGameState = BGS_FALL;
    
    auto rotateToUp = RotateTo::create(BIRD_ROTTATE_UP_TIME, -BIRD_ROTTATE_UP);
    auto rotateToDown = RotateTo::create(BIRD_ROTTATE_DOWN_TIME, BIRD_ROTTATE_DOWN);
    auto delay = DelayTime::create(BIRD_ROTTATE_DALAY);
    auto sequence = Sequence::create(rotateToUp, delay, rotateToDown, NULL);
    
    physicsBodyBird->setVelocity(Vec2::ZERO);
    physicsBodyBird->applyImpulse(Vec2(0, physicsBodyBird->getMass() * -GRAVITY*BIRD_IMPULSE));
    
    birdSprite->stopAllActions();
    birdSprite->runAction(sequence);
}

void BirdAvatar::killBird()
{
    physicsBodyBird->setVelocity(Vec2(0, -BIRD_VELOSITY_TO_DOWN));
    physicsBodyBird->setLinearDamping(0.3f);
    if (birdGameState != BGS_DEATH)
    {
        birdSprite->stopAllActions();
        birdGameState = BGS_DEATH;
    }
    auto rotateToDown = RotateTo::create(BIRD_ROTTATE_DOWN_TIME, BIRD_ROTTATE_DOWN);
    
    birdSprite->runAction(rotateToDown);
}

void BirdAvatar::makeUpDownAnimation()
{
    auto onUp = MoveBy::create(BIRD_ANIMATION_TIME, Vec2(0, BIRD_ANIMATION_OFFSET));
    auto onDown = MoveBy::create(BIRD_ANIMATION_TIME, Vec2(0, -BIRD_ANIMATION_OFFSET));
    auto startAnimation = RepeatForever::create(Sequence::create(onUp, onDown,  NULL));
    
    this->runAction(startAnimation);
}

void BirdAvatar::riseBird()
{
    birdGameState = BGS_WAIT;
    
    birdSprite->stopAllActions();
    
    makeUpDownAnimation();
    
    physicsBodyBird->setDynamic(false);
    
    birdSprite->setRotation(0.0f);
    this->setRotation(0.0f);
    physicsBodyBird->setRotationOffset(0.0f);
}


