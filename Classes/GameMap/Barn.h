#pragma once

#ifndef __BARN_H__
#define __BARN_H__

#include "cocos2d.h"
#include "Constant.h"
#include "GameMap.h"
#include "../MapItem/BarnManager.h"

USING_NS_CC;

// 畜棚场景类
class Barn : public GameMap
{
public:
    // 静态创建与销毁
    static Barn* create();
    static GameMap* getInstance();
    static void destroyInstance();

    // 初始化
    virtual bool init() override;

    // 场景切换逻辑
    virtual MapType leaveMap(const Vec2& curPos, bool isStart, const Direction& direction) override;
    virtual void IntoMap(MapType lastMap) override;

    // 获取玩家初始位置
    virtual Vec2 getPlayerStartPosition(MapType lastMap) override;

    // 摄像机是否跟随
    virtual bool isCameraFollow() const { return true; }

    // 交互事件处理
    virtual MouseEvent onLeftClick(const Vec2& playerPos, const Direction direction, ItemType object) override;
    virtual MouseEvent onRightClick(const Vec2& playerPos, const Direction direction) override;

private:
    static GameMap* _instance;

    // 畜棚管理器
    BarnManager* _barnManager = nullptr;

    // 禁用拷贝赋值
    Barn& operator=(const Barn&) = delete;
};

#endif // _BARN_H_