#pragma once
/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Barn.h
 * File Function: Barn类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/14
 * License:       MIT License
 ****************************************************************/

#ifndef __BARN_H__
#define __BARN_H__
#include "GameMap.h"
#include "cocos2d.h"
#include "Constant.h"

USING_NS_CC;

class Barn : public GameMap
{
public:
    CREATE_FUNC(Barn);
    static GameMap* getInstance();
    static void destroyInstance();

    virtual bool init() override;
    virtual void update(float dt) override;
    virtual std::string getNewMap(const Vec2& curPos, bool isStart, const Direction& direction) override;
    virtual void setStartPosition(std::string lastMap) override;
    virtual Vec2 getPlayerStartPosition(std::string lastMap) override;
    virtual bool isCameraFollow() const { return true; }

    virtual MouseEvent onLeftClick(const Vec2& playerPos, const Direction direction, Objects object)override;
    virtual MouseEvent onRightClick(const Vec2& playerPos, const Direction direction) override;

private:
    static GameMap* _instance;
};
#endif // _BARN_H_
