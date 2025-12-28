/****************************************************************
 * Project Name:  StardewValley
 * File Name:     CreateScene.h
 * File Function: CreateScene类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/11
 * License:       MIT License
 ****************************************************************/
#pragma once
#ifndef __CREATE_SCENE_H__
#define __CREATE_SCENE_H__

#include "cocos2d.h"
#include "InitialMenuScene.h"
#include "../Button/HoverButton.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"

// 创建角色场景
class CreateScene : public cocos2d::Scene
{
public:
    // 创建场景静态方法
    static cocos2d::Scene* createScene();

    // 初始化
    virtual bool init();

    // 宏定义 CREATE_FUNC
    CREATE_FUNC(CreateScene);
};


#endif // __CREATE_SCENE_H__
