/****************************************************************
 * Project Name:  StardewValley
 * File Name:     EnvironmentItem.h
 * File Function: EnvironmentItem类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/16
 * License:       MIT License
 ****************************************************************/
#pragma once
#ifndef __FARM_ITEM_H__
#define __FARM_ITEM_H__

#include "cocos2d.h"
#include "Constant.h"

USING_NS_CC;

// 环境物品基类（如石头、木头等）
class EnvironmentItem : public Sprite {
public:
    // 静态创建函数
    static EnvironmentItem* create(EnvironmentItemType type, const Vec2& tileCoord);

    // 初始化
    bool init(EnvironmentItemType type, const Vec2& tileCoord);

    // 获取物品类型
    EnvironmentItemType getType() const { return _type; }

    // 获取/设置瓦片坐标
    const Vec2& getTileCoord() const { return _tileCoord; }
    void setTileCoord(const Vec2& coord) { _tileCoord = coord; }

protected:
    EnvironmentItemType _type; // 物品类型
    Vec2 _tileCoord;           // 瓦片坐标
};


#endif // __FARM_ITEM_H__
