/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Farm.h
 * File Function: Farm类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/28
 * License:       MIT License
 ****************************************************************/
#pragma once

#ifndef __FARM_H__
#define __FARM_H__

#include "GameMap.h"
#include "cocos2d.h"
#include "Constant.h"
#include "../MapItem/FarmItemManager.h"
#include "../MapItem/CultivationManager.h"
#include "../Shop/ShopLayer.h"
#include "../Shop/ShopMenuLayer.h"
#include "../Player/SkillLevel.h"

USING_NS_CC;

// 农场场景类
class Farm : public GameMap
{
public:
    // 静态创建与销毁
    static Farm* create();
    static GameMap* getInstance();
    static void destroyInstance();

    // 初始化
    virtual bool init() override;

    // 场景切换逻辑
    virtual MapType leaveMap(const Vec2& curPos, bool isStart, const Direction& direction) override;
    virtual void IntoMap(MapType lastMap) override;

    // 获取玩家初始位置
    virtual Vec2 getPlayerStartPosition(MapType lastMap) override;

    // 摄像机跟随与碰撞检测
    virtual bool isCameraFollow() const noexcept { return true; }
    virtual bool isCollidable(Vec2 worldPos) override;

    // 交互事件处理
    virtual MouseEvent onLeftClick(const Vec2& playerPos, const Direction direction, ItemType objects) override;
    virtual MouseEvent onRightClick(const Vec2& playerPos, const Direction direction) override;

    // 是否为户外场景
    virtual bool isOutdoor() noexcept override { return true; }

    // 打开交易界面
    void openShopForNPC();

private:
    static GameMap* _instance;

    // 管理器
    FarmItemManager* _farmItemManager = nullptr;
    CultivationManager* _cultivationManager = nullptr;
};


#endif // __FARM_H__
