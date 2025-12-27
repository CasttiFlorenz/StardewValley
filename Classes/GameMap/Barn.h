#pragma once

#ifndef __BARN_H__
#define __BARN_H__

#include "cocos2d.h"
#include "Constant.h"
#include "GameMap.h"
#include "../MapItem/BarnManager.h"

USING_NS_CC;

class Barn : public GameMap
{
public:
    static Barn* create();
    static GameMap* getInstance();
    static void destroyInstance();

    // 初始化场景（加载 TMX、隐藏 event 层、初始化管理器）
    virtual bool init() override;
    
    // 基于玩家位置与朝向判断是否切换到其他地图
    virtual MapType leaveMap(const Vec2& curPos, bool isStart, const Direction& direction) override;
    
    // 设置场景起始参数（摄像机与地图定位）
    virtual void IntoMap(MapType lastMap) override;
    
    // 计算玩家在本场景的起始位置（基于入口对象）
    virtual Vec2 getPlayerStartPosition(MapType lastMap) override;
    virtual bool isCameraFollow() const { return true; }

    // 左键交互：支持在 feed 区域投放干草
    virtual MouseEvent onLeftClick(const Vec2& playerPos, const Direction direction, ItemType object)override;
    // 右键交互：支持从生产点位收集产物
    virtual MouseEvent onRightClick(const Vec2& playerPos, const Direction direction) override;

private:
    static GameMap* _instance;

    // 管理器指针仅引用单例，生命周期由管理器自身负责
    BarnManager* _barnManager = nullptr;
    Barn& operator=(const Barn&) = delete;
};
#endif // _BARN_H_
