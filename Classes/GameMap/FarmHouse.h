#ifndef __FARM_HOUSE_H__
#define __FARM_HOUSE_H__

#include "GameMap.h"
#include "cocos2d.h"
#include "Constant.h"
#include "../Dialogue/DialogueLayer.h"

USING_NS_CC;

// 农舍地图（室内场景）
class FarmHouse : public GameMap
{
public:
    // 创建 FarmHouse 对象
    static FarmHouse* create();

    // 获取 FarmHouse 单例
    static GameMap* getInstance();

    // 销毁 FarmHouse 单例
    static void destroyInstance();

    // 初始化农舍地图与事件层
    virtual bool init() override;

    // 判断是否需要切换到其他地图
    virtual MapType leaveMap(const Vec2& curPos,
        bool isStart,
        const Direction& direction) override;

    // 进入农舍时设置地图位置与缩放
    virtual void IntoMap(MapType lastMap) override;

    // 根据来源地图确定玩家出生点
    virtual Vec2 getPlayerStartPosition(MapType lastMap) override;

    // 右键交互（检测是否点击床）
    virtual MouseEvent onRightClick(const Vec2& playerPos,
        const Direction direction) override;

private:
    // FarmHouse 单例指针
    static GameMap* _instance;

    // 禁止拷贝赋值
    FarmHouse& operator=(const FarmHouse&) = delete;
};

#endif // __FARM_HOUSE_H__
