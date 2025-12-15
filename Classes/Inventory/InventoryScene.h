/****************************************************************
 * Project Name:  StardewValley
 * File Name:     InventoryScene.h
 * File Function: InventoryScene类的实现
 * Author:        于恩熙
 * Update Date:   2025/12/15
 * License:       MIT License
 ****************************************************************/

#ifndef __INVENTORY_SCENE_H__
#define __INVENTORY_SCENE_H__

#include "cocos2d.h"
#include "../Player/Player.h"
#include "InventoryGridScene.h"

class InventoryScene : public cocos2d::Scene {
public:
    CREATE_FUNC(InventoryScene);
    static InventoryScene* getInstance();
    static void destroyInstance();
    virtual bool init();
    void updatePreviewTool();
    void setPlayer(Player* player) { _player = player; }
    void toggleInventory();              // 切换背包显示/隐藏
    void ToolUseAnimation();            // 返回是否该显示工具特效

private:
    static InventoryScene* _instance;

    Player* _player;
    cocos2d::Sprite* _background;         // 背景精灵
    InventoryGridScene* _inventoryLayer;  // 背包界面
    bool _inventoryVisible;               // 是否显示背包
    cocos2d::Sprite* _toolUseEffect = nullptr;    // 工具使用特效（在人物头上显示的图片）
    bool _showToolUseEffect = false;              // 是否允许显示工具使用特效
    int _selectedGrid = -1;          // 当前选中的工具索引

    void setToolUse();                            // 设置工具动画属性
    void hideToolUseEffect();                     // 隐藏工具使用特效
    void showToolUseEffect();            // 显示工具使用特效


    cocos2d::Sprite* _previewFrame;      // 预览框图片
    cocos2d::Sprite* _previewTool;       // 预览的工具图片
};

#endif // __INVENTORY_SCENE_H___pragma once
