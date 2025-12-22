#ifndef __FISH_GAME_SCENE_H__
#define __FISH_GAME_SCENE_H__

#include "cocos2d.h"
#include "FishGameLayer.h"

class FishGameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    CREATE_FUNC(FishGameScene);
};

#endif // __FISH_GAME_SCENE_H__