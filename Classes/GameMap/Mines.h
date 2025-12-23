#pragma once
#ifndef __MINES_H__
#define __MINES_H__

#include "GameMap.h"
#include "cocos2d.h"
#include "Constant.h"
#include "../MapItem/MinesItemManager.h"

USING_NS_CC;

// 矿洞地图
class Mines : public GameMap
{
public:
    // 创建 Mines 对象
    static Mines* create();

    // 获取 Mines 单例
    static GameMap* getInstance();

    // 销毁 Mines 单例
    static void destroyInstance();

    // 初始化矿洞地图与物品管理器
    virtual bool init() override;

    // 每帧更新（当前未使用）
    virtual void update(float dt) override;

    // 判断是否离开矿洞并切换地图
    virtual MapType leaveMap(const Vec2& curPos,
        bool isStart,
        const Direction& direction) override;

    // 进入矿洞时设置地图参数
    virtual void IntoMap(MapType lastMap) override;

    // 根据来源地图确定玩家出生点
    virtual Vec2 getPlayerStartPosition(MapType lastMap) override;

    // 矿洞内摄像机跟随玩家
    virtual bool isCameraFollow() const override { return true; }

    // 碰撞检测（含矿洞物品）
    virtual bool isCollidable(Vec2 worldPos) override;

    // 左键交互（挖矿等行为）
    virtual MouseEvent onLeftClick(const Vec2& playerPos,
        const Direction direction,
        ItemType objects) override;

private:
    // Mines 单例指针
    static GameMap* _instance;

    // 矿洞物品管理器
    MinesItemManager* _minesItemManager = nullptr;
};

#endif // __MINES_H__
