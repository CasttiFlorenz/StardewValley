#pragma once
#ifndef __WOOD_ITEM_H__
#define __WOOD_ITEM_H__

#include "EnvironmentItem.h"

// Ä¾Í·ÎïÆ·
class WoodItem : public EnvironmentItem {
public:
    static WoodItem* create(const cocos2d::Vec2& tileCoord);
    bool init(const cocos2d::Vec2& tileCoord);
};

#endif // __WOOD_ITEM_H__