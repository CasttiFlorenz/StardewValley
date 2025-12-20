/****************************************************************
 * Project Name:  StardewValley
 * File Name:     FarmHouse.h
 * File Function: FarmHouseµÄÊµÏÖ
 * Author:        ¹ùÜÆÑÌ
 * Update Date:   2025/12/11
 * License:       MIT License
 ****************************************************************/

#ifndef __FARM_HOUSE_H__
#define __FARM_HOUSE_H__

#include "GameMap.h"
#include "cocos2d.h"
#include "Constant.h"
#include"../Inventory/Item.h"
#include"../NPC/NPCbase.h"
#include"../NPC/NPCManager.h"
#include"../Dialogue/DialogueLayer.h"
USING_NS_CC;

class FarmHouse : public GameMap
{
public:
    CREATE_FUNC(FarmHouse);
    static GameMap* getInstance();
    static void destroyInstance();

    virtual bool init() override;
    virtual void update(float dt) override;

    virtual std::string getNewMap(const Vec2& curPos,  bool isStart, const Direction& direction) override;
    
    virtual void setStartPosition(std::string lastMap) override;
    
    virtual Vec2 getPlayerStartPosition(std::string lastMap) override;

    virtual MouseEvent onLeftClick(const Vec2& playerPos, const Direction direction, Objects objects)override;
    virtual MouseEvent onRightClick(const Vec2& playerPos, const Direction direction) override;
    void sleep();
private:
    static GameMap* _instance;
};

#endif // __FARM_HOUSE_H__
