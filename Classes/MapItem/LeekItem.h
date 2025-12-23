#pragma once
#ifndef __LEEK_ITEM_H__
#define __LEEK_ITEM_H__

#include "EnvironmentItem.h"

class LeekItem : public EnvironmentItem {
public:
    // 创建一个位于指定瓦片坐标的葱/韭菜环境物体
    static LeekItem* create(const cocos2d::Vec2& tileCoord);
    // 初始化葱/韭菜环境物体（设置类型与纹理）
    bool init(const cocos2d::Vec2& tileCoord);
};

#endif // __LEEK_ITEM_H__
