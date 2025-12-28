/****************************************************************
 * Project Name:  StardewValley
 * File Name:     DaffodilsItem.cpp
 * File Function: DaffodilsItem类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/16
 * License:       MIT License
 ****************************************************************/
#include "DaffodilsItem.h"

// ´´½¨ÊµÀý
DaffodilsItem* DaffodilsItem::create(const cocos2d::Vec2& tileCoord) {
    auto p = new (std::nothrow) DaffodilsItem();
    if (p && p->init(tileCoord)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

// ³õÊ¼»¯
bool DaffodilsItem::init(const cocos2d::Vec2& tileCoord) {
    if (!EnvironmentItem::init(EnvironmentItemType::DAFFODILS, tileCoord)) {
        return false;
    }

    this->setTexture(DAFFODILS_ITEM_TEXTURE_PATH);
    return true;

}
