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
#include "../MapItem/FarmItemManager.h"
#include "../MapItem/CultivationManager.h"
#include"../Shop/ShopLayer.h"
#include"../Shop/ShopMenuLayer.h"

USING_NS_CC;

class Farm : public GameMap
{
public:
    CREATE_FUNC(Farm);
    static GameMap* getInstance();
    static void destroyInstance();
    
    virtual ~Farm(); // 添加析构函数

    virtual bool init() override;
    virtual void update(float dt) override;

    virtual std::string getNewMap(const Vec2& curPos, bool isStart, const Direction& direction) override;

    virtual void setStartPosition(std::string lastMap) override;

    virtual bool isCameraFollow() const { return true; }

    virtual Vec2 getPlayerStartPosition(std::string lastMap) override;

    virtual bool isCollidable(Vec2 worldPos)override;

    virtual MouseEvent onLeftClick(const Vec2& playerPos, const Direction direction, Objects objects)override;
    virtual MouseEvent onRightClick(const Vec2& playerPos, const Direction direction) override;

    virtual bool isOutdoor() override { return true; }

    void openShopForNPC();

private:
    static GameMap* _instance;
    FarmItemManager* _farmItemManager;
    CultivationManager* _cultivationManager;
};

#endif // __FARM_H__
