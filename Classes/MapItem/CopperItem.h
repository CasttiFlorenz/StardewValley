#pragma once
#ifndef __COPPER_ITEM_H__
#define __COPPER_ITEM_H__

#include "EnvironmentItem.h"

class CopperItem : public EnvironmentItem {
public:
    // 创建一个位于指定瓦片坐标的铜矿环境物体
    static CopperItem* create(const cocos2d::Vec2& tileCoord);
    // 初始化铜矿环境物体（设置类型与纹理）
    bool init(const cocos2d::Vec2& tileCoord);
};

#endif // __COPPER_ITEM_H__
