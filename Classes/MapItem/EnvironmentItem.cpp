/****************************************************************
 * Project Name:  StardewValley
 * File Name:     EnvironmentItem.cpp
 * File Function: EnvironmentItem类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/16
 * License:       MIT License
 ****************************************************************/
#include "EnvironmentItem.h"

// ´´½¨ÊµÀý
EnvironmentItem* EnvironmentItem::create(EnvironmentItemType type, const Vec2& tileCoord) {
    auto p = new (std::nothrow) EnvironmentItem();
    if (p && p->init(type, tileCoord)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

// ³õÊ¼»¯
bool EnvironmentItem::init(EnvironmentItemType type, const Vec2& tileCoord) {
    if (!Sprite::init()) {
        return false;
    }

    _type = type;
    _tileCoord = tileCoord;

    return true;

}
