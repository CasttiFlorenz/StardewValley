#pragma once
#ifndef __FARM_ITEM_MANAGER_H__
#define __FARM_ITEM_MANAGER_H__

#include "cocos2d.h"
#include "EnvironmentItem.h"
#include "WoodItem.h"
#include "GrassItem.h"
#include "DaffodilsItem.h"
#include "LeekItem.h"
#include "../GameMap/GameMap.h"
#include <unordered_map>

USING_NS_CC;

class FarmItemManager : public Ref {
public:
    static FarmItemManager* create(GameMap* gameMap);
    bool init(GameMap* gameMap);

    bool addItem(EnvironmentItemType type, const Vec2& tileCoord);
    
    bool removeItem(const Vec2& tileCoord);

    bool hasItem(const Vec2& tileCoord) const;

    EnvironmentItem* getItem(const Vec2& tileCoord) const;

    void clear();

    void spawnInitialItems();

private:
    GameMap* _gameMap;
    TMXTiledMap* _tiledMap;
    TMXLayer* _eventLayer;
    std::unordered_map<long long, EnvironmentItem*> _items;

    static long long keyFor(const Vec2& tileCoord);

    static unsigned int stripFlags(unsigned int gid);

    bool isCultivated(const Vec2& tileCoord) const;

    int _woodCount;
    int _grassCount;
    int _daffodilsCount;
    int _leekCount;
};

#endif // __FARM_ITEM_MANAGER_H__
