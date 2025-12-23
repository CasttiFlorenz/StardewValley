#pragma once
#ifndef __DAFFODILS_ITEM_H__
#define __DAFFODILS_ITEM_H__

#include "EnvironmentItem.h"

class DaffodilsItem : public EnvironmentItem {
public:
    // 创建一个位于指定瓦片坐标的黄水仙环境物体
    static DaffodilsItem* create(const cocos2d::Vec2& tileCoord);
    // 初始化黄水仙环境物体（设置类型与纹理）
    bool init(const cocos2d::Vec2& tileCoord);
};

#endif // __DAFFODILS_ITEM_H__
