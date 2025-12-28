#pragma once
#ifndef __TOWN_H__
#define __TOWN_H__

#include "GameMap.h"
#include "cocos2d.h"
#include "Constant.h"

#include "../Inventory/Item.h"
#include "../Shop/ShopLayer.h"
#include "../Shop/ShopMenuLayer.h"
#include "../NPC/NPCbase.h"
#include "../NPC/NPCManager.h"
#include "../Dialogue/DialogueLayer.h"

USING_NS_CC;

// 城镇地图类（包含 NPC 和商店）
class Town : public GameMap
{
public:
    // 静态创建与销毁
    static Town* create();
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

    // 摄像机跟随与户外判断
    virtual bool isCameraFollow() const noexcept override { return true; }
    virtual bool isOutdoor() noexcept override { return true; }

    // 右键点击处理（与 NPC 交互或打开商店）
    virtual MouseEvent onRightClick(const Vec2& playerPos,
        const Direction direction) override;

    // 与指定 NPC 进行交互
    void interactWithNPC(const std::string& npcName, ItemType heldItem);

    // 获取 NPC 对象
    NPCBase* getNPCByName(const std::string& name);

    // 打开指定 NPC 的商店
    void openShopForNPC(const std::string& npcName);

    // 初始化所有 NPC
    void initNPCs();

private:
    static GameMap* _instance;

    // NPC 名称与对象映射
    std::map<std::string, NPCBase*> _npcMap;
};

#endif // __TOWN_H__