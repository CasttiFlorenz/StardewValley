/****************************************************************
 * Project Name:  StardewValley
 * File Name:     ShopMenuLayer.h
 * File Function: ShopMenuLayer类的声明
 * Author:        赵睿妍
 * Update Date:   2025/12/17
 * License:       MIT License
 ****************************************************************/

#ifndef __SHOP_MENU_LAYER_H__
#define __SHOP_MENU_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <vector>
#include "Constant.h" 
#include "ShopLayer.h"
#include <new>      
#include <algorithm> 
#include "../Money/Money.h"
#include "../Inventory/Item.h"
#include "../Time/TimeManager.h"
#include "../MapItem/BarnManager.h" 
#include "../Inventory/InventoryScene.h" 

class Item; // 前向声明

class ShopMenuLayer : public cocos2d::Layer {
public:
    // 创建函数
    static ShopMenuLayer* create(const std::string& shopName,
        const std::vector<Item*>& shopItems,
        const std::vector<ItemType>& acceptedTags);

    // 初始化函数
    virtual bool init(const std::string& shopName,
        const std::vector<Item*>& shopItems,
        const std::vector<ItemType>& acceptedTags);

    // 刷新UI显示
    void refreshUI();

private:
    // 售卖物品逻辑
    void sellItem(Item item);

    // 显示提示信息
    void showTip(const std::string& message, const cocos2d::Color3B& color);

    // 绘制选中高亮框
    void drawHighlight(float x, float y, float size);

    // 创建带保底逻辑的按钮 (内部辅助)
    cocos2d::ui::Button* createFallbackButton(const std::string& imagePath, const std::string& text, const cocos2d::Size& size, cocos2d::Color3B color);

private:
    // UI 组件
    cocos2d::DrawNode* _highlightNode;
    cocos2d::Label* _moneyLabel;
    cocos2d::ui::ListView* _listView;
    cocos2d::Node* _inventoryNode;

    // 数据状态
    int _selectedSlotIndex;
    std::vector<ItemType> _acceptedSellTags;
};

#endif // __SHOP_MENU_LAYER_H__