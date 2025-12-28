#pragma once
#ifndef __LEEK_ITEM_H__
#define __LEEK_ITEM_H__

#include "EnvironmentItem.h"

// ¾Â´ÐÎïÆ·
class LeekItem : public EnvironmentItem {
public:
    static LeekItem* create(const cocos2d::Vec2& tileCoord);
    bool init(const cocos2d::Vec2& tileCoord);
};

#endif // __LEEK_ITEM_H__