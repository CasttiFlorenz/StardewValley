/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Mines.h
 * File Function: Mines类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/23
 * License:       MIT License
 ****************************************************************/
#pragma once
#ifndef __MINES_H__
#define __MINES_H__

#include "GameMap.h"
#include "cocos2d.h"
#include "Constant.h"
#include "../MapItem/MinesItemManager.h"

USING_NS_CC;

// 矿洞场景类
class Mines : public GameMap
{
public:
    // 静态创建与销毁
    static Mines* create();
    static GameMap* getInstance();
    static void destroyInstance();

    // 初始化
    virtual bool init() override;

    // 场景切换逻辑
    virtual MapType leaveMap(const Vec2& curPos,
        bool isStart,
        const Direction& direction) override;

    // 进入地图初始化
    virtual void IntoMap(MapType lastMap) override;

    // 获取玩家初始位置
    virtual Vec2 getPlayerStartPosition(MapType lastMap) override;

    // 摄像机跟随与碰撞检测
    virtual bool isCameraFollow() const noexcept override { return true; }
    virtual bool isCollidable(Vec2 worldPos) override;

    // 左键点击处理（挖掘）
    virtual MouseEvent onLeftClick(const Vec2& playerPos,
        const Direction direction,
        ItemType objects) override;

private:
    static GameMap* _instance;

    // 矿洞物品管理器
    MinesItemManager* _minesItemManager = nullptr;
};


#endif // __MINES_H__
