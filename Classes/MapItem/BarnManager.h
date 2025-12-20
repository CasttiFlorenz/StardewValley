#pragma once
#ifndef __BARN_MANAGER_H__
#define __BARN_MANAGER_H__

#include "cocos2d.h"
#include "../GameMap/GameMap.h"
#include "../Time/TimeManager.h"
#include "BarnAnimal.h"
#include "Chicken.h"
#include "Cow.h"
#include "Constant.h"
#include <unordered_map>
#include <vector>

USING_NS_CC;


class BarnManager : public Ref {
public:
    static BarnManager* getInstance();
    static void destroyInstance();
    bool init(GameMap* barn);

    bool addHayAt(const Vec2& tileCoord);
    bool addAnimal(BarnAnimalType type);
    void onNewDay();
    void startAnimations();
    void stopAnimations();
    Objects collectProductionAt(const Vec2& tileCoord);

private:
    static BarnManager* _instance;
    GameMap* _gameMap;
    TMXTiledMap* _tiledMap;
    TMXLayer* _feedLayer;
    std::vector<Vec2> _nestCenters;
    std::vector<Vec2> _productionCenters;
    std::vector<BarnAnimal*> _animals;
    std::unordered_map<long long, Sprite*> _haySprites;
    std::vector<std::vector<std::pair<Sprite*, Objects>>> _productions;
    std::vector<long long> _productionTileKeys;

    static long long keyFor(const Vec2& tileCoord);
    BarnAnimal* createAnimal(BarnAnimalType type);
};

#endif // __BARN_MANAGER_H__
