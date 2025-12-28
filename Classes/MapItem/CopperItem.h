/****************************************************************
 * Project Name:  StardewValley
 * File Name:     CopperItem.h
 * File Function: CopperItem类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/28
 * License:       MIT License
 ****************************************************************/
#pragma once
#ifndef __COPPER_ITEM_H__
#define __COPPER_ITEM_H__

#include "EnvironmentItem.h"

class CopperItem : public EnvironmentItem {
public:
    static CopperItem* create(const cocos2d::Vec2& tileCoord);
    bool init(const cocos2d::Vec2& tileCoord);
};

#endif // __COPPER_ITEM_H__
