/****************************************************************
 * Project Name:  StardewValley
 * File Name:     InitialMenuScene.h
 * File Function: InitialMenuScene类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/16
 * License:       MIT License
 ****************************************************************/
#ifndef __INITIALMENU_SCENE_H__
#define __INITIALMENU_SCENE_H__

#include "cocos2d.h"
#include "CreateScene.h"
#include "../Music/MusicManager.h"
#include "../Save/SaveManage.h"

 // 游戏启动后的第一个界面
class InitialMenuScene : public cocos2d::Scene
{
public:
    // 创建初始菜单场景
    static cocos2d::Scene* createScene();

    // 场景初始化
    virtual bool init() override;

    CREATE_FUNC(InitialMenuScene);
};

#endif // __INITIALMENU_SCENE_H__

