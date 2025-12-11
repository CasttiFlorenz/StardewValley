#pragma once
/****************************************************************
 * Project Name:  StardewValley
 * File Name:     InitialMenuScene.h
 * File Function: InitialMenuScene类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/07
 * License:       MIT License
 ****************************************************************/

#ifndef __INITIALMENU_SCENE_H__
#define __INITIALMENU_SCENE_H__

#include "cocos2d.h"
#include "CreateScene.h"

class InitialMenuScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    // implement the "static create()" method manually
    CREATE_FUNC(InitialMenuScene);
};

#endif // __INITIALMENU_SCENE_H__

