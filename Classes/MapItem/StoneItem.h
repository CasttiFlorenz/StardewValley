/****************************************************************
 * Project Name:  StardewValley
 * File Name:     StoneItem.h
 * File Function: StoneItem类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/28
 * License:       MIT License
 ****************************************************************/
#pragma once
#ifndef __STONE_ITEM_H__
#define __STONE_ITEM_H__

#include "EnvironmentItem.h"

class StoneItem : public EnvironmentItem {
public:
    static StoneItem* create(const cocos2d::Vec2& tileCoord);
    bool init(const cocos2d::Vec2& tileCoord);
};

#endif // __STONE_ITEM_H__
