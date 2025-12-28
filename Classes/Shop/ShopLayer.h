/****************************************************************
 * Project Name:  StardewValley
 * File Name:     ShopLayer.h
 * File Function: ShopLayer类的声明
 * Author:        赵睿妍
 * Update Date:   2025/12/17
 * License:       MIT License
 ****************************************************************/
#ifndef __SHOPLAYER_H__
#define __SHOPLAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <vector>
#include <string>
#include "../Player/Player.h"
#include "../Inventory/InventoryScene.h"
#include "../Inventory/Item.h"
#include "../Money/Money.h"
#include <new>              // 用于 std::nothrow

class ShopLayer : public cocos2d::LayerColor {
public:
    // 创建函数
    static ShopLayer* create(Item* item);

    // 初始化函数
    virtual bool init(Item* item);

    // 设置购买成功回调
    void setOnPurchaseSuccess(std::function<void()> callback) { _onPurchaseSuccess = callback; }

    std::function<void()> onPurchaseSuccess;
private:
    // 更新UI显示
    void updateUI();

    // 购买按钮点击回调
    void onBuyClicked(cocos2d::Ref* sender);

    // 关闭按钮点击回调
    void onCloseClicked(cocos2d::Ref* sender);

private:
    // 目标商品 (弱引用，外部管理生命周期)
    Item* _targetItem;

    // 购买数量控制
    int _currentQuantity;
    int _maxQuantity;

    // 购买成功回调
    std::function<void()> _onPurchaseSuccess;

    // UI 控件引用
    cocos2d::Label* _lblQuantity;
    cocos2d::Label* _lblTotalCost;
    cocos2d::ui::Button* _btnBuy;
};

#endif // SHOP_LAYER_H