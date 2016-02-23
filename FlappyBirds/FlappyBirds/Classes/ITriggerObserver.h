#pragma once


#include "stdVars.h"

class PipeTrigger;

class ITriggerObserver
{
public:
    virtual void handleTriggerActivate(PipeTrigger* ) = 0;
    
};