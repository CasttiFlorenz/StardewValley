/****************************************************************
 * Project Name:  StardewValley
 * File Name:     PlaceItems.h
 * File Function: PlaceItems类的实现
 * Author:        于恩熙
 * Update Date:   2025/12/19
 * License:       MIT License
 ****************************************************************/

#ifndef PLACEITEMS_H
#define PLACEITEMS_H

#include "cocos2d.h"
#include "Item.h"

class PlaceItems
{

    friend class InventoryGridScene;
public:

    // 常量定义
    static const int TOOL_TAG_BASE = 1000;
    static const int COUNT_TAG_BASE = 2000;

private:
    cocos2d::Node* _parent;           // 父节点
    Item _inventory[36] ;             // 物品数组
    std::vector<Item>* _currentInventory;   // 目前背包内物品
    float _pos[12] =                        // 打印位置
    { 0.65,1.6,2.6,3.6,4.6,5.5,6.5,7.5,8.4,9.4,10.4,11.35 };
    int* _itemCount;                // 物品数量
    const float* _backpackX;       // 背包X坐标数组
    const float* _backpackY;       // 背包Y坐标数组
    float _backpackWidth;          // 背包宽度
    float _cellWidth;              // 格子宽度

    // 构造函数
    PlaceItems();

    // 初始化物品数组
    void initItem();

    // 放置工具
    void placeTools(int highlightIndex = -1);

    // 清除所有工具精灵
    void clearAllTools();

    // 显示物品数量
    void showItemCount(int itemIndex, int count, float x, float y);

    // 更新单个工具精灵
    void updateToolSprite(int toolIndex, const std::string& imagePath,
        float size, float x, float y);

    // 设置上下文信息
    void setContext(cocos2d::Node* parent, std::vector<Item>& inventory, int& itemCount,
        float* backpackX, float* backpackY, float backpackWidth, float cellWidth);

    bool addItem(Objects object, int amount);
    bool removeItem(Objects object, int amount);

    int findItemIndex(Objects tag);

    void adjustPos();           // 删除物品后调整格子
};

#endif // PLACEITEMS_H