#pragma once
#ifndef __WOOD_ITEM_H__
#define __WOOD_ITEM_H__

#include "FarmItem.h"

class WoodItem : public FarmItem {
public:
    static WoodItem* create(const cocos2d::Vec2& tileCoord);
    bool init(const cocos2d::Vec2& tileCoord);
};

#endif // __WOOD_ITEM_H__
