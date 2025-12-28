#pragma once
#ifndef __STONE_ITEM_H__
#define __STONE_ITEM_H__

#include "EnvironmentItem.h"

// 石头物品
class StoneItem : public EnvironmentItem {
public:
    static StoneItem* create(const cocos2d::Vec2& tileCoord);
    bool init(const cocos2d::Vec2& tileCoord);
};

#endif // __STONE_ITEM_H__