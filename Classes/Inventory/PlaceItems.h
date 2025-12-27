/****************************************************************
 * Project Name:  StardewValley
 * File Name:     PlaceItems.h
 * File Function: PlaceItems类的实现
 * Author:        于恩熙
 * Update Date:   2025/12/21
 * License:       MIT License
 ****************************************************************/

#ifndef PLACEITEMS_H
#define PLACEITEMS_H

#include "cocos2d.h"
#include "Item.h"

class PlaceItems {

    friend class InventoryGridScene;

public:
    static PlaceItems* getInstance();
    static void destroyInstance();

    // 返回当前物品数组
    std::vector<Item>& getCurrentInventory() { return *_currentInventory; }

private:
    cocos2d::Node* _parent;           // 父节点
    Item _inventory[INVENTORY_TOTAL_SLOTS] ;       // 物品数组
    std::vector<Item>* _currentInventory;         // 目前背包内物品
    float _pos[12] =                              // 打印位置
    { 0.65f,1.6f,2.6f,3.6f,4.6f,5.5f,6.5f,7.5f,8.4f,9.4f,10.4f,11.35f };
    int* _itemCount;                // 物品数量
    const float* _backpackX;       // 背包X坐标数组
    const float* _backpackY;       // 背包Y坐标数组
    float _backpackWidth;          // 背包宽度
    float _cellWidth;              // 格子宽度
    static PlaceItems* _instance;

    // 构造函数
    PlaceItems();

    // 初始化物品数组
    void initItem();

    // 放置工具
    void placeAllItems(int highlightIndex = -1);

    // 清除所有工具精灵
    void clearAllItems();

    // 显示物品数量
    void showItemCount(int itemIndex, int count, float x, float y);

    // 更新单个工具精灵
    void updateItemSprite(int toolIndex, const std::string& imagePath,
        float size, float x, float y);

    // 设置上下文信息
    void setContext(cocos2d::Node* parent, std::vector<Item>& inventory, int& itemCount,
        float* backpackX, float* backpackY, float backpackWidth, float cellWidth);

    // 改变物品数量
    bool addItem(ItemType object, int amount);
    bool removeItem(ItemType object, int amount);

    // 删除物品后调整格子
    void adjustPos();  

    // 返回指定物品的索引
    int findItemIndex(ItemType tag);

};

#endif // PLACEITEMS_H