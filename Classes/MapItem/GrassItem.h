/****************************************************************
 * Project Name:  StardewValley
 * File Name:     GrassItem.h
 * File Function: GrassItem类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/28
 * License:       MIT License
 ****************************************************************/
#pragma once
#ifndef __GRASS_ITEM_H__
#define __GRASS_ITEM_H__

#include "EnvironmentItem.h"

class GrassItem : public EnvironmentItem {
public:
    static GrassItem* create(const cocos2d::Vec2& tileCoord);
    bool init(const cocos2d::Vec2& tileCoord);
};


#endif // __GRASS_ITEM_H__
