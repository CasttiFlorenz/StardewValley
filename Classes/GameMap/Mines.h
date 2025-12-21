#pragma once
/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Mines.cpp
 * File Function: Mines类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/14
 * License:       MIT License
 ****************************************************************/

#ifndef __MINES_H__
#define __MINES_H__
#include "GameMap.h"
#include "cocos2d.h"
#include "../MapItem/MinesItemManager.h"
#include "Constant.h"
USING_NS_CC;
class Mines : public GameMap
{
public:
    CREATE_FUNC(Mines);
    static GameMap* getInstance();
    static void destroyInstance();

    virtual bool init() override;
    virtual void update(float dt) override;
    virtual std::string getNewMap(const Vec2& curPos, bool isStart, const Direction& direction) override;
    virtual void setStartPosition(std::string lastMap) override;
    virtual Vec2 getPlayerStartPosition(std::string lastMap) override;
    virtual bool isCameraFollow() const override { return true; }

    virtual bool isCollidable(Vec2 worldPos)override;

    virtual MouseEvent onLeftClick(const Vec2& playerPos, const Direction direction, Objects objects)override;
    virtual MouseEvent onRightClick(const Vec2& playerPos, const Direction direction) override ;

private:
    static GameMap* _instance;
    MinesItemManager* _minesItemManager;
};
#endif
