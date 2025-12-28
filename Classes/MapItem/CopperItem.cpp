/****************************************************************
 * Project Name:  StardewValley
 * File Name:     CopperItem.cpp
 * File Function: CopperItem类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/16
 * License:       MIT License
 ****************************************************************/
#include "CopperItem.h"

// ´´½¨ÊµÀý
CopperItem* CopperItem::create(const cocos2d::Vec2& tileCoord) {
    auto p = new (std::nothrow) CopperItem();
    if (p && p->init(tileCoord)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

// ³õÊ¼»¯
bool CopperItem::init(const cocos2d::Vec2& tileCoord) {
    if (!EnvironmentItem::init(EnvironmentItemType::COPPER, tileCoord)) {
        return false;
    }
    this->setTexture(COPPER_ITEM_TEXTURE_PATH);
    return true;

}
