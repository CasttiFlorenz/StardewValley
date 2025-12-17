#pragma once
#ifndef __FARM_ITEM_H__
#define __FARM_ITEM_H__

#include "cocos2d.h"
#include "Constant.h"

USING_NS_CC;

class EnvironmentItem : public Sprite {
public:
    static EnvironmentItem* create(EnvironmentItemType type, const Vec2& tileCoord);
    bool init(EnvironmentItemType type, const Vec2& tileCoord);

    EnvironmentItemType getType() const { return _type; }
    const Vec2& getTileCoord() const { return _tileCoord; }
    void setTileCoord(const Vec2& coord) { _tileCoord = coord; }

protected:
    EnvironmentItemType _type;
    Vec2 _tileCoord;
};

#endif // __FARM_ITEM_H__
