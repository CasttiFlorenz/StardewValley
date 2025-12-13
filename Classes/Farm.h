#pragma once
/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Farm.h
 * File Function: Farm类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/13
 * License:       MIT License
 ****************************************************************/

#ifndef __FARM_H__
#define __FARM_H__

#include "GameMap.h"
#include "cocos2d.h"
#include "Constant.h"

USING_NS_CC;

// 农场地图类
class Farm : public GameMap
{
public:
    // 创建场景
    static GameMap* createScene();

    virtual bool init() override;
    virtual void update(float dt) override;

    // 获取新地图
    virtual std::string getNewMap(const Vec2& curPos, bool isStart, const Direction& direction) override;

    // 设置初始位置
    virtual void setStartPosition(std::string lastMap) override;

    virtual bool isCameraFollow() const { return true; }

    // 获取玩家初始位置
    virtual Vec2 getPlayerStartPosition(std::string lastMap) override;

    CREATE_FUNC(Farm);

private:
    // 这里可以添加私有成员
};

#endif // __FARM_H__