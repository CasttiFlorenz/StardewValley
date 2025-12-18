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
#include "../Inventory/PlayerState.h"
#include "../Inventory/Item.h"
class Item; // 前向声明

class ShopMenuLayer : public cocos2d::Layer
{
public:
    // 1. 修改参数：增加 acceptedTags (允许售卖的物品类型)
    static ShopMenuLayer* create(const std::vector<Item*>& shopItems, const std::vector<Objects>& acceptedTags);

    virtual bool init(const std::vector<Item*>& shopItems, const std::vector<Objects>& acceptedTags);

private:
    void refreshUI();
    void sellItem(Item* item);
    // 辅助显示提示信息
    void showTip(const std::string& message, const cocos2d::Color3B& color);

private:
    cocos2d::ui::ListView* _listView;
    cocos2d::Node* _inventoryNode;
    cocos2d::Label* _moneyLabel;

    // 2. 存储当前商店允许回收的物品列表
    std::vector<Objects> _acceptedSellTags;
};

#endif // __SHOP_MENU_LAYER_H__
