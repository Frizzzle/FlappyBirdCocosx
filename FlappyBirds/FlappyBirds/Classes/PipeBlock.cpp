#include "PipeBlock.h"

bool PipeBlock::initPipe(float PipeHeightCoef)
{
    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    pipeHeight = PipeHeightCoef * Director::getInstance()->getVisibleSize().height;
    
    pipeMainBlock = Sprite::create("scene_block_main.png");
    pipeMainBlock->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    
    pipeHeadBlock = Sprite::create("scene_block_head.png");
    pipeHeadBlock->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    
    pipeMainBlock->getTexture()->setTexParameters(TEXTURE_PARAMETR_REPEAT_ALL);
    pipeMainBlock->setTextureRect(Rect(0, 0, pipeMainBlock->getContentSize().width, pipeHeight-pipeHeadBlock->getContentSize().height));
    pipeMainBlock->setScale(PIPE_BLOCK_MAIN_SCALE, 1);
    
    physicsPipe = PhysicsBody::createBox(Size(pipeMainBlock->getContentSize().width * PIPE_BLOCK_MAIN_SCALE,
                                              pipeMainBlock->getContentSize().height + pipeHeadBlock->getContentSize().height),
                                              PhysicsMaterial(PHYSICS_MATERIAL, 0, 0));
    physicsPipe->setDynamic(false);
    physicsPipe->setGravityEnable(false);
    
    physicsPipe->setContactTestBitmask(true);
    physicsPipe->setCollisionBitmask(TAG_PIPE);
    this->setPhysicsBody(physicsPipe);
    
    this->setContentSize(Size(pipeMainBlock->getContentSize().width * PIPE_BLOCK_MAIN_SCALE, pipeMainBlock->getContentSize().height + pipeHeadBlock->getContentSize().height));
    
    pipeMainBlock->setPosition(Vec2::ZERO);
    this->addChild(pipeMainBlock);
    
    pipeHeadBlock->setPosition(Vec2(-PIPE_OFFSET_HEAD, pipeMainBlock->getContentSize().height));
    this->addChild(pipeHeadBlock);
    
    this->scheduleUpdate();
    
    return true;
}

void PipeBlock::update(float deltaTime)
{
    if ( -this->getParent()->getPositionX() >  this->getPositionX() + this->getContentSize().width )
    {
        this->getParent()->removeChild(this);
    }
}