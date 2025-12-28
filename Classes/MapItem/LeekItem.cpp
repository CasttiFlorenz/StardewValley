/****************************************************************
 * Project Name:  StardewValley
 * File Name:     LeekItem.cpp
 * File Function: LeekItem类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/16
 * License:       MIT License
 ****************************************************************/
#include "LeekItem.h"

// ´´½¨ÊµÀý
LeekItem* LeekItem::create(const cocos2d::Vec2& tileCoord) {
    auto p = new (std::nothrow) LeekItem();
    if (p && p->init(tileCoord)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

// ³õÊ¼»¯
bool LeekItem::init(const cocos2d::Vec2& tileCoord) {
    if (!EnvironmentItem::init(EnvironmentItemType::LEEK, tileCoord)) {
        return false;
    }

    this->setTexture(LEEK_ITEM_TEXTURE_PATH);
    return true;

}
