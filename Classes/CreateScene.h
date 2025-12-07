#pragma once
/****************************************************************
 * Project Name:  StardewValley
 * File Name:     CreateScene.h
 * File Function: CreateScene类的定义
 * Author:        郭芷烟
 * Update Date:   2025/12/07
 * License:       MIT License
 ****************************************************************/

#ifndef __CREATE_SCENE_H__
#define __CREATE_SCENE_H__

#include "cocos2d.h"
#include "InitialMenuScene.h"
#include "HoverButton.h"
#include "SimpleAudioEngine.h"
#include <iostream>


class CreateScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(CreateScene);
};

#endif // __CREATE_SCENE_H__
