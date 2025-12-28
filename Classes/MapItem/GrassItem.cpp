/****************************************************************
 * Project Name:  StardewValley
 * File Name:     GrassItem.cpp
 * File Function: GrassItem类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/16
 * License:       MIT License
 ****************************************************************/
#include "GrassItem.h"

// ´´½¨ÊµÀý
GrassItem* GrassItem::create(const cocos2d::Vec2& tileCoord) {
    auto p = new (std::nothrow) GrassItem();
    if (p && p->init(tileCoord)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

// ³õÊ¼»¯
bool GrassItem::init(const cocos2d::Vec2& tileCoord) {
    if (!EnvironmentItem::init(EnvironmentItemType::GRASS, tileCoord)) {
        return false;
    }

    this->setTexture(GRASS_ITEM_TEXTURE_PATH);
    return true;

}
