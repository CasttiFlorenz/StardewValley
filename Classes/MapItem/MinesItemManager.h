/****************************************************************
 * Project Name:  StardewValley
 * File Name:     MinesItemManager.h
 * File Function: MinesItemManager类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/28
 * License:       MIT License
 ****************************************************************/
#pragma once
#ifndef __MINES_ITEM_MANAGER_H__
#define __MINES_ITEM_MANAGER_H__

#include "cocos2d.h"
#include "Constant.h"
#include "StoneItem.h"
#include "CopperItem.h"
#include "../GameMap/GameMap.h"

USING_NS_CC;

// 矿洞环境物品管理器（负责生成和管理石头、铜矿）
class MinesItemManager : public Ref {
public:
    // 存档数据结构
    struct MineItemData {
        EnvironmentItemType type;
        float x;
        float y;
    };

    // 获取单例对象（首次调用需要传入 gameMap）
    static MinesItemManager* getInstance(GameMap* gameMap = nullptr);

    // 销毁单例对象
    static void destroyInstance();

    // 在指定瓦片坐标添加物品
    bool addItem(EnvironmentItemType type, const Vec2& tileCoord);

    // 移除指定瓦片坐标的物品
    bool removeItem(const Vec2& tileCoord);

    // 检查指定位置是否有物品
    bool hasItem(const Vec2& tileCoord) const;

    // 获取指定位置的物品对象
    EnvironmentItem* getItem(const Vec2& tileCoord) const;

    // 清空所有物品
    void clear();

    // 生成初始物品（在初始化或新的一天调用）
    void spawnInitialItems();

    // 新的一天更新逻辑
    void onNewDay();

    // 获取所有物品数据（用于存档）
    std::vector<MineItemData> getItems() const;

    // 恢复存档数据
    void restoreData(const std::vector<MineItemData>& items);

private:
    static MinesItemManager* _instance;

    MinesItemManager() = default;
    ~MinesItemManager() override = default;
    MinesItemManager(const MinesItemManager&) = delete;
    MinesItemManager& operator=(const MinesItemManager&) = delete;

    // 初始化管理器
    bool init(GameMap* gameMap);

    // 获取瓦片坐标对应的唯一键值
    static long long keyFor(const Vec2& tileCoord);

    // 移除 TMX 翻转标志，获取原始 GID
    static unsigned int stripFlags(unsigned int gid);

    // 检查该位置是否允许生成矿石（基于 TMX 属性）
    bool isStone(const Vec2& tileCoord) const;

private:
    GameMap* _gameMap = nullptr;
    TMXTiledMap* _tiledMap = nullptr;
    TMXLayer* _eventLayer = nullptr;

    // 物品映射表 (key: tileCoord -> value: EnvironmentItem*)
    std::unordered_map<long long, EnvironmentItem*> _items;

    // 物品计数
    int _stoneCount = 0;
    int _copperCount = 0;
};


#endif // __MINES_ITEM_MANAGER_H__
