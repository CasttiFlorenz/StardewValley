#pragma once
#ifndef __CULTIVATION_MANAGER_H__
#define __CULTIVATION_MANAGER_H__

#include "cocos2d.h"
#include "CultivatedSoil.h"
#include "FarmItemManager.h"
#include "../Time/TimeManager.h"
#include <unordered_map>

USING_NS_CC;

// 耕地管理器：
// 负责管理所有已开垦土壤的创建、查询、每日更新与销毁
// 逻辑上与 FarmItemManager / GameMap 协作
class CultivationManager : public Ref
{
public:
    // 获取单例（首次可传入依赖对象）
    static CultivationManager* getInstance(
        FarmItemManager* farmItemManager = nullptr,
        GameMap* gameMap = nullptr
    );

    // 销毁单例
    static void destroyInstance();

    // 初始化 / 重新初始化（地图切换时可调用）
    bool init(FarmItemManager* farmItemManager, GameMap* gameMap);

    // 尝试在指定瓦片上开垦土地
    bool attemptCultivate(const Vec2& tileCoord);

    // 给指定瓦片上的耕地浇水
    bool waterSoil(const Vec2& tileCoord);

    // 在指定耕地上种植作物
    bool plantCrop(const Vec2& tileCoord, ItemType type);

    // 新的一天到来时调用
    void onNewDay();

    // 移除指定瓦片上的耕地
    bool removeSoil(const Vec2& tileCoord);

    // 收获指定瓦片上的作物
    ItemType harvestCrop(const Vec2& tileCoord);

    // 一次性给所有耕地浇水
    void waterAllSoils();

private:
    static CultivationManager* _instance;

    CultivationManager();
    ~CultivationManager();

    CultivationManager(const CultivationManager&) = delete;
    CultivationManager& operator=(const CultivationManager&) = delete;

private:
    FarmItemManager* _farmItemManager; // 农场物品管理器（不拥有）
    GameMap* _gameMap;                 // 地图对象（不拥有）
    TMXTiledMap* _tiledMap;            // TMX 地图缓存指针

    // key = tileCoord 生成的唯一 key
    std::unordered_map<long long, CultivatedSoil*> _soils;

    // 根据瓦片坐标生成唯一 key
    static long long keyFor(const Vec2& tileCoord);
};

#endif // __CULTIVATION_MANAGER_H__
