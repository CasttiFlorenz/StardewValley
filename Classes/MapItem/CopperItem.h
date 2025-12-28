#pragma once
#ifndef __COPPER_ITEM_H__
#define __COPPER_ITEM_H__

#include "EnvironmentItem.h"

// Í­¿óÎïÆ·
class CopperItem : public EnvironmentItem {
public:
    static CopperItem* create(const cocos2d::Vec2& tileCoord);
    bool init(const cocos2d::Vec2& tileCoord);
};

#endif // __COPPER_ITEM_H__