#pragma once
#ifndef __STONE_ITEM_H__
#define __STONE_ITEM_H__

#include "EnvironmentItem.h"

class StoneItem : public EnvironmentItem {
public:
    // 创建一个位于指定瓦片坐标的石头环境物体
    static StoneItem* create(const cocos2d::Vec2& tileCoord);
    // 初始化石头环境物体（设置类型与纹理）
    bool init(const cocos2d::Vec2& tileCoord);
};

#endif // __STONE_ITEM_H__
