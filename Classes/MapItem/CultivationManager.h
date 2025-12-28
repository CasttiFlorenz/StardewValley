/****************************************************************
 * Project Name:  StardewValley
 * File Name:     CultivationManager.h
 * File Function: CultivationManager类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/16
 * License:       MIT License
 ****************************************************************/
#pragma once
#ifndef __CULTIVATION_MANAGER_H__
#define __CULTIVATION_MANAGER_H__

#include "cocos2d.h"
#include "CultivatedSoil.h"
#include "FarmItemManager.h"
#include "../Time/TimeManager.h"

USING_NS_CC;

// 耕种管理器（负责所有耕地和作物的生命周期）
class CultivationManager : public Ref
{
public:
    // 存档数据结构
    struct SoilSaveData {
        float x;
        float y;
        int status;     // 土壤状态
        int cropType;   // 作物类型
        int cropStage;  // 生长阶段
        int cropStatus; // 作物状态
    };

    // 获取单例
    static CultivationManager* getInstance(
        FarmItemManager* farmItemManager = nullptr,
        GameMap* gameMap = nullptr
    );

    // 销毁单例
    static void destroyInstance();

    // 初始化
    bool init(FarmItemManager* farmItemManager, GameMap* gameMap);

    // 尝试开垦土地
    bool attemptCultivate(const Vec2& tileCoord);

    // 浇水
    bool waterSoil(const Vec2& tileCoord);

    // 种植作物
    bool plantCrop(const Vec2& tileCoord, ItemType type);

    // 新的一天更新
    void onNewDay();

    // 移除耕地
    bool removeSoil(const Vec2& tileCoord);

    // 收获作物
    ItemType harvestCrop(const Vec2& tileCoord);

    // 全图浇水（调试/测试用）
    void waterAllSoils();

    // 获取存档数据
    std::vector<SoilSaveData> getSoilsData() const;

    // 恢复存档数据
    void restoreData(const std::vector<SoilSaveData>& data);

private:
    CultivationManager();
    ~CultivationManager();

    CultivationManager(const CultivationManager&) = delete;
    CultivationManager& operator=(const CultivationManager&) = delete;

    // 获取瓦片唯一键
    static long long keyFor(const Vec2& tileCoord);

private:
    static CultivationManager* _instance;

    FarmItemManager* _farmItemManager; // 农场物品管理器引用
    GameMap* _gameMap;                 // 游戏地图引用
    TMXTiledMap* _tiledMap;            // Tiled地图引用

    // 耕地映射表 (key: tileCoord -> value: CultivatedSoil*)
    std::unordered_map<long long, CultivatedSoil*> _soils;
};


#endif // __CULTIVATION_MANAGER_H__
