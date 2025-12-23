#pragma once
#ifndef __MINES_ITEM_MANAGER_H__
#define __MINES_ITEM_MANAGER_H__

#include "cocos2d.h"
#include "StoneItem.h"
#include "CopperItem.h"
#include "../GameMap/GameMap.h"
#include <unordered_map>

USING_NS_CC;

class MinesItemManager : public Ref {
public:
    // 获取单例（首次需要传入 GameMap）
    static MinesItemManager* getInstance(GameMap* gameMap = nullptr);

    // 销毁单例
    static void destroyInstance();

    bool addItem(EnvironmentItemType type, const Vec2& tileCoord);
    bool removeItem(const Vec2& tileCoord);
    bool hasItem(const Vec2& tileCoord) const;
    EnvironmentItem* getItem(const Vec2& tileCoord) const;
    void clear();
    void spawnInitialItems();
    void onNewDay();

private:
    // 单例指针
    static MinesItemManager* _instance;

    // 构造 / 析构私有化
    MinesItemManager() = default;
    ~MinesItemManager() override = default;

    // 禁止拷贝
    MinesItemManager(const MinesItemManager&) = delete;
    MinesItemManager& operator=(const MinesItemManager&) = delete;

    // 初始化（仅内部调用一次）
    bool init(GameMap* gameMap);

    GameMap* _gameMap = nullptr;
    TMXTiledMap* _tiledMap = nullptr;
    TMXLayer* _eventLayer = nullptr;

    std::unordered_map<long long, EnvironmentItem*> _items;

    static long long keyFor(const Vec2& tileCoord);
    static unsigned int stripFlags(unsigned int gid);
    bool isStone(const Vec2& tileCoord) const;

    int _stoneCount = 0;
    int _copperCount = 0;
};

#endif // __MINES_ITEM_MANAGER_H__
