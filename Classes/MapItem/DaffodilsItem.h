/****************************************************************
 * Project Name:  StardewValley
 * File Name:     DaffodilsItem.h
 * File Function: DaffodilsItem类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/28
 * License:       MIT License
 ****************************************************************/
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
