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

// 农场环境物品管理器（木头、杂草、花、韭葱、耕地等）
class FarmItemManager : public Ref {
public:
    // 获取单例（首次必须传入 GameMap）
    static FarmItemManager* getInstance(GameMap* gameMap = nullptr);

    // 销毁单例并清空所有物品
    static void destroyInstance();

    // 在指定瓦片添加物品
    bool addItem(EnvironmentItemType type, const Vec2& tileCoord);

    // 移除指定瓦片上的物品
    bool removeItem(const Vec2& tileCoord);

    // 移除指定瓦片上的耕地标记
    bool removeCultivation(const Vec2& tileCoord);

    // 判断该瓦片是否已有物品或耕地
    bool hasItem(const Vec2& tileCoord) const;

    // 判断该瓦片是否可碰撞
    bool isCollidable(const Vec2& tileCoord) const;

    // 获取指定瓦片上的物品
    EnvironmentItem* getItem(const Vec2& tileCoord) const;

    // 判断该瓦片是否为可耕种区域
    bool isCultivated(const Vec2& tileCoord) const;

    // 新的一天触发
    void onNewDay();

private:
    static FarmItemManager* _instance;

    // 禁止外部创建和拷贝
    FarmItemManager() = default;
    ~FarmItemManager() override = default;
    FarmItemManager(const FarmItemManager&) = delete;
    FarmItemManager& operator=(const FarmItemManager&) = delete;

    // 内部初始化
    bool init(GameMap* gameMap);

    // 清空所有数据
    void clear();

    // 随机生成初始物品
    void spawnInitialItems();

private:
    GameMap* _gameMap = nullptr;
    TMXTiledMap* _tiledMap = nullptr;
    TMXLayer* _eventLayer = nullptr;

    // 瓦片坐标 -> 物品
    std::unordered_map<long long, EnvironmentItem*> _items;

    // 已耕地标记
    std::unordered_map<long long, bool> _cultivatedSoils;

    // 坐标转 key
    static long long keyFor(const Vec2& tileCoord);

    // 去除 TMX 翻转标志
    static unsigned int stripFlags(unsigned int gid);

    int _woodCount = 0;
    int _grassCount = 0;
    int _daffodilsCount = 0;
    int _leekCount = 0;
};

#endif // __FARM_ITEM_MANAGER_H__
