/****************************************************************
 * Project Name:  StardewValley
 * File Name:     InventoryScene.h
 * File Function: InventoryScene类的实现
 * Author:        于恩熙
 * Update Date:   2025/12/28
 * License:       MIT License
 ****************************************************************/

#ifndef __INVENTORY_SCENE_H__
#define __INVENTORY_SCENE_H__

#include "cocos2d.h"
#include "../Player/Player.h"
#include "InventoryGridScene.h"
#include "Item.h"
#include "PlaceItems.h"


USING_NS_CC;

class InventoryScene : public cocos2d::Scene {
public:
    CREATE_FUNC(InventoryScene);
    static InventoryScene* getInstance();
    static void destroyInstance();

    // 背包物品显示
    virtual bool init();
    void updatePreviewTool();
    void toggleInventory();              // 切换背包显示/隐藏
    void ToolUseAnimation();             // 显示工具特效

    InventoryGridScene* getInventoryLayer() { return _inventoryLayer; }
    bool getInventoryVisible() { return _inventoryVisible; }

    void setPlayer(Player* player);   // 设置玩家

    // 从存档数据加载物品（专为存档设计）
    bool loadItemsFromSaveData(const std::vector<Item>&savedItems);

    // 物品信息
    ItemType getTap() const;
    void addItemCount(ItemType object, int amount, bool animation = true);        // 增加指定物品的数量
    void removeItemCount(ItemType object, int amount);      // 减少指定物品的数量

private:
    static InventoryScene* _instance;

    Player* _player;
    cocos2d::Sprite* _background;         // 背景精灵
    InventoryGridScene* _inventoryLayer;  // 背包界面
    bool _inventoryVisible;               // 是否显示背包
    cocos2d::Sprite* _toolUseEffect = nullptr;    // 工具使用特效（在人物头上显示的图片）
    bool _showToolUseEffect = false;              // 是否允许显示工具使用特效
    int _selectedGrid = -1;                       // 当前选中的工具索引
    cocos2d::Vec2 _secondEffectPos;          // 保存数量增加时第二个特效的位置

    void setToolUse();                            // 设置工具动画属性
    void hideToolUseEffect();                     // 隐藏工具使用特效
    void showToolUseEffect(int selected, bool clearPrevious = true);   // 显示工具使用特效

    cocos2d::Sprite* _previewFrame;      // 预览框图片
    cocos2d::Sprite* _previewTool;       // 预览的工具图片
};

#endif // __INVENTORY_SCENE_H___pragma once