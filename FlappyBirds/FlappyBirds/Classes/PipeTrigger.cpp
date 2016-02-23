#include "PipeTrigger.h"

bool PipeTrigger::initTrigger()
{
    auto trigger = PhysicsBody::createEdgeSegment(Vec2(0,-TRIGGER_INCREASE_LENGTH * Director::getInstance()->getVisibleSize().height),
                                                  Vec2(0, (SPACE_BETWEEN_PIPE + TRIGGER_INCREASE_LENGTH) * Director::getInstance()->getVisibleSize().height));
    
    trigger->setCollisionBitmask(TAG_TRIGGER);
    trigger->setContactTestBitmask(true);
    
    
    this->setTag(TAG_TRIGGER);
    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->setPhysicsBody(trigger);

    return true;
}

void PipeTrigger::onCollide()
{
    observer->handleTriggerActivate(this);
    this->removeFromParent();
}

