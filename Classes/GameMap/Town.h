#pragma once
/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Town.h
 * File Function: Town类的实现
 * Author:        郭芷烟、赵睿妍
 * Update Date:   2025/12/19
 * License:       MIT License
 ****************************************************************/

#ifndef __TOWN_H__
#define __TOWN_H__
#include "GameMap.h"
#include <string>
#include <map>
#include "cocos2d.h"
#include "Constant.h"
#include"../Inventory/Item.h"
#include"../Shop/ShopLayer.h"
#include"../Shop/ShopMenuLayer.h"
#include"../NPC/NPCbase.h"
#include"../NPC/NPCManager.h"
#include"../Dialogue/DialogueLayer.h"
USING_NS_CC;

class Town : public GameMap
{
public:
    CREATE_FUNC(Town);
    static GameMap* getInstance();
    static void destroyInstance();

    virtual bool init() override;

    virtual void update(float dt) override;

    virtual std::string getNewMap(const Vec2& curPos, bool isStart, const Direction& direction) override;

    virtual void setStartPosition(std::string lastMap) override;

    virtual Vec2 getPlayerStartPosition(std::string lastMap) override;

    virtual bool isCameraFollow() const { return true; }

    virtual MouseEvent onLeftClick(const Vec2& playerPos, const Direction direction, Objects objects)override;
    virtual MouseEvent onRightClick(const Vec2& playerPos, const Direction direction) override;

    virtual bool isOutdoor() override { return true; }

    void Town::interactWithNPC(const std::string& npcName, Objects heldItem);
    NPCBase* getNPCByName(const std::string& name);
    void openShopForNPC(const std::string& npcName);
    void initNPCs();
private:
    static GameMap* _instance;
    std::map<std::string, NPCBase*> _npcMap;
};
#endif
