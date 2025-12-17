#pragma once
#ifndef __MINES_ITEM_H__
#define __MINES_ITEM_H__

#include "cocos2d.h"
#include "Constant.h"

USING_NS_CC;

class MinesItem : public Sprite {
public:
    static MinesItem* create(MinesItemType type, const Vec2& tileCoord);
    bool init(MinesItemType type, const Vec2& tileCoord);

    MinesItemType getType() const { return _type; }
    const Vec2& getTileCoord() const { return _tileCoord; }
    void setTileCoord(const Vec2& coord) { _tileCoord = coord; }

protected:
    MinesItemType _type;
    Vec2 _tileCoord;
};

#endif // __MINES_ITEM_H__
