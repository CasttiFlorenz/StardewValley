#pragma once
#ifndef __MINES_ITEM_MANAGER_H__
#define __MINES_ITEM_MANAGER_H__

#include "cocos2d.h"
#include "StoneItem.h"
#include "CopperItem.h"
#include "../GameMap/GameMap.h"
#include <unordered_map>

USING_NS_CC;

// 矿洞环境物品管理器（石头、铜矿等）
class MinesItemManager : public Ref {
public:
    // 获取单例（首次调用需要传入 GameMap）
    static MinesItemManager* getInstance(GameMap* gameMap = nullptr);

    // 销毁单例并清理所有物品
    static void destroyInstance();

    // 在指定瓦片生成物品
    bool addItem(EnvironmentItemType type, const Vec2& tileCoord);

    // 移除指定瓦片上的物品
    bool removeItem(const Vec2& tileCoord);

    // 判断该瓦片是否已有物品
    bool hasItem(const Vec2& tileCoord) const;

    // 获取指定瓦片上的物品
    EnvironmentItem* getItem(const Vec2& tileCoord) const;

    // 清空所有生成的物品
    void clear();

    // 生成初始矿物
    void spawnInitialItems();

    // 新的一天触发
    void onNewDay();

private:
    // 单例实例
    static MinesItemManager* _instance;

    // 禁止外部创建和拷贝
    MinesItemManager() = default;
    ~MinesItemManager() override = default;
    MinesItemManager(const MinesItemManager&) = delete;
    MinesItemManager& operator=(const MinesItemManager&) = delete;

    // 内部初始化
    bool init(GameMap* gameMap);

    GameMap* _gameMap = nullptr;
    TMXTiledMap* _tiledMap = nullptr;
    TMXLayer* _eventLayer = nullptr;

    // 瓦片坐标 -> 物品
    std::unordered_map<long long, EnvironmentItem*> _items;

    // 坐标转 key
    static long long keyFor(const Vec2& tileCoord);

    // 去除 TMX 翻转标志
    static unsigned int stripFlags(unsigned int gid);

    // 判断该瓦片是否为可生成石头区域
    bool isStone(const Vec2& tileCoord) const;

    int _stoneCount = 0;
    int _copperCount = 0;
};

#endif // __MINES_ITEM_MANAGER_H__
