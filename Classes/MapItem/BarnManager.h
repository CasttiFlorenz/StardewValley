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

USING_NS_CC;

class BarnManager : public Ref {
public:
    // 单例获取（首次调用需传入 barn 地图指针）
    static BarnManager* getInstance(GameMap* barn = nullptr);

    // 释放单例
    static void destroyInstance();

    // 在指定瓦片坐标添加一捆饲料（hay）
    bool addHayAt(const Vec2& tileCoord);
    // 添加一个指定类型的动物到空闲巢位
    bool addAnimal(AnimalType type);
    // 每天开始时的刷新逻辑：消耗饲料、生成产物
    void onNewDay();
    // 启动所有动物动画
    void startAnimations();
    // 停止所有动物动画
    void stopAnimations();
    // 收集指定产物点位上的产物
    ItemType collectProductionAt(const Vec2& tileCoord);

private:
    static BarnManager* _instance;

    BarnManager() = default;
    ~BarnManager() override = default;

    BarnManager(const BarnManager&) = delete;
    BarnManager& operator=(const BarnManager&) = delete;

    bool init(GameMap* barn);

private:
    // 地图与图层
    GameMap* _gameMap = nullptr;
    TMXTiledMap* _tiledMap = nullptr;
    TMXLayer* _feedLayer = nullptr;

    // 巢位中心与产物中心位置
    std::vector<Vec2> _nestCenters;
    std::vector<Vec2> _productionCenters;
    // 圈舍内动物
    std::vector<BarnAnimal*> _animals;
    // 饲料精灵（使用 tileKey 作为键）
    std::unordered_map<long long, Sprite*> _haySprites;
    // 各巢位的待收集产物队列：精灵与物品类型
    std::vector<std::vector<std::pair<Sprite*, ItemType>>> _productions;
    // 产物点位对应的瓦片键
    std::vector<long long> _productionTileKeys;

    static long long keyFor(const Vec2& tileCoord);
    BarnAnimal* createAnimal(AnimalType type);

    // 统一清理资源（供 destroyInstance 内部调用）
    void clear();
};

#endif // __BARN_MANAGER_H__
