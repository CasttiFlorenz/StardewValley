#pragma once
#ifndef __FARM_ITEM_MANAGER_H__
#define __FARM_ITEM_MANAGER_H__

#include "cocos2d.h"
#include "FarmItem.h"
#include "WoodItem.h"
#include "GrassItem.h"
#include "CultivatedItem.h"
#include "../GameMap/GameMap.h"
#include <unordered_map>

USING_NS_CC;

class FarmItemManager : public Ref {
public:
    static FarmItemManager* create(GameMap* gameMap);
    bool init(GameMap* gameMap);

    // 尝试在指定瓦片坐标放置物品
    bool addItem(FarmItemType type, const Vec2& tileCoord);
    
    // 移除指定坐标的物品
    bool removeItem(const Vec2& tileCoord);

    // 查询指定坐标是否有物品
    bool hasItem(const Vec2& tileCoord) const;

    // 获取指定坐标的物品
    FarmItem* getItem(const Vec2& tileCoord) const;

    // 清空所有物品
    void clear();

    // 随机生成初始物品
    void spawnInitialItems();

private:
    GameMap* _gameMap;
    TMXTiledMap* _tiledMap;
    TMXLayer* _eventLayer;
    std::unordered_map<long long, FarmItem*> _items;

    // 坐标转 key (x << 32 | y)
    static long long keyFor(const Vec2& tileCoord);

    // 去除 GID 的翻转标志位
    static unsigned int stripFlags(unsigned int gid);

    // 检查该位置是否允许耕种 (event 层 Cultivated 属性为 1)
    bool isCultivated(const Vec2& tileCoord) const;
};

#endif // __FARM_ITEM_MANAGER_H__
