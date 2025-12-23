#pragma once
#ifndef __WOOD_ITEM_H__
#define __WOOD_ITEM_H__

#include "EnvironmentItem.h"

class WoodItem : public EnvironmentItem {
public:
    // 创建一个位于指定瓦片坐标的木材环境物体
    static WoodItem* create(const cocos2d::Vec2& tileCoord);
    // 初始化木材环境物体（设置类型与纹理）
    bool init(const cocos2d::Vec2& tileCoord);
};

#endif // __WOOD_ITEM_H__
