/****************************************************************
 * Project Name:  StardewValley
 * File Name:     WoodItem.h
 * File Function: WoodItem类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/28
 * License:       MIT License
 ****************************************************************/
#pragma once
#ifndef __WOOD_ITEM_H__
#define __WOOD_ITEM_H__

#include "EnvironmentItem.h"

class WoodItem : public EnvironmentItem {
public:
    static WoodItem* create(const cocos2d::Vec2& tileCoord);
    bool init(const cocos2d::Vec2& tileCoord);
};


#endif // __WOOD_ITEM_H__
