#pragma once
#ifndef __MINES_ITEM_MANAGER_H__
#define __MINES_ITEM_MANAGER_H__

#include "cocos2d.h"
#include "StoneItem.h"
#include "CopperItem.h"
#include "../GameMap/GameMap.h"
#include <unordered_map>

USING_NS_CC;

class MinesItemManager : public Ref {
public:
    static MinesItemManager* create(GameMap* gameMap);
    bool init(GameMap* gameMap);

    // 在指定坐标添加物品
    bool addItem(EnvironmentItemType type, const Vec2& tileCoord);
    
    // 移除指定坐标的物品
    bool removeItem(const Vec2& tileCoord);

    // 查询指定坐标是否有物品
    bool hasItem(const Vec2& tileCoord) const;

    // 获取指定坐标的物品
    EnvironmentItem* getItem(const Vec2& tileCoord) const;

    // 清除所有物品
    void clear();

    // 生成初始物品
    void spawnInitialItems();

private:
    GameMap* _gameMap;
    TMXTiledMap* _tiledMap;
    TMXLayer* _eventLayer;
    std::unordered_map<long long, EnvironmentItem*> _items;

    // 坐标转 key (x << 32 | y)
    static long long keyFor(const Vec2& tileCoord);

    // 去除 GID 的翻转标志位
    static unsigned int stripFlags(unsigned int gid);

    // 检查位置是否在 Stone 层上
    bool isStone(const Vec2& tileCoord) const;

    int _stoneCount;
    int _copperCount;
};

#endif // __MINES_ITEM_MANAGER_H__
