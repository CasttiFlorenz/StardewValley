#pragma once
#ifndef __CULTIVATED_ITEM_H__
#define __CULTIVATED_ITEM_H__

#include "EnvironmentItem.h"

class CultivatedItem : public EnvironmentItem {
public:
    static CultivatedItem* create(const cocos2d::Vec2& tileCoord) {
        auto p = new (std::nothrow) CultivatedItem();
        if (p && p->init(EnvironmentItemType::CULTIUVATED, tileCoord)) {
            p->autorelease();
            return p;
        }
        CC_SAFE_DELETE(p);
        return nullptr;
    }
};

#endif // __CULTIVATED_ITEM_H__
