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

USING_NS_CC;

// 农场物品管理器（木头、杂草、采集物等）
class FarmItemManager : public Ref {
public:
    // 存档数据结构
    struct ItemData {
        EnvironmentItemType type;
        float x;
        float y;
    };

    // 获取单例
    static FarmItemManager* getInstance(GameMap* gameMap = nullptr);

    // 销毁单例
    static void destroyInstance();

    // 添加物品
    bool addItem(EnvironmentItemType type, const Vec2& tileCoord);

    // 移除物品
    bool removeItem(const Vec2& tileCoord);

    // 移除耕地标记
    bool removeCultivation(const Vec2& tileCoord);

    // 检查是否有物品
    bool hasItem(const Vec2& tileCoord) const;

    // 检查碰撞
    bool isCollidable(const Vec2& tileCoord) const;

    // 获取物品对象
    EnvironmentItem* getItem(const Vec2& tileCoord) const;

    // 检查是否为耕地
    bool isCultivated(const Vec2& tileCoord) const;

    // 新的一天更新
    void onNewDay();

    // 获取所有物品数据
    std::vector<ItemData> getItems() const;

    // 恢复存档数据
    void restoreData(const std::vector<ItemData>& items);

private:
    static FarmItemManager* _instance;

    FarmItemManager() = default;
    ~FarmItemManager() override = default;
    FarmItemManager(const FarmItemManager&) = delete;
    FarmItemManager& operator=(const FarmItemManager&) = delete;

    // 初始化
    bool init(GameMap* gameMap);

    // 清理资源
    void clear();

    // 生成初始物品
    void spawnInitialItems();

    // 坐标键值转换
    static long long keyFor(const Vec2& tileCoord);

    // 移除 TMX 翻转标志
    static unsigned int stripFlags(unsigned int gid);

private:
    GameMap* _gameMap = nullptr;
    TMXTiledMap* _tiledMap = nullptr;
    TMXLayer* _eventLayer = nullptr;

    // 物品映射表
    std::unordered_map<long long, EnvironmentItem*> _items;

    // 耕地标记
    std::unordered_map<long long, bool> _cultivatedSoils;

    // 物品计数
    int _woodCount = 0;
    int _grassCount = 0;
    int _daffodilsCount = 0;
    int _leekCount = 0;
};

#endif // __FARM_ITEM_MANAGER_H__