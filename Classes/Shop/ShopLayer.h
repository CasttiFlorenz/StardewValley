#ifndef __SHOPLAYER_H__
#define __SHOPLAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <vector>
#include <string>
#include "../Player/Player.h"
#include "../Inventory/InventoryScene.h"
#include "../Inventory/Item.h"

class ShopLayer : public cocos2d::LayerColor {
public:
    static ShopLayer* create(Item* item);
    virtual bool init(Item* item);
    std::function<void()> onPurchaseSuccess;
    void onBuyClicked(cocos2d::Ref* sender);
    void onCloseClicked(cocos2d::Ref* sender);
private:
    Item* _targetItem;      // 当前要买的商品
    int _maxQuantity;       // 最大可购买数量
    int _currentQuantity;   // 当前选择数量

    // UI 控件
    cocos2d::Label* _lblQuantity;
    cocos2d::Label* _lblTotalCost;
    cocos2d::ui::Slider* _slider;
    cocos2d::ui::Button* _btnBuy;

    // 回调函数
    void onSliderEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type);

    void updateUI();
};
#endif