/****************************************************************
 * Project Name:  StardewValley
 * File Name:     BarnManager.h
 * File Function: BarnManager类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/16
 * License:       MIT License
 ****************************************************************/
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

// 畜棚管理器，负责动物、干草和产物
class BarnManager : public Ref {
public:
    // 获取单例
    static BarnManager* getInstance(GameMap* barn = nullptr);

    // 销毁单例
    static void destroyInstance();

    // 在指定位置添加干草
    bool addHayAt(const Vec2& tileCoord);

    // 添加动物
    bool addAnimal(AnimalType type);

    // 新的一天逻辑（消耗干草、生产物品）
    void onNewDay();

    // 启动动画
    void startAnimations();

    // 停止动画
    void stopAnimations();

    // 收集指定位置的产物
    ItemType collectProductionAt(const Vec2& tileCoord);

    // 获取所有干草位置
    std::vector<Vec2> getHayPositions() const;

    // 获取所有动物类型
    std::vector<int> getAnimalTypes() const;

    // 获取所有产物信息
    std::vector<std::pair<int, int>> getProductions() const;

    // 恢复数据
    void restoreData(const std::vector<Vec2>& hayPos,
        const std::vector<int>& animalTypes,
        const std::vector<std::pair<int, int>>& productions);

private:
    BarnManager() = default;
    ~BarnManager() override = default;

    BarnManager(const BarnManager&) = delete;
    BarnManager& operator=(const BarnManager&) = delete;

    bool init(GameMap* barn);

    // 清理资源
    void clear();

    // 坐标键值转换
    static long long keyFor(const Vec2& tileCoord);

    // 创建动物对象
    BarnAnimal* createAnimal(AnimalType type);

private:
    static BarnManager* _instance;

    // 地图引用
    GameMap* _gameMap = nullptr;
    TMXTiledMap* _tiledMap = nullptr;
    TMXLayer* _feedLayer = nullptr;

    // 位置中心点
    std::vector<Vec2> _nestCenters;
    std::vector<Vec2> _productionCenters;

    // 动物列表
    std::vector<BarnAnimal*> _animals;

    // 干草精灵映射
    std::unordered_map<long long, Sprite*> _haySprites;

    // 产物列表
    std::vector<std::vector<std::pair<Sprite*, ItemType>>> _productions;

    // 产物位置键值
    std::vector<long long> _productionTileKeys;
};


#endif // __BARN_MANAGER_H__
