#pragma once
#ifndef __GRASS_ITEM_H__
#define __GRASS_ITEM_H__

#include "EnvironmentItem.h"

class GrassItem : public EnvironmentItem {
public:
    // 创建一个位于指定瓦片坐标的草丛环境物体
    static GrassItem* create(const cocos2d::Vec2& tileCoord);
    // 初始化草丛环境物体（设置类型与纹理）
    bool init(const cocos2d::Vec2& tileCoord);
};

#endif // __GRASS_ITEM_H__
