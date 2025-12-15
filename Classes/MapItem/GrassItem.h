#pragma once
#ifndef __GRASS_ITEM_H__
#define __GRASS_ITEM_H__

#include "FarmItem.h"

class GrassItem : public FarmItem {
public:
    static GrassItem* create(const cocos2d::Vec2& tileCoord);
    bool init(const cocos2d::Vec2& tileCoord);
};

#endif // __GRASS_ITEM_H__
