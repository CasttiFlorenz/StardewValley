/****************************************************************
 * Project Name:  StardewValley
 * File Name:     InventoryGridScene.h
 * File Function: InventoryGridScene类的实现
 * Author:        于恩熙
 * Update Date:   2025/12/11
 * License:       MIT License
 ****************************************************************/

#ifndef __INVENTORY_GRID_SCENE_H__  
#define __INVENTORY_GRID_SCENE_H__

#include "cocos2d.h"
#include "Item.h"
#include "Constant.h"

USING_NS_CC;

#define INVENTORY_SIZE 36  // 背包大小


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

    // 常量定义
    static const int INTEM_COUNT = 25;
    static const int TOOL_TAG_BASE = 1000;      // 工具tag基值
    static const int COUNT_TAG_BASE = 2000;     // 数量标签tag基值
     
private:
    Item _inventory[INVENTORY_SIZE];  // 物品数组
    BackpackPosition _backpacks[3];  // 三行背包

    // 工具管理相关
    void placeTools(int highlightIndex = -1);  // 放置工具，-1表示无高亮

    // 为指定格子创建触摸监听器
    void createTouchListenerForGrid(int gridIndex, cocos2d::Sprite* grid);

    // 核心函数
    void createBackpacks();
    void createInvertory();
    void initItem();
    // 高光处理
    void createClickableGrids();
    void onGridClicked(int gridIndex);
    void highlightGrid(int gridIndex, bool highlight);

    // 工具精灵管理
    void clearAllTools();  // 清除所有工具精灵
    void updateToolSprite(int toolIndex, const std::string& imagePath,
        float size, float x, float y);  // 更新单个工具

    // 辅助函数
    float getGridOffset(int gridIndex);
    void showItemCount(int itemIndex, int count, float x, float y);

    // 背包相关
    cocos2d::Sprite* _backpack;
    float _backpackWidth, _backpackHeight;
    float _cellWidth;

    // 格子管理
    cocos2d::DrawNode* _gridHighlights[INVENTORY_SIZE];
    int _selectedGrid;

    // 回调函数
    std::function<void(int)> _gridSelectedCallback;

    // 存储格子触摸监听器的映射表：格子索引 -> 监听器
    std::unordered_map<int, cocos2d::EventListenerTouchOneByOne*> _gridListeners;
};

#endif // __INVENTORY_GRID_SCENE_H__