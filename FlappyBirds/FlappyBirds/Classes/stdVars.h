#pragma once

#include "cocos2d.h"
#include "CocosGUI.h"


using namespace cocos2d;
using namespace cocos2d::ui;


// The game can take such state
enum  GameState
{
    GS_MENU,
    GS_READY,
    GS_PLAY,
    GS_SCORE
};

// The game can take such menu state
enum MenuState
{
    MENU_START,
    MENU_SCORE,
    MENU_ABOUT,
    MENU_OPTION,
    MENU_GAME_DEFEAT,
    MENU_EMPTY
};
// Screen--------------------------------------

#define SCREEN_SIZE_WIDTH 640.0f
#define SCREEN_SIZE_HEIGHT 960.0f

#define SCREEN_SIZE_WIDTH_MAC 378.0f
#define SCREEN_SIZE_HEIGHT_MAC 568.0f

#define SCREEN_FPS 1.0 / 60

#define TIME_SPLASH 2

// Offset -------------------------------------

#define OFFSET_PHYSICS_BODY_GROUND 20
#define OFFSET_PHYSiCS_BODY_BIRD 10
#define OFFSET_MENU_SCORE_LEFT_TOP_WIDTH 70
#define OFFSET_MENU_SCORE_LEFT_TOP_HEIGHT 50
#define OFFSET_MENU_START_HEIGHT 10
#define OFFSET_MENU_SCORE_HEIGHT 70
#define OFFSET_MENU_ABOUT_HEIGHT 150
#define OFFSET_BIRD_ON_START 100
#define OFFSET_SCORE_TEXT_HEIGTH 4 / 5

// String Key---------------------------------

#define KEY_GENERATE_BLOCK "GenerateBlock"
#define KEY_REMOVE_BLOCK "RemoveBlock"
#define KEY_MAX_SCORE "Max"
#define KEY_SPLASH_TIMER "Timer"

// Font --------------------------------------

#define FONT_SIZE_MAIN 40;

#define FONT_ARIAL "Arial"
#define GAME_FONT "Third.ttf"

#define FONT_SIZE_MAIN_MENU 50
#define FONT_SIZE_MENU_ABOUT 40
#define FONT_SIZE_MENU_GAME_DEFEAT 40
#define FONT_SIZE_TITLE 64

// Bird setting ------------------------------

#define BIRD_SPEED_DEFAULT 110.0f
#define BIRD_MASS 10000
#define GRAVITY -98.0f
#define BIRD_IMPULSE 2
#define BIRD_VELOSITY_TO_DOWN 50
#define BIRD_HEIGTH_UNDER_CENTER 100

#define BIRD_ANIMATION_TIME 0.3
#define BIRD_ANIMATION_OFFSET 20

#define BIRD_ROTTATE_UP 30
#define BIRD_ROTTATE_DOWN 90
#define BIRD_ROTTATE_UP_TIME 0.1
#define BIRD_ROTTATE_DOWN_TIME 0.2
#define BIRD_ROTTATE_UP 30
#define BIRD_ROTTATE_DALAY 0.3

// Tag ---------------------------------------

#define TAG_TRIGGER 4
#define TAG_BIRD 1 
#define TAG_GROUND 2
#define TAG_PIPE 3

// Trigger -----------------------------------

#define TRIGGER_POSITION 50
#define TRIGGER_INCREASE_LENGTH 0.05f

// Pipe setting ------------------------------

#define START_BLOCK_POSITION 700
#define BLOCK_DISTANCE 455


#define SPACE_BETWEEN_PIPE 0.15
#define PIPE_ROTATION 180

#define PIPE_BLOCK_MAIN_SCALE 1.5

// Physics-----------------------------------

#define PHYSICS_WORLD_SPEED 4
#define PHYSICS_MATERIAL 400

// Random -----------------------------------

#define RANDOM_MIN_VALUE 20
#define RANDOM_COUNT 55
#define ONE_HUNDRED 100.0f
#define PIPE_OFFSET_HEAD 4

// Menu setting------------------------------

#define MENU_POSITION_RESTART 200
#define MENU_POSITION_SCORE 50

#define MENU_RESTART_POSITION_OFFSET 1/4
#define MENU_SCORE_POSITION_OFFSET 3/4

#define MENU_ABOUT_TEXT_CONTENT_HIGHT 100

// Texture setting--------------------------

#define TEXTURE_PARAMETR_REPEAT_ALL {GL_LINEAR,GL_LINEAR,GL_REPEAT,GL_REPEAT}
#define TEXTURE_PARAMETR_REPEAT_ONE {GL_LINEAR,GL_LINEAR,GL_REPEAT,GL_CLAMP_TO_EDGE}

#define TEXTURE_MOVE_GROUND_SPEED 1
#define TEXTURE_MOVE_BACKGROUND_SPEED 0.6

// Platform random -------------------------

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#define random_func random()
#else
#define random_func arc4random()
#endif
