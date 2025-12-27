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

// 城镇地图（室外场景，包含 NPC 与商店）
class Town : public GameMap
{
public:
    // 创建 Town 对象
    static Town* create();

    // 获取 Town 单例
    static GameMap* getInstance();

    // 销毁 Town 单例
    static void destroyInstance();

    // 初始化城镇地图与 NPC
    virtual bool init() override;

    // 判断是否离开城镇并切换地图
    virtual MapType leaveMap(const Vec2& curPos,
        bool isStart,
        const Direction& direction) override;

    // 进入城镇时设置地图参数
    virtual void IntoMap(MapType lastMap) override;

    // 根据来源地图确定玩家出生点
    virtual Vec2 getPlayerStartPosition(MapType lastMap) override;

    // 城镇内摄像机跟随玩家
    virtual bool isCameraFollow() const override { return true; }

    // 右键交互（NPC 互动）
    virtual MouseEvent onRightClick(const Vec2& playerPos,
        const Direction direction) override;

    // 标记为室外地图
    virtual bool isOutdoor() noexcept override { return true; }

    // 与指定 NPC 进行交互
    void interactWithNPC(const std::string& npcName, ItemType heldItem);

    // 根据名字获取 NPC 对象
    NPCBase* getNPCByName(const std::string& name);

    // 为指定 NPC 打开商店界面
    void openShopForNPC(const std::string& npcName);

    // 初始化城镇中的所有 NPC
    void initNPCs();

private:
    // Town 单例指针
    static GameMap* _instance;

    // NPC 名字到对象的映射表
    std::map<std::string, NPCBase*> _npcMap;
};

#endif // __TOWN_H__
