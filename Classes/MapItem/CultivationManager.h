#pragma once
#ifndef __CULTIVATION_MANAGER_H__
#define __CULTIVATION_MANAGER_H__

#include "cocos2d.h"
#include "CultivatedSoil.h"
#include "FarmItemManager.h"
#include "../Time/TimeManager.h"
#include <unordered_map>

USING_NS_CC;

class CultivationManager : public Ref
{
public:
    static CultivationManager* getInstance(
        FarmItemManager* farmItemManager = nullptr,
        GameMap* gameMap = nullptr
    );
    static void destroyInstance();

    bool init(FarmItemManager* farmItemManager, GameMap* gameMap);

    bool attemptCultivate(const Vec2& tileCoord);
    bool waterSoil(const Vec2& tileCoord);
    bool plantCrop(const Vec2& tileCoord, ItemType type);

    void onNewDay();
    bool removeSoil(const Vec2& tileCoord);
    ItemType harvestCrop(const Vec2& tileCoord);

private:
    static CultivationManager* _instance;

    CultivationManager();
    ~CultivationManager();

    CultivationManager(const CultivationManager&) = delete;
    CultivationManager& operator=(const CultivationManager&) = delete;

    FarmItemManager* _farmItemManager;
    GameMap* _gameMap;
    TMXTiledMap* _tiledMap;

    std::unordered_map<long long, CultivatedSoil*> _soils;

    static long long keyFor(const Vec2& tileCoord);
};

#endif // __CULTIVATION_MANAGER_H__
