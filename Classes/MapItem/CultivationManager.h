#pragma once
#ifndef __CULTIVATION_MANAGER_H__
#define __CULTIVATION_MANAGER_H__

#include "cocos2d.h"
#include "CultivatedSoil.h"
#include "FarmItemManager.h"
#include "../Time/TimeManager.h"
#include <unordered_map>

class CultivationManager {
public:
    static CultivationManager* getInstance();

    void init(FarmItemManager* farmItemManager, GameMap* gameMap);

    bool attemptCultivate(const Vec2& tileCoord);

    bool waterSoil(const Vec2& tileCoord);

    bool plantCrop(const Vec2& tileCoord, CropType type);

    void onNewDay();
    bool removeSoil(const Vec2& tileCoord);
    CropType harvestCrop(const Vec2& tileCoord);

private:
    CultivationManager();
    ~CultivationManager();

    static CultivationManager* _instance;
    FarmItemManager* _farmItemManager;
    GameMap* _gameMap;
    TMXTiledMap* _tiledMap;
    
    std::unordered_map<long long, CultivatedSoil*> _soils;

    static long long keyFor(const Vec2& tileCoord);
};

#endif // __CULTIVATION_MANAGER_H__
