#ifndef __FARM_HOUSE_H__
#define __FARM_HOUSE_H__

#include "GameMap.h"
#include "cocos2d.h"
#include "Constant.h"
#include "../Dialogue/DialogueLayer.h"

USING_NS_CC;

// 农舍场景类
class FarmHouse : public GameMap
{
public:
    // 静态创建与销毁
    static FarmHouse* create();
    static GameMap* getInstance();
    static void destroyInstance();

    // 初始化
    virtual bool init() override;

    // 场景切换逻辑
    virtual MapType leaveMap(const Vec2& curPos, bool isStart, const Direction& direction) override;
    virtual void IntoMap(MapType lastMap) override;

    // 获取玩家初始位置
    virtual Vec2 getPlayerStartPosition(MapType lastMap) override;

    // 交互事件处理
    virtual MouseEvent onRightClick(const Vec2& playerPos, const Direction direction) override;

private:
    static GameMap* _instance;

    // 禁用拷贝赋值
    FarmHouse& operator=(const FarmHouse&) = delete;
};

#endif // __FARM_HOUSE_H__