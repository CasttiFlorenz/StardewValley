#pragma once
#ifndef __FARM_ITEM_H__
#define __FARM_ITEM_H__

#include "cocos2d.h"
#include "Constant.h"

USING_NS_CC;

// 环境物体基类：
// 所有放置在地图上的环境对象（耕地、石头、树等）都应继承该类
class EnvironmentItem : public Sprite {
public:
    // 创建指定类型、指定瓦片坐标的环境物体
    static EnvironmentItem* create(EnvironmentItemType type, const Vec2& tileCoord);
    bool init(EnvironmentItemType type, const Vec2& tileCoord);

    // 获取环境物体类型
    EnvironmentItemType getType() const { return _type; }

    // 获取 / 设置瓦片坐标（逻辑坐标）
    const Vec2& getTileCoord() const { return _tileCoord; }
    void setTileCoord(const Vec2& coord) { _tileCoord = coord; }

protected:
    EnvironmentItemType _type; // 环境物体类型
    Vec2 _tileCoord;           // 所在瓦片坐标（非世界坐标）
};

#endif // __FARM_ITEM_H__
