/****************************************************************
 * Project Name:  StardewValley
 * File Name:     MainScene.h
 * File Function: MainScene类的实现
 * Author:        于恩熙
 * Update Date:   2025/12/11
 * License:       MIT License
 ****************************************************************/

#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"

#include "InventoryGridScene.h"

class MainScene : public cocos2d::Scene {
private:
    cocos2d::Sprite* _background;        // 背景精灵
    InventoryGridScene* _inventoryLayer;  // 背包界面
    bool _inventoryVisible;               // 是否显示背包

    cocos2d::Sprite* _previewFrame;      // 预览框图片
    cocos2d::Sprite* _previewTool;       // 预览的工具图片

public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(MainScene);

    void updatePreviewTool();            // 更新预览的工具图片

    void setupKeyboardListener();        // 设置键盘监听
    void toggleInventory();              // 切换背包显示/隐藏
};

#endif // __MAIN_SCENE_H__pragma once
