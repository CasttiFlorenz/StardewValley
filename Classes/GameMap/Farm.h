#pragma once

#ifndef __FARM_H__
#define __FARM_H__

#include "GameMap.h"
#include "cocos2d.h"
#include "Constant.h"
#include "../MapItem/FarmItemManager.h"      // 农场环境物品管理（木头、杂草、采集物）
#include "../MapItem/CultivationManager.h"   // 耕种系统管理（翻地、浇水、种植、收获）
#include "../Shop/ShopLayer.h"
#include "../Shop/ShopMenuLayer.h"

USING_NS_CC;

// 农场场景类，负责农场地图显示与交互逻辑
class Farm : public GameMap
{
public:
    // 创建 Farm 对象（Cocos2d-x 标准创建接口）
    static Farm* create();

    // 获取 Farm 场景单例
    static GameMap* getInstance();

    // 销毁 Farm 单例
    static void destroyInstance();

    // 初始化场景：加载 TMX 地图、隐藏 event 层、初始化管理器
    virtual bool init() override;

    // 每帧更新（当前未实现逻辑，预留扩展）
    virtual void update(float dt) override;

    // 根据玩家位置和朝向判断是否需要切换地图
    virtual MapType leaveMap(const Vec2& curPos, bool isStart, const Direction& direction) override;

    // 进入地图时的初始化设置（地图缩放、位置等）
    virtual void IntoMap(MapType lastMap) override;

    // 是否启用摄像机跟随玩家
    virtual bool isCameraFollow() const { return true; }

    // 获取玩家进入农场时的起始位置
    virtual Vec2 getPlayerStartPosition(MapType lastMap) override;

    // 碰撞检测：地图阻挡 + 农场物品阻挡
    virtual bool isCollidable(Vec2 worldPos) override;

    // 鼠标左键交互：使用工具（锄地、浇水、播种）、砍树、割草、钓鱼
    virtual MouseEvent onLeftClick(const Vec2& playerPos, const Direction direction, ItemType objects) override;

    // 鼠标右键交互：采集野生植物、收获成熟作物
    virtual MouseEvent onRightClick(const Vec2& playerPos, const Direction direction) override;

    // 是否为室外场景（影响天气系统）
    virtual bool isOutdoor() override { return true; }

    // 打开 NPC 商店界面
    void openShopForNPC();

private:
    static GameMap* _instance;                 // Farm 场景单例指针
    FarmItemManager* _farmItemManager;         // 农场环境物品管理器
    CultivationManager* _cultivationManager;   // 耕种系统管理器
};

#endif // __FARM_H__
