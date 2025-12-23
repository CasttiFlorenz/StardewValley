/****************************************************************
 * Project Name:  StardewValley
 * File Name:     ShopMenuLayer.h
 * File Function: ShopMenuLayer类的实现
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
#include "../Money/Money.h"
#include "../Inventory/Item.h"
#include"../Time/TimeManager.h"
#include "../MapItem/BarnManager.h" 
class Item; // 前向声明

class ShopMenuLayer : public cocos2d::Layer {
public:
    static ShopMenuLayer* create(const std::string& shopName,
        const std::vector<Item*>& shopItems,
        const std::vector<ItemType>& acceptedTags);

    virtual bool init(const std::string& shopName,
        const std::vector<Item*>& shopItems,
        const std::vector<ItemType>& acceptedTags);

    void refreshUI();
    void sellItem(Item item);
    void showTip(const std::string& message, const cocos2d::Color3B& color);
    void buyLivestock(Item* item);

private:
    cocos2d::DrawNode* _highlightNode; // 用于画红框
    int _selectedSlotIndex;            // 当前选中的格子索引 (-1表示没选中)

    void drawHighlight(float x, float y, float size); // 画框的具体逻辑
    std::vector<ItemType> _acceptedSellTags;
    cocos2d::Label* _moneyLabel;
    cocos2d::ui::ListView* _listView;
    cocos2d::Node* _inventoryNode;
};

#endif