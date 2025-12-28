/****************************************************************
 * Project Name:  StardewValley
 * File Name:     WoodItem.cpp
 * File Function: WoodItem类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/16
 * License:       MIT License
 ****************************************************************/
#include "WoodItem.h"

// ´´½¨ÊµÀý
WoodItem* WoodItem::create(const cocos2d::Vec2& tileCoord) {
    auto p = new (std::nothrow) WoodItem();
    if (p && p->init(tileCoord)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

// ³õÊ¼»¯
bool WoodItem::init(const cocos2d::Vec2& tileCoord) {
    if (!EnvironmentItem::init(EnvironmentItemType::WOOD, tileCoord)) {
        return false;
    }

    this->setTexture(WOOD_ITEM_TEXTURE_PATH);
    return true;

}
