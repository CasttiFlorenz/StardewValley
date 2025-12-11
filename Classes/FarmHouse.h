/****************************************************************
 * Project Name:  StardewValley
 * File Name:     FarmHouse.h
 * File Function: FarmHouse类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/11
 * License:       MIT License
 ****************************************************************/

#ifndef __FARM_HOUSE_H__
#define __FARM_HOUSE_H__

#include "GameMap.h"
#include "cocos2d.h"
#include "Constant.h"

USING_NS_CC;

// 农舍地图类
class FarmHouse : public GameMap
{
public:
    // 创建场景
    static GameMap* createScene();

    virtual bool init() override;
    virtual void update(float dt) override;

    // 获取新地图
    virtual std::string getNewMap(const GameMap* curMap, const Vec2& curPos, bool isStart) override;
    
    // 设置初始位置
    virtual void setStartPosition(std::string lastMap) override;
    
    // 获取玩家初始位置
    virtual Vec2 getPlayerStartPosition(std::string lastMap) override;

    CREATE_FUNC(FarmHouse);

private:
    // 这里可以添加私有成员
};

#endif // __FARM_HOUSE_H__
