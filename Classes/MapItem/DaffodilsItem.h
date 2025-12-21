#pragma once
#ifndef __DAFFODILS_ITEM_H__
#define __DAFFODILS_ITEM_H__

#include "EnvironmentItem.h"

class DaffodilsItem : public EnvironmentItem {
public:
    static DaffodilsItem* create(const cocos2d::Vec2& tileCoord);
    bool init(const cocos2d::Vec2& tileCoord);
};

#endif // __DAFFODILS_ITEM_H__