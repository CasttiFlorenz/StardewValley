#pragma once
#ifndef __FARM_ITEM_MANAGER_H__
#define __FARM_ITEM_MANAGER_H__

#include "cocos2d.h"
#include "EnvironmentItem.h"
#include "WoodItem.h"
#include "GrassItem.h"
#include "DaffodilsItem.h"
#include "LeekItem.h"
#include "CultivatedSoil.h"
#include "../GameMap/GameMap.h"
#include <unordered_map>

USING_NS_CC;

class FarmItemManager : public Ref {
public:
    // 单例获取（首次必须传 GameMap）
    static FarmItemManager* getInstance(GameMap* gameMap = nullptr);

    // 销毁单例（离开 Farm 地图时）
    static void destroyInstance();

    bool addItem(EnvironmentItemType type, const Vec2& tileCoord);
    bool removeItem(const Vec2& tileCoord);
    bool removeCultivation(const Vec2& tileCoord);
    bool hasItem(const Vec2& tileCoord) const;
    bool isCollidable(const Vec2& tileCoord) const;
    EnvironmentItem* getItem(const Vec2& tileCoord) const;
    bool isCultivated(const Vec2& tileCoord) const;
    void onNewDay();

private:
    static FarmItemManager* _instance;

    FarmItemManager() = default;
    ~FarmItemManager() override = default;

    FarmItemManager(const FarmItemManager&) = delete;
    FarmItemManager& operator=(const FarmItemManager&) = delete;

    bool init(GameMap* gameMap);
    void clear();
    void spawnInitialItems();

    GameMap* _gameMap = nullptr;
    TMXTiledMap* _tiledMap = nullptr;
    TMXLayer* _eventLayer = nullptr;

    std::unordered_map<long long, EnvironmentItem*> _items;
    std::unordered_map<long long, bool> _cultivatedSoils;

    static long long keyFor(const Vec2& tileCoord);
    static unsigned int stripFlags(unsigned int gid);

    int _woodCount = 0;
    int _grassCount = 0;
    int _daffodilsCount = 0;
    int _leekCount = 0;
};

#endif // __FARM_ITEM_MANAGER_H__
