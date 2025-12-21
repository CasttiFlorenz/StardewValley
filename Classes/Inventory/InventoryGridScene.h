/****************************************************************
 * Project Name:  StardewValley
 * File Name:     InventoryGridScene.h
 * File Function: InventoryGridScene类的实现
 * Author:        于恩熙
 * Update Date:   2025/12/21
 * License:       MIT License
 ****************************************************************/

#ifndef __INVENTORY_GRID_SCENE_H__  
#define __INVENTORY_GRID_SCENE_H__

#include "cocos2d.h"
#include "Item.h"
#include "Constant.h"
#include "PlaceItems.h"
#include "InventoryManager.h"
#include <array>

USING_NS_CC;

//#define INVENTORY_SIZE 36  // 背包大小


struct BackpackPosition {
    float x;
    float y;
    Sprite* backpack;  // 背包精灵指针
    float scale;       // 缩放比例
};

class InventoryGridScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    CREATE_FUNC(InventoryGridScene);
    int getSelectedGrid();

    void setGridSelectedCallback(const std::function<void(int)>& callback);

    // 获取指定格子的物品
    Item& getItemAt(int gridIndex);

    // 启用或禁用所有格子的触摸监听
    void setGridsTouchEnabled(bool enabled);

    // 添加刷新方法
    void refreshInventory();

    // 返回总物品数量
    int getAmount() { return ItemCount; };

    // 增加数量
    bool addItem(Objects object, int amount);

    // 减少数量
    bool removeItem(Objects object, int amount);

    // 查找物品在当前数组内的索引
    int findItemGridIndex(Objects tag);

    // 常量定义
    static const int TOOL_TAG_BASE = 1000;      // 工具tag基值
    static const int COUNT_TAG_BASE = 2000;     // 数量标签tag基值

private:
    PlaceItems* _placeItems;  // 工具放置管理器
    std::vector<Item> _items;        // 动态存储物品
    BackpackPosition _backpacks[3];  // 三行背包
    cocos2d::Sprite* _background;
    std::array<cocos2d::Sprite*, 4> _buttons;

    int ItemCount = 6;

    // 为指定格子创建触摸监听器
    void createTouchListenerForGrid(int gridIndex, cocos2d::Sprite* grid);

    // 事件监听器
    void setupEventListeners();

    // 核心函数
    void createBackpacks();
    void createInvertory();

    // 高光处理
    void createClickableGrids();
    void onGridClicked(int gridIndex);
    void highlightGrid(int gridIndex, bool highlight);

    // 辅助函数
    float getGridOffset(int gridIndex);
    void onButtonClicked(int buttonIndex);

    // 背包相关
    cocos2d::Sprite* _backpack;
    float _backpackWidth, _backpackHeight;
    float _cellWidth;

    // 格子管理
    cocos2d::DrawNode* _gridHighlights[36];
    int _selectedGrid;

    // 回调函数
    std::function<void(int)> _gridSelectedCallback;

    // 存储格子触摸监听器的映射表：格子索引 -> 监听器
    std::unordered_map<int, cocos2d::EventListenerTouchOneByOne*> _gridListeners;
};

#endif // __INVENTORY_GRID_SCENE_H__